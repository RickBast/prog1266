/**
@file Aircraft.h
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#pragma once
#include "Entity.h"
#include "TextureHolder.h"
#include <SFML/Graphics.hpp>
#include "Command.h"
#include "Projectile.h"
#include "CommandQueue.h"

namespace GEX
{
	class TextNode;
	class Aircraft : public Entity
	{
	public:
		enum Type {
			Raptor,
			Eagle,
			Avenger
		};

							Aircraft(Type type = Type::Eagle);
		unsigned int		getCategory() const override;
		float				getMaxSpeed() const;
		void				fire();
		void				launchMissile();
		sf::FloatRect		getBoundingRect() const override;
		bool				isAllied() const;

	private:
		void				drawCurrent(sf::RenderTarget& target, sf::RenderStates state) const override;
		void				movementUpdate(sf::Time dt);
		virtual void        updateCurrent(sf::Time dt, CommandQueue& commands) override;
		void				updateTexts();
		void				createBullets(SceneNode& node) const;
		void				createProjectile(SceneNode& node, Projectile::Type type, float xOffSet, float yOffSet) const;
		void				checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
		Type				_type;
		sf::Sprite			_sprite;
		TextNode*			_healthDisplay;
		int					_directionIndex;
		float				_travelDistance;
		Command				_fireCommand;
		Command				_launchMissileCommand;
		std::size_t			_missileAmmo;
		std::size_t			_fireRateLevel;
		std::size_t			_spreadLevel;
		bool				_isFiring;
		bool				_isLaunchingMissile;
		sf::Time			_fireCountdown;

	};
}