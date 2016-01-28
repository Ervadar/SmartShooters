#pragma once

#include "Entity.h"
#include "Bullet.h"

class Character : public Entity
{
private:
	int hp;
	float shootTimer;
	float timeBetweenShots = 1.0f;

	float rayCastLength = 150.0f;

	BulletPool * bulletPool;

	cocos2d::Label * hpLabel;

protected:
	cocos2d::Node * characterHUD;

public:
	Character();
	virtual bool init(std::string spritePath, int hp, BulletPool & bulletPool, cocos2d::Vec2 position);
	virtual void update(float delta);

	virtual void onCollision();

	void decreaseHp(int hpToDecrease);
	Bullet* shoot();
	bool canShoot() const;

	void move(cocos2d::Vec2 direction);

	int getHp() const { return hp; }
	bool isAlive() const { return hp > 0; }

	float getRayCastLength() const { return rayCastLength; }

	void updateHUDposition();
};