#pragma once

#include "cocos2d.h"

class Entity : public cocos2d::Node
{
protected:
	// Entity's sprite
	cocos2d::Sprite * sprite;

	// Entity's parameters
	float speed;

public:
	static const int TAG_BULLET = 0;
	static const int TAG_WALL = 1;
	static const int TAG_CHARACTER = 2;

	Entity();

	virtual void update(float delta) = 0;
	virtual void onCollision() = 0;

	void setPosition(float x, float y) { sprite->setPosition(x, y); }
	const cocos2d::Vec2 & getPosition() const { return sprite->getPosition(); }
	void setRotation(float rotation)
	{
		if (rotation > 360.0f) rotation -= 360.0f;
		sprite->setRotation(rotation);
	}
	float getRotation() const { return sprite->getRotation(); }
	cocos2d::Sprite * getSprite() const { return sprite; }
	float getSpeed() const { return speed; }
	void setSpeed(float speed) { this->speed = speed; }
	const float getMaxSpeed() { return 1.0f; }
	const float getMaxRotationSpeed() { return 1.0f; }
};
