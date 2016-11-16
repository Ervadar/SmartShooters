#include "Character.h"
#include <SimpleAudioEngine.h>

Character::Character()
{
	characterHUD = cocos2d::Node::create();
	this->addChild(characterHUD, 2);
}

bool Character::init(std::string spritePath, int hp, BulletPool & bulletPool, cocos2d::Vec2 position)
{
	sprite = cocos2d::Sprite::create("sprites/" + spritePath);
	setPhysicsBody(cocos2d::PhysicsBody::createCircle(15.0f, cocos2d::PhysicsMaterial(1.0f, 0.0f, 0.0f)));
	cocos2d::PhysicsBody* physicsBody = getPhysicsBody();
	physicsBody->setTag(Entity::TAG_CHARACTER);
	physicsBody->setCategoryBitmask(0x00000002);
	physicsBody->setContactTestBitmask(0x0000000D);
	sprite->setPhysicsBody(physicsBody);
	addChild(sprite, 1);
	this->bulletPool = &bulletPool;
	this->speed = 1.5f;

	initState(position, hp);

	hpLabel = cocos2d::Label::createWithTTF("", "fonts/small_pixel.ttf", 10);
	updateHUDposition();
	characterHUD->addChild(hpLabel, 1);
	scheduleUpdate();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("sound_effects/shoot.wav");
	return true;
}

void Character::initState(cocos2d::Vec2 position, int hp)
{
	active = true;
	setPosition(position.x, position.y);
	this->hp = hp;
}

void Character::update(float delta)
{
	if (!canShoot())
	{
		shootTimer -= delta;
	}
	std::string additionalDebugInfo = "";
	hpLabel->setString(std::to_string(this->hp) + additionalDebugInfo);
	updateHUDposition();
}

void Character::onCollision()
{

}

Bullet* Character::shoot()
{
	if (canShoot())
	{
		float angleInDegrees = getSprite()->getRotation();
		float angleInRadians = MATH_DEG_TO_RAD(angleInDegrees);
		cocos2d::Vec2 directionVector = cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -angleInRadians);
		cocos2d::Vec2 spawnPosition = getSprite()->getPosition() + directionVector * 20.0f;
		Bullet* bullet = bulletPool->createBullet(spawnPosition, directionVector);
		shootTimer = timeBetweenShots;
		return bullet;
	}
	return nullptr;
}

bool Character::canShoot() const
{
	return shootTimer < 0;
}

void Character::move(cocos2d::Vec2 direction)
{
	float x = sprite->getPosition().x + getSpeed() * direction.x;
	float y = sprite->getPosition().y + getSpeed() * direction.y;
	sprite->setPosition(x, y);
	updateHUDposition();
}

void Character::updateHUDposition()
{
	characterHUD->setPosition(sprite->getPosition() - cocos2d::Vec2(16, 16));
}

void Character::decreaseHp(int hpToDecrease)
{
	hp -= hpToDecrease;
	if (!isAlive())
	{
		hp = 0;
		deactivate();
		setVisible(false);
		setPosition(-10000.0f, -10000.0f);		// Magic disappearing position
		//removeFromParentAndCleanup(true);
	}
}
