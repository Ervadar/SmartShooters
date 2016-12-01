#include "Sensor.h"
#include "cocos2d.h"
#include "Character.h"

using namespace cocos2d;

Sensor::Sensor(Character * character, float angleOffsetInDegrees) : character(character), angleOffsetInDegrees(angleOffsetInDegrees)
{
}

void Sensor::update(float delta)
{
	float angle = MATH_DEG_TO_RAD(character->getRotation() + angleOffsetInDegrees);
	Vec2 direction(character->getRayCastLength() * (Vec2(0, 1).rotateByAngle(Vec2(0, 0), -angle)));
	source = character->getPosition();
	destination = source + direction;

	fraction = 1.0f;
	auto func = [this](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data) -> bool
	{
		PhysicsShape* shape = info.shape;
		PhysicsShape* botsPhysicsShape = character->getSprite()->getPhysicsBody()->getFirstShape();
		if (info.fraction < fraction && info.shape != botsPhysicsShape)
		{
			contact = info.contact;
			fraction = info.fraction;
		}
		return true;
	};
	character->getScene()->getPhysicsWorld()->rayCast(func, source, destination, nullptr);
}
