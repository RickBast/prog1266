/**
@file Aorcraft.cpp
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#include "Aircraft.h"
#include "Category.h"
#include "TextNode.h"
#include "DataTables.h"
#include "Utility.h"
#include <iostream>
#include <memory>

namespace GEX
{
	const std::map<Aircraft::Type, AircraftData> table = initializeAircraftData();

	Aircraft::Aircraft(Type type) :
		Entity(table.at(type).hitPoints),
		_type(type),
		_sprite(TextureHolder::getInstance().get(table.at(type).texture)),
		_directionIndex(0),
		_travelDistance(0),
		_missileAmmo(8)

	{
		centerOrigin(_sprite);


		_fireCommand.category = Category::SceneAirLayer;
		_fireCommand.action = [this](SceneNode& node, sf::Time) 
		{
			this->createBullets(node);
		};

		_launchMissileCommand.category = Category::SceneAirLayer;
		_launchMissileCommand.action = [this](SceneNode& node, sf::Time)
		{
			this->createProjectile(node, Projectile::Type::Missile, 0.f, 0.5f);
		};

		std::unique_ptr<TextNode> healthDisplay(new TextNode(std::to_string(getHitPoints()) + "HP "));
		_healthDisplay = healthDisplay.get();
		healthDisplay->setPosition(0.f, 50.f);
		attachChild(std::move(healthDisplay));
	}

	unsigned int Aircraft::getCategory() const
	{
		switch (_type)
		{
		case Type::Eagle:
			return Category::PlayerAircraft;
		case Type::Raptor:
			return Category::EnemyAircraft;
		case Type::Avenger:
			return Category::EnemyAircraft;
		default:
			assert(0);
		}

	}

	float Aircraft::getMaxSpeed() const
	{
		return table.at(_type).speed;
	}

	void Aircraft::fire()
	{
		if (table.at(_type).fireInterval != sf::Time::Zero)
			_isFiring = true;
	}

	void Aircraft::launchMissile()
	{
		if (_missileAmmo > 0)
		{
			_isLaunchingMissile = true;
			--_missileAmmo;
		}
	}

	sf::FloatRect Aircraft::getBoundingRect() const
	{
		return getWorldTransform().transformRect(_sprite.getGlobalBounds());
	}

	bool Aircraft::isAllied() const
	{
		return _type == Aircraft::Type::Eagle;
	}

	void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		// adjust speed if on diag
		sf::Vector2f velocity = getVelocity();
		if (velocity.x != 0.f && velocity.y != 0.f)
			setVelocity(velocity / std::sqrt(2.f));

		updateTexts();
		checkProjectileLaunch(dt, commands);


		movementUpdate(dt);
		Entity::updateCurrent(dt, commands);  // call Entity base class method
	}

	void Aircraft::updateTexts()
	{
		_healthDisplay->setText(std::to_string(getHitPoints()) + " HP");
		_healthDisplay->setPosition(0.f, 50.f);
		_healthDisplay->setRotation(-getRotation());
	}

	void Aircraft::createBullets(SceneNode & node) const
	{
		Projectile::Type type = isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet;
		createProjectile(node, type, 0, 0.5f);
	}

	void Aircraft::createProjectile(SceneNode & node, Projectile::Type type, float xOffSet, float yOffSet) const
	{
		// node is the ScreenAirLayer node that the command was targeted to
		std::unique_ptr<Projectile> projectile(new Projectile(type));

		sf::Vector2f offset(xOffSet * _sprite.getGlobalBounds().width, yOffSet * _sprite.getGlobalBounds().height);
		sf::Vector2f velocity(0, projectile->getMaxSpeed());
		float sign = isAllied() ? -1.f : 1;
		projectile->setPosition(getWorldPosition()+ offset * sign);
		projectile->setVelocity(velocity * sign);
		node.attachChild(std::move(projectile));
	}

	void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
	{
		if (!isAllied())
			fire();
		if (_isFiring)
		{
			commands.push(_fireCommand);
			_isFiring = false;
		}

		if (_isLaunchingMissile)
		{
			commands.push(_launchMissileCommand);
			_isLaunchingMissile = false;
		}
	}

	void Aircraft::drawCurrent(sf::RenderTarget & target, sf::RenderStates state) const
	{
		target.draw(_sprite, state);
	}

	void Aircraft::movementUpdate(sf::Time dt)
	{
		const std::vector<Direction>& directions = table.at(_type).directions;
		if (!directions.empty())
		{
			float distanceToTravel = directions.at(_directionIndex).distance;
			if (_travelDistance > distanceToTravel)
			{
				_directionIndex = (_directionIndex + 1) % directions.size();
				_travelDistance = 0;
			}
			_travelDistance += getMaxSpeed() * dt.asSeconds();
			float dirAngle = directions.at(_directionIndex).angle + 90.f;
			float vx = getMaxSpeed() * GEX::cos(dirAngle);
			float vy = getMaxSpeed() * GEX::sin(dirAngle);
			setVelocity(vx, vy);
		}
	}
}