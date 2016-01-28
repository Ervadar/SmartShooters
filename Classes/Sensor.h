#pragma once
#include "cocos2d.h"
#include "Character.h"

#define SENSOR_NEURAL_NETWORK 1
#define SENSOR_VISION 2

class Sensor
{
private:
	int sensorType;

	float angleOffsetInDegrees;
	cocos2d::Vec2 source;
	cocos2d::Vec2 destination;
	cocos2d::Vec2 contact;
	float fraction;
	Character* character;

public:
	Sensor(Character* bot, float angleOffsetInDegrees);
	void update(float delta);

	cocos2d::Vec2 getSource() const { return source; }
	cocos2d::Vec2 getDestination() const { return destination; }
	cocos2d::Vec2 getContact() const { return contact; }
	float getFraction() const { return fraction; }
};