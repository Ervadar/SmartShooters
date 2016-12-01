#pragma once

#include "cocos2d.h"
#include "Player.h"
#include "Bot.h"
#include "Bullet.h"
#include <memory>
#include <vector>

class Player;

namespace isi
{
	/*
	Class representing game instance
	*/
	class Game : public cocos2d::Node
	{
	private:
		// Tile map
		cocos2d::TMXTiledMap * tileMap;
		cocos2d::TMXLayer * walls;

		// Bullets
		BulletPool bulletPool;

		// Player
		std::unique_ptr<Player> player;

		// Enemies
		cocos2d::Vector<Bot*> enemies;

		bool trainingGame;

		bool paused = false;

		// Used generation of neural networks (loaded from file)
		std::vector<std::vector<double>> loadedNeuralNetworkGeneration;

	public:
		Game();
		~Game();
		virtual bool init(bool trainingGame);
		void update(float delta);
		void restart();

		bool onContactBegin(cocos2d::PhysicsContact& contact);
		void bulletCollisionWithBody(Bullet * bullet, cocos2d::PhysicsBody * body);

		int getAliveEnemiesCount();

		cocos2d::Vec2 getRandomObjCoordsFromMapLayer(std::string objectGroupName, std::string objectNamePrefix);

		cocos2d::Vec2 tileCoordsFromPosition(cocos2d::Vec2 position);
		void move(Character & character, cocos2d::Vec2 direction);
		cocos2d::TMXTiledMap * getTileMap() const { return tileMap; }
		Player & getPlayer() { return *player; }
		cocos2d::Vector<Bot*>& getEnemies() { return enemies; }
		cocos2d::Vector<Bot*> getActiveEnemies();

		BulletPool * getBulletPool() { return &bulletPool; }

		std::vector<cocos2d::PhysicsBody*> getAllPhysicsBodies();
		bool isTrainingGame() { return trainingGame; }

		void pause();
		void unpause();
		bool isPaused() { return paused; }

		void loadNeuralNetworkGeneration();
	};

};