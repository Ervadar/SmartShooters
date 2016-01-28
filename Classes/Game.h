#pragma once

#include "cocos2d.h"
#include "Player.h"
#include "Bot.h"
#include "Bullet.h"
#include <vector>

class Player;

namespace isi
{

	class Game : public cocos2d::Node
	{
	private:
		// Tile map
		cocos2d::TMXTiledMap * tileMap;
		cocos2d::TMXLayer * walls;

		// Bullets
		BulletPool bulletPool;

		// Player
		Player * player;

		// Enemies
		std::vector<Bot*> enemies;

		bool trainingGame;


	public:
		Game();
		~Game();
		virtual bool init(bool trainingGame);
		void update(float delta);

		bool onContactBegin(cocos2d::PhysicsContact& contact);
		void bulletCollisionWithBody(Bullet * bullet, cocos2d::PhysicsBody * body);

		int getAliveEnemiesCount();

		cocos2d::Vec2 tileCoordsFromPosition(cocos2d::Vec2 position);
		void move(Character & character, cocos2d::Vec2 direction);
		cocos2d::TMXTiledMap * getTileMap() const { return tileMap; }
		Player & getPlayer() { return *player; }
		std::vector<Bot*> getEnemies() { return enemies; }

		BulletPool * getBulletPool() { return &bulletPool; }

		std::vector<cocos2d::PhysicsBody*> getAllPhysicsBodies();
		bool isTrainingGame() { return trainingGame; }
	};

};