/**
@file DataTable.cpp
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#include "DataTables.h"



namespace GEX
{
	Direction::Direction(float a, float d) : angle(a), distance(d)
	{}

	std::map<Aircraft::Type, AircraftData> GEX::initializeAircraftData()
	{
		std::map<Aircraft::Type, AircraftData> data;

		data[Aircraft::Type::Eagle].hitPoints = 100;
		data[Aircraft::Type::Eagle].speed = 200.f;
		data[Aircraft::Type::Eagle].fireInterval = sf::seconds(1);
		data[Aircraft::Type::Eagle].texture = TextureID::Eagle;

		data[Aircraft::Type::Raptor].hitPoints = 20;
		data[Aircraft::Type::Raptor].speed = 50.f;
		data[Aircraft::Type::Raptor].texture = TextureID::Raptor;
		data[Aircraft::Type::Raptor].fireInterval = sf::Time::Zero;
		data[Aircraft::Type::Raptor].directions.push_back(Direction(+80.f, 80.f));
		data[Aircraft::Type::Raptor].directions.push_back(Direction(-80.f, 100.f));
		data[Aircraft::Type::Raptor].directions.push_back(Direction(+80.f, 80.f));

		data[Aircraft::Type::Avenger].hitPoints = 40;
		data[Aircraft::Type::Avenger].speed = 50.f;
		data[Aircraft::Type::Avenger].texture = TextureID::Avenger;
		data[Aircraft::Type::Avenger].fireInterval = sf::seconds(2);
		data[Aircraft::Type::Avenger].directions.push_back(Direction(+70.f, 150.f));
		data[Aircraft::Type::Avenger].directions.push_back(Direction(0.f, 25.f));
		data[Aircraft::Type::Avenger].directions.push_back(Direction(-70.f, 300.f));
		data[Aircraft::Type::Avenger].directions.push_back(Direction(0.f, 25.f));
		data[Aircraft::Type::Avenger].directions.push_back(Direction(+70.f, 150.f));


		return data;

	}

	std::map<Projectile::Type, ProjectileData> GEX::initializeProjectileData()
	{
		std::map<Projectile::Type, ProjectileData> data;

		data[Projectile::Type::AlliedBullet].damage = 100;
		data[Projectile::Type::AlliedBullet].speed = 300.f;
		data[Projectile::Type::AlliedBullet].texture = TextureID::Bullet;

		data[Projectile::Type::EnemyBullet].damage = 10;
		data[Projectile::Type::EnemyBullet].speed = 300.f;
		data[Projectile::Type::EnemyBullet].texture = TextureID::Bullet;

		data[Projectile::Type::EnemyBullet].damage = 200;
		data[Projectile::Type::EnemyBullet].speed = 150.f;
		data[Projectile::Type::EnemyBullet].texture = TextureID::Missile;
		return data;
	}
}


