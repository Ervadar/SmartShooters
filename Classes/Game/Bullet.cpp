#include "Bullet.h"

/*
	BULLET
*/

bool Bullet::init()
{
	sprite = cocos2d::Sprite::create("sprites/bullet.png");
	setPhysicsBody(cocos2d::PhysicsBody::createCircle(2.5f, cocos2d::PhysicsMaterial(0.0f, 1.0f, 0.0f)));
	cocos2d::PhysicsBody* physicsBody = getPhysicsBody();
	sprite->setPhysicsBody(physicsBody);
	physicsBody->setTag(Entity::TAG_BULLET);
	physicsBody->setCategoryBitmask(0x00000001);
	physicsBody->setContactTestBitmask(0x0000000F);
	active = false;
	speed = 250.0f;
	power = 50;
	playersBullet = false;
	addChild(sprite, 2);
	return true;
}

void Bullet::update(float delta)
{
}

void Bullet::onCollision()
{
	bulletPool->deleteBullet(*this);
}

/*
	BULLET POOL
*/

bool BulletPool::init()
{
	for (int i = 0; i < BULLET_POOL_SIZE; ++i)
	{
		pool[i].bulletPool = this;
		pool[i].init();
	}
	firstAvailable = &pool[0];
	for (int i = 0; i < BULLET_POOL_SIZE - 1; ++i)
		pool[i].setNext(&pool[i + 1]);
	pool[BULLET_POOL_SIZE - 1].setNext(nullptr);

	return true;
}

Bullet * BulletPool::createBullet(cocos2d::Vec2 spawnPosition, cocos2d::Vec2 direction)
{
	assert(firstAvailable != NULL);
	Bullet* newBullet = firstAvailable;
	firstAvailable = newBullet->getNext();
	assert(firstAvailable != newBullet);
	assert(firstAvailable != NULL);
	newBullet->setActive(true);
	newBullet->getSprite()->setPosition(spawnPosition);
	newBullet->direction = direction;
	newBullet->getSprite()->getPhysicsBody()->setVelocity(direction * newBullet->speed);
	removeChild(newBullet);
	addChild(newBullet);
	return newBullet;
}

void BulletPool::deleteBullet(Bullet & bullet)
{
	if (bullet.isActive())
	{
		bullet.setActive(false);
		bullet.setNext(firstAvailable);
		bullet.playersBullet = false;
		firstAvailable = &bullet;
		removeChild(&bullet);
	}
}

std::vector<Bullet*> BulletPool::getActiveBullets()
{
	std::vector<Bullet*> activeBullets;
	for (int i = 0; i < BULLET_POOL_SIZE; ++i)
	{
		if (pool[i].isActive())
		{
			activeBullets.push_back(&pool[i]);
		}
	}
	return activeBullets;
}


