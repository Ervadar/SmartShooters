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
viewRange(250.0f),                
fightingCircleRadius(150.0f),
angleToLastSeenPlayerPos(0.0f),
state(searching)
{
	for (int i = 0; i < MAX_SENSOR_COUNT; ++i)
		sensorDebugLabels.push_back(cocos2d::Label::createWithTTF("", "fonts/slkscr.ttf", 14));

	for (auto label : sensorDebugLabels) addChild(label);

	// TO DELETE and move to state changing function
	searchingSensors.push_back(Sensor(this, -75));
	searchingSensors.push_back(Sensor(this, -50));
	searchingSensors.push_back(Sensor(this, -25));
	searchingSensors.push_back(Sensor(this, 0));
	searchingSensors.push_back(Sensor(this, 25));
	searchingSensors.push_back(Sensor(this, 50));
	searchingSensors.push_back(Sensor(this, 75));
	
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

bool Bot::init(std::string spritePath, int hp, BulletPool & bulletPool, cocos2d::Vec2 position)
{
	Character::init(spritePath, hp, bulletPool, position);
	// Training
	if (game.isTrainingGame())
	{
		initTrainingState();
	}
	return true;
}

bool Bot::initNeuralNetworksRandomly()
{
	searchingNeuralNetwork.initWeightsRandomly();
	fightingNeuralNetwork.initWeightsRandomly();
	return true;
}

void Bot::update(float delta)
{
	if (!isActive()) return;
	this->Character::update(delta);

	auto scene = getScene();
	auto scenePosition = scene->getPosition();

	// BOT STATES
	if (state == searching && !game.isTrainingGame())
	{
		if (isPlayerSeen())
		{
			state = fighting;
			//activeNeuralNetwork = &fightingNeuralNetwork;
			//activeSensors = &fightingSensors;
		}
	}
	else if (state == fighting && !game.isTrainingGame())
	{
		cocos2d::Vec2 distanceToPlayer = game.getPlayer().getPosition() - getPosition();
		if (distanceToPlayer.getLength() > fightingCircleRadius && !isPlayerSeen())
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
	//if (state == fighting)
	//{
	//	float angleToClosestBullet = 0.0f;
	//	float distanceToClosestBullet = viewRange;
	//	float angleToPlayer = 0.0f;
	//	Bullet* closestBullet = nullptr;
	//	for (Bullet* bullet : game.getBulletPool()->getActiveBullets())
	//	{
	//		if (bullet->isPlayersBullet())
	//		{
	//			float distanceToBullet = (bullet->getSprite()->getPosition() - getPosition()).length();
	//			if (distanceToBullet < distanceToClosestBullet) closestBullet = bullet;
	//		}
	//	}
	//	if (closestBullet)
	//	{
	//		cocos2d::Vec2 vectorToClosestBullet(closestBullet->getPosition() - getPosition());
	//		distanceToClosestBullet = vectorToClosestBullet.length() / viewRange;	// transform to 0..1
	//		cocos2d::Vec2 directionVector = cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -MATH_DEG_TO_RAD(getRotation()));
	//		angleToClosestBullet = MATH_RAD_TO_DEG(atan2f(directionVector.y, directionVector.x) - atan2f(vectorToClosestBullet.y, vectorToClosestBullet.x));
	//		if (angleToClosestBullet < 0.0f) angleToClosestBullet = (90.0f + angleToClosestBullet) + 270.0f;	// transform from -90..270 to 0..360
	//		angleToClosestBullet = angleToClosestBullet / 360.0f;	// transform to 0..1
	//		cocos2d::Vec2 vectorToPlayer(game.getPlayer().getPosition() - getPosition());
	//		angleToPlayer = MATH_RAD_TO_DEG(atan2f(directionVector.y, directionVector.x) - atan2f(vectorToPlayer.y, vectorToPlayer.x));
	//		if (angleToPlayer < 0.0f) angleToPlayer = (90.0f + angleToPlayer) + 270.0f;	// transform from -90..270 to 0..360
	//		angleToPlayer = angleToPlayer / 360.0f;	// transform to 0..1
	//	}
	//	input.push_back(angleToClosestBullet);
	//	input.push_back(distanceToClosestBullet);
	//	input.push_back(angleToPlayer);
	//}
	
	// 2. Get output into vector<double> from getNetworkOutput(input);
	if (state == searching)
	{
		std::vector<double> output = activeNeuralNetwork->getNetworkOutput(input);

		// 3. Interpret output (for each different state)
		setSpeed(output[0] * getMaxSpeed());
		rotationChange = Utils::denormalizeData(output[1], -1.0f, 1.0f) * getMaxRotationSpeed();
		setRotation(getRotation() + rotationChange);
		//if (state == fighting)
		//{
		//	if (output[2] > 0.5f) shoot();
		//}

		// Move the bot
		cocos2d::Vec2 directionVector = cocos2d::Vec2(0, 1).rotateByAngle(cocos2d::Vec2(0, 0), -MATH_DEG_TO_RAD(getSprite()->getRotation()));
		move(directionVector);
	}
	else if (state == fighting)		// Placeholder fighting behaviour (rotation to player and shooting all the time)
	{
		// Rotating in player direction
		Player& player = game.getPlayer();
		cocos2d::Vec2 vectorToPlayer(player.getPosition() - getPosition());
		float angleToPlayer = MATH_RAD_TO_DEG(atan2f(1.0f, 0.0f) - atan2f(vectorToPlayer.y, vectorToPlayer.x));
		if (angleToPlayer < 0.0f) angleToPlayer = (90.0f + angleToPlayer) + 270.0f;	// transform from -90..270 to 0..360
		setRotation(angleToPlayer);
		CCLOG("ROTE: %f", getRotation());
		shoot();
	}
	// DEBUG
	drawDebugInfo();

	// Training
	if (game.isTrainingGame())
	{
		timeBeingTrained += delta;
		updateFitness(delta);
	}
}

void Bot::updateFitness(float delta)
{	
	updateFitnessTime += delta;

	if (updateFitnessTime > 3)		// Every second update fitness with passed length in that period
	{
		updateFitnessTime = 0.0f;

		cocos2d::Vec2 newPosition = getPosition();
		float length = (newPosition - lastPosition).length();
		lastPosition = newPosition;

		fitness += 3 * 5 * Utils::normalizeData(length, 0.0f, 36.0f);	// 36.0f, magic number - avg length is close 36 on my machine
	}

	if (abs(rotationChange) < 0.5f)	// Each frame improve fitness if rotation is less than certain value
	{
		fitness += 1.0f * delta;
	}

	for (unsigned int i = 0; i < activeSensors->size(); ++i)
	{
		float sensorFraction = (*activeSensors)[i].getFraction();
		//if (sensorFraction < 0.4f)	// Each frame punish the bot if its sensor fraction is lower than some value
		//{
		//	addFitness(-10);
		//}
		float penalty = -0.6f;
		if (i == 1 || i == 3) penalty = -0.9f;
		if (i == activeSensors->size()/2) penalty = -1.2f;	// 2x bigger penalty for middle sensor
		if(sensorFraction < 0.7f) addFitness(delta * penalty * (1.0f - sensorFraction));
	}

}

void Bot::addFitness(double fitness)
{
	this->fitness += fitness;
	if (this->fitness < 0.0f) this->fitness = 0.0f;
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
			if (info.fraction < fraction && info.shape != botsPhysicsShape && info.shape->getBody()->getTag() != Entity::TAG_BULLET)
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
		if (state == searching) debugInfoString = "searching";
		else if (state == fighting) debugInfoString = "fighting";
		if (game.isTrainingGame()) debugInfoString += " " + std::to_string(fitness);
		debugInfoLabel->setString(debugInfoString);
	}
	else
	{
		if (game.isTrainingGame()) debugInfoLabel->setString("Fitness: " + std::to_string(fitness));
		for (int i = 0; i < MAX_SENSOR_COUNT; ++i) sensorDebugLabels[i]->setString("");
	}
}

void Bot::initTrainingState()
{
	auto options = isi::Options::getInstance();
	if (options.trainedNetwork == options.SEARCHING_NEURAL_NETWORK) fitness = options.searchingTrainInitialFitness;
	else fitness = 0;
	timeBeingTrained = 0.0f;
	lastPosition = getPosition();
	updateFitnessTime = 0.0f;
}
