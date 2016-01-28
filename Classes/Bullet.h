#pragma once

#include "cocos2d.h"
#include "Entity.h"

class Bullet : public Entity
{
	friend class BulletPool;

private:
	BulletPool * bulletPool;
	Bullet* next;
	bool active;
	cocos2d::Vec2 direction;
	
	bool playersBullet;

	int power;

	bool init();

public:
	Bullet() {};
	virtual void update(float delta);
	virtual void onCollision();

	Bullet* getNext() const { return next; }
	void setNext(Bullet* next) { this->next = next; }
	void setActive(bool active) { this->active = active; }
	bool isActive() { return active; }
	void setDirection(cocos2d::Vec2 direction) { direction = direction; }
	cocos2d::Vec2 getDirection() { return direction; }
	int getPower() const { return power; }
	void setPlayersBullet(bool playersBullet) { this->playersBullet = playersBullet; }
	bool isPlayersBullet() const { return playersBullet; }
};

class BulletPool : public cocos2d::Node
{
private:
	static const int BULLET_POOL_SIZE = 50;
	Bullet pool[BULLET_POOL_SIZE];

	Bullet* firstAvailable;

public:
	BulletPool() {};
	virtual bool init();
	Bullet * createBullet(cocos2d::Vec2 spawnPosition, cocos2d::Vec2 direction);
	void deleteBullet(Bullet & bullet);

	std::vector<Bullet *> getActiveBullets();
};