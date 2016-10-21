/**
@file World.cpp
@author  R Bastarache <rick_hockey8@hotmail.com>
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt
These additions and modifications are my sole work for prog 1266

@section DESCRIPTION


*/
#include "World.h"
#include <algorithm>
namespace GEX
{
	World::World(sf::RenderWindow& window) :
		_window(window),
		_worldView(window.getDefaultView()),
		_sceneGraph(),
		_sceneLayers(),
		_commandQueue(),
		_worldBounds(0.f, 0.f, _worldView.getSize().x, 2000.f),
		_spawnPosition(_worldView.getSize().x / 2.f, _worldBounds.height - _worldView.getSize().y / 2.f),
		_scrollSpeed(-50.f),
		_playerAircraft(nullptr)
	{
		buildScene();
		addEnemies();

		// Prepare the view
		_worldView.setCenter(_spawnPosition);
	}

	void World::update(sf::Time dt)
	{
		// Scroll the world
		_worldView.move(0.f, _scrollSpeed * dt.asSeconds());
		_playerAircraft->setVelocity(0.f, _scrollSpeed);

		// run all the commands
		while (!_commandQueue.isEmpty())
			_sceneGraph.onCommand(_commandQueue.pop(), dt);
		spawnEnemies();
		// Apply movements
		_sceneGraph.update(dt, _commandQueue);
		adaptPlayerPostition();
	}

	void World::adaptPlayerPostition()
	{
		// Keep player's position inside the screen bounds, 
		   //at least borderDistance units from the border

		sf::FloatRect viewBounds(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
		const float borderDistance = 35.f;

		sf::Vector2f position = _playerAircraft->getPosition();
		position.x = std::max(position.x, viewBounds.left + borderDistance);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
		position.y = std::max(position.y, viewBounds.top + borderDistance);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
		_playerAircraft->setPosition(position);
	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100;
		bounds.height += 100;
		return bounds;
	}

	void World::spawnEnemies()
	{
		while (!_enemySpawnPoints.empty() && _enemySpawnPoints.back().y > getBattlefieldBounds().top)
		{
			auto spawn = _enemySpawnPoints.back();
			std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type));
			enemy->setPosition(spawn.x, spawn.y);
			enemy->setRotation(180);
			_sceneLayers[Air]->attachChild(std::move(enemy));
			_enemySpawnPoints.pop_back();
		}
	}

	void World::addEnemies()
	{
		addEnemy(Aircraft::Type::Raptor, -250.f, -200.f);
		addEnemy(Aircraft::Type::Raptor, +250.f, -200.f);
		addEnemy(Aircraft::Type::Raptor, -350.f, -400.f);
		addEnemy(Aircraft::Type::Raptor, +350.f,- 400.f);

		addEnemy(Aircraft::Type::Avenger, -250.f, -800.f);
		addEnemy(Aircraft::Type::Avenger, +250.f, -800.f);
		addEnemy(Aircraft::Type::Avenger, -350.f, -900.f);
		addEnemy(Aircraft::Type::Avenger, +350.f, -900.f);

		std::sort(_enemySpawnPoints.begin(), _enemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs) {return lhs.y < rhs.y;});
	}

	void World::addEnemy(Aircraft::Type type, float _x, float _y)
	{
		addEnemy(SpawnPoint(type, _x, _y));
	}

	void World::addEnemy(SpawnPoint sPoint)
	{
		sPoint.x = _spawnPosition.x + sPoint.x;
		sPoint.y = _spawnPosition.y + sPoint.y;
		_enemySpawnPoints.push_back(sPoint);
	}

	void World::draw()
	{
		_window.setView(_worldView);
		_window.draw(_sceneGraph);
	}

	CommandQueue&  World::getCommandQueue()
	{
		return _commandQueue;
	}

	void World::buildScene()
	{
		// Put the layer nodes into the scene graph
		for (std::size_t i = 0; i < LayerCount; ++i)
		{
			Category::Type category = (1 == Air) ? Category::SceneAirLayer : Category::None;
			SceneNode::Ptr layer(new SceneNode(category));
			_sceneLayers[i] = layer.get();
			_sceneGraph.attachChild(std::move(layer));
		}

		// Prepare the tiled background
		sf::Texture& texture = TextureHolder::getInstance().get(TextureID::Jungle);
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(true);

		// Add the background sprite to the scene
		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);
		_sceneLayers[Background]->attachChild(std::move(backgroundSprite));

		// Add player's aircraft
		std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Type::Eagle));
		_playerAircraft = leader.get();
		_playerAircraft->setPosition(_spawnPosition);
		_playerAircraft->setVelocity(0.f, _scrollSpeed);
		_sceneLayers[Air]->attachChild(std::move(leader));
	}
}
