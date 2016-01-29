#include "Player.h"
#include "cocos2d.h"
#include "Options.h"

Player::Player(isi::Game & game) : Character(), game(game)
{
}

bool Player::init(std::string spritePath, int hp, BulletPool & bulletPool, cocos2d::Vec2 position)
{
	Character::init(spritePath, hp, bulletPool, position);
	this->spawnPoint = sprite->getPosition();
	ellipseWidth = cocos2d::RandomHelper::random_real(10.0f, 30.0f);
	ellipseHeight = cocos2d::RandomHelper::random_real(10.0f, 30.0f);

	return true;
}

void Player::update(float delta)
{
	Character::update(delta);

	// Training-player-bot behaviour
	if (game.isTrainingGame())
	{
		// Ellipsoidal movement
		static float timePassed = 0.0f;
		setPosition(spawnPoint.x + cos(timePassed)*ellipseWidth,
					spawnPoint.y + sin(timePassed)*ellipseHeight);
		timePassed += delta;

		// Rotating in bot direction
		Bot* bot =	nullptr;
		std::vector<Bot*>& activeEnemies = game.getActiveEnemies();
		if (activeEnemies.size() > 0) bot = activeEnemies[0];
		if (bot)
		{
			cocos2d::Vec2 vectorToBot(bot->getPosition() - getPosition());
			float angleToBot = MATH_RAD_TO_DEG(atan2f(1.0f, 0.0f) - atan2f(vectorToBot.y, vectorToBot.x));
			if (angleToBot < 0.0f) angleToBot = (90.0f + angleToBot) + 270.0f;	// transform from -90..270 to 0..360
			setRotation(angleToBot);
		}

		// Shooting (all the time for now)
		if (isi::Options::getInstance().trainedNetwork == isi::Options::SEARCHING_NEURAL_NETWORK)
		{
			shoot();
		}
	}
}

void Player::shoot()
{
	Bullet* bullet = this->Character::shoot();
	if (bullet)
	{
		bullet->setPlayersBullet(true);
	}
}
