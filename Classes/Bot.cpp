#include "Bot.h"
#include "cocos2d.h"
#include "Sensor.h"
#include "Utils.h"
#include "Game.h"
#define MAX_SENSOR_COUNT 8

Bot::Bot(isi::Game& game) : Character(),
game(game),
searchingNeuralNetwork(isi::Options::getInstance().hiddenLayerCount),
fightingNeuralNetwork(isi::Options::getInstance().hiddenLayerCount),
activeNeuralNetwork(&searchingNeuralNetwork),
fieldOfView(100.0f),
viewRange(300.0f),
fightingCircleRadius(300.0f),
angleToLastSeenPlayerPos(0.0f),
state(searching)
{
	for (int i = 0; i < MAX_SENSOR_COUNT; ++i)
		sensorDebugLabels.push_back(cocos2d::Label::createWithTTF("", "fonts/slkscr.ttf", 14));

	for (auto label : sensorDebugLabels) addChild(label);

	// USUÑ TO I WRZUC DO FUNKCJI ZMIENIAJACEJ STAN SIECI
	searchingSensors.push_back(Sensor(this, -90));
	searchingSensors.push_back(Sensor(this, -45));
	searchingSensors.push_back(Sensor(this, 0));
	searchingSensors.push_back(Sensor(this, 45));
	searchingSensors.push_back(Sensor(this, 90));

	float angle = 0;
	for (int i = 0; i < 8; ++i)
	{
		fightingSensors.push_back(Sensor(this, angle));
		angle += 45;
	}

	activeSensors = &searchingSensors;

	activeNeuralNetwork = &searchingNeuralNetwork;

	// Debug info
	debugInfoLabel = cocos2d::Label::createWithTTF("", "fonts/small_pixel.ttf", 10);
	debugInfoLabel->setPosition(cocos2d::Vec2(35, 35));
	characterHUD->addChild(debugInfoLabel, 1);
}

Bot::~Bot()
{
}

bool Bot::initNeuralNetworksRandomly()
{
	// placeholder
	static int i = 0;
	CCLOG("random network %d\n", i);
	++i;
	return true;
}

void Bot::update(float delta)
{
	this->Character::update(delta);

	auto scene = getScene();
	auto scenePosition = scene->getPosition();

	// BOT STATES
	if (state == searching)
	{
		if (isPlayerSeen())
		{
			state = fighting;
			activeNeuralNetwork = &fightingNeuralNetwork;
			activeSensors = &fightingSensors;
		}
	}
	else if (state == fighting)
	{
		cocos2d::Vec2 distanceToPlayer = game.getPlayer().getPosition() - getPosition();
		if (distanceToPlayer.getLength() > fightingCircleRadius)
		{
			state = searching;
			activeNeuralNetwork = &searchingNeuralNetwork;
			activeSensors = &searchingSensors;
		}
	}

	for (auto& sensor : *activeSensors) sensor.update(delta);
	updateHUDposition();

	// 1. Get input from sensors into a vector<double> and scale all to [0; 1]
	std::vector<double> input;
	for (auto & sensor : *activeSensors) input.push_back(sensor.getFraction());

	// 1.1 Additional input for fighting neural network
	if (state == fighting)
	{
		float angleToClosestBullet = 0.0f;
		float distanceToClosestBullet = viewRange;
		float angleToPlayer = 0.0f;
		Bullet* closestBullet = nullptr;
		for (Bullet* bullet : game.getBulletPool()->getActiveBullets())
		{
			if (bullet->isPlayersBullet())
			{
				float distanceToBullet = (bullet->getSprite()->getPosition() - getPosition()).length();
				if (distanceToBullet < distanceToClosestBullet) closestBullet = bullet;
			}
		}
		if (closestBullet)
		{
			cocos2d::Vec2 vectorToClosestBullet(closestBullet->getPosition() - getPosition());
			distanceToClosestBullet = vectorToClosestBullet.length() / viewRange;	// transform to 0..1
			cocos2d::Vec2 directionVector = cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -MATH_DEG_TO_RAD(getRotation()));
			angleToClosestBullet = MATH_RAD_TO_DEG(atan2f(directionVector.y, directionVector.x) - atan2f(vectorToClosestBullet.y, vectorToClosestBullet.x));
			if (angleToClosestBullet < 0.0f) angleToClosestBullet = (90.0f + angleToClosestBullet) + 270.0f;	// transform from -90..270 to 0..360
			angleToClosestBullet = angleToClosestBullet / 360.0f;	// transform to 0..1
			cocos2d::Vec2 vectorToPlayer(game.getPlayer().getPosition() - getPosition());
			angleToPlayer = MATH_RAD_TO_DEG(atan2f(directionVector.y, directionVector.x) - atan2f(vectorToPlayer.y, vectorToPlayer.x));
			if (angleToPlayer < 0.0f) angleToPlayer = (90.0f + angleToPlayer) + 270.0f;	// transform from -90..270 to 0..360
			angleToPlayer = angleToPlayer / 360.0f;	// transform to 0..1
		}
		input.push_back(angleToClosestBullet);
		input.push_back(distanceToClosestBullet);
		input.push_back(angleToPlayer);
	}
	
	// 2. Get output into vector<double> from getNetworkOutput(input);
	std::vector<double> output = activeNeuralNetwork->getNetworkOutput(input);
	
	// 3. Interpret output (for each different state)
	setSpeed(output[0] * getMaxSpeed());
	setRotation(getRotation() + Utils::denormalizeData(output[1], -1.0f, 1.0f) * getMaxRotationSpeed());
	if (state == fighting)
	{
		if (output[2] > 0.5f) shoot();
	}
	
	// Move the bot
	cocos2d::Vec2 directionVector = cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -MATH_DEG_TO_RAD(getSprite()->getRotation()));
	move(directionVector);

	// DEBUG
	drawDebugInfo();
}

bool Bot::isPlayerSeen()
{
	cocos2d::Vec2 vectorToPlayer(game.getPlayer().getPosition() - getPosition());
	cocos2d::Vec2 directionVector = cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -MATH_DEG_TO_RAD(getSprite()->getRotation()));
	float angleToPlayer = MATH_RAD_TO_DEG(cocos2d::Vec2::angle(vectorToPlayer, directionVector));	// absolute angle
	if (angleToPlayer < 0.5f * fieldOfView && vectorToPlayer.length() <= viewRange)
	{
		bool playerHit = false;
		float fraction = 1.0f;
		auto func = [&playerHit, &fraction, this](cocos2d::PhysicsWorld& world, const cocos2d::PhysicsRayCastInfo& info, void* data) -> bool
		{
			cocos2d::PhysicsShape* shape = info.shape;
			cocos2d::PhysicsShape* botsPhysicsShape = getSprite()->getPhysicsBody()->getFirstShape();
			cocos2d::PhysicsShape* playerPhysicsShape = game.getPlayer().getSprite()->getPhysicsBody()->getFirstShape();
			if (info.fraction < fraction && info.shape != botsPhysicsShape)
			{
				if (info.shape == playerPhysicsShape)
					playerHit = true;
				else
					playerHit = false;
				fraction = info.fraction;
			}
			return true;
		};
		getScene()->getPhysicsWorld()->rayCast(func, getPosition(), getPosition() + vectorToPlayer*viewRange, nullptr);
		return playerHit;
	}

	return false;
}

void Bot::drawDebugInfo()
{
	// DEBUG
	bool debugInfo = !isi::Options::getInstance().debugInfo;
	if (sensorsDrawNode) removeChild(sensorsDrawNode);
	if (!debugInfo)
		for (int i = 0; i < MAX_SENSOR_COUNT; ++i) sensorDebugLabels[i]->setString("");
	if (debugInfo)
	{
		sensorsDrawNode = cocos2d::DrawNode::create();
		// Debug draw sensors
		for (unsigned int i = 0; i < (*activeSensors).size(); ++i)
		{
			sensorsDrawNode->drawSegment((*activeSensors)[i].getSource(), (*activeSensors)[i].getDestination(), 1, cocos2d::Color4F::RED);
			if ((*activeSensors)[i].getFraction() < 1.0f) sensorsDrawNode->drawDot((*activeSensors)[i].getContact(), 3, cocos2d::Color4F::WHITE);
			sensorDebugLabels[i]->setPosition((*activeSensors)[i].getDestination());
			sensorDebugLabels[i]->setString(std::to_string((*activeSensors)[i].getFraction()));
		}
		for (int i = (*activeSensors).size(); i < MAX_SENSOR_COUNT; ++i)
		{
			sensorDebugLabels[i]->setString("");
		}
		// Debug draw two vision lines
		float angle = MATH_DEG_TO_RAD(getRotation() - 0.5f * fieldOfView);
		cocos2d::Vec2 direction(viewRange * (cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -angle)));
		sensorsDrawNode->drawSegment(getPosition(), getPosition() + direction, 1, cocos2d::Color4F::BLUE);
		angle = MATH_DEG_TO_RAD(getRotation() + 0.5f * fieldOfView);
		direction = viewRange * (cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -angle));
		sensorsDrawNode->drawSegment(getPosition(), getPosition() + direction, 1, cocos2d::Color4F::BLUE);
		addChild(sensorsDrawNode);
		// Debug draw fighting circle
		sensorsDrawNode->drawCircle(getPosition(), fightingCircleRadius, 360.0f, 30, false, cocos2d::Color4F::GREEN);
		// Debug info
		std::string debugInfoString;
		if (activeNeuralNetwork == &searchingNeuralNetwork) debugInfoString = "searching";
		else debugInfoString = "fighting";
		debugInfoLabel->setString(debugInfoString);
	}
}
