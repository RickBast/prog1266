/**
@file Projectile.cpp
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#include "Projectile.h"
#include "DataTables.h"
#include "Utility.h"
#include <iostream>
#include "Category.h"




namespace GEX
{
	const std::map<Projectile::Type, ProjectileData> table = initializeProjectileData();

	Projectile::Projectile(Type type) :
		Entity(1),
		_type(type),
		_sprite(TextureHolder::getInstance().get(table.at(type).texture))

	{
		centerOrigin(_sprite);
	}

	unsigned int Projectile::getCategory() const
	{
		if (_type == Type::EnemyBullet)
			return Category::EnemyProjectile;
		else
			return Category::AlliedProjectile;
	}

	float Projectile::getMaxSpeed() const
	{
		return table.at(_type).speed;
	}

	void Projectile::fire()
	{
		std::cout << "Fire" << std::endl;
	}

	void Projectile::launchMissile()
	{
		std::cout << "Missile" << std::endl;
	}

	void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		Entity::updateCurrent(dt, commands);  // call Entity base class method
	}

	void Projectile::drawCurrent(sf::RenderTarget & target, sf::RenderStates state) const
	{
		target.draw(_sprite, state);
	}

}