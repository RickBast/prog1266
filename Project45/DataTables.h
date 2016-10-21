/**
@file DataTable.h
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#pragma once
#include "ResourceIdentifiers.h"
#include "Aircraft.h"
#include <map>
#include <vector>
#include "Projectile.h"
//#include "Pickup.h"
namespace GEX
{	
	struct Direction
	{
		Direction(float a, float d);
		float angle;
		float distance;
	};
	struct AircraftData
	{
		int						hitPoints;
		float					speed;
		TextureID				texture;
		sf::Time				fireInterval;
		std::vector<Direction>	directions;
	};
	struct ProjectileData
	{
		int			damage;
		float		speed;
		TextureID	texture;
	};

	struct PickupData
	{
		std::function<void(Aircraft&)>	action;
		TextureID						texture;
	};


	std::map<Aircraft::Type, AircraftData>		initializeAircraftData();
	std::map<Projectile::Type, ProjectileData>	initializeProjectileData();

}
