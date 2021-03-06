#pragma once

#include "Game/Character.h"
#include "NeuralNetworks/NeuralNetwork.h"
#include "NeuralNetworks/SearchingNeuralNetwork.h"
#include "NeuralNetworks/FightingNeuralNetwork.h"
#include "Options.h"
#include "Game/Sensor.h"
#include "Game/Character.h"
#include "Training/Genome.h"
#include <vector>

namespace isi { class Game; }

/*
Class representing a bot - enemy which player fights
*/
class Bot : public Character
{
private:
	NeuralNetwork* activeNeuralNetwork;
	SearchingNeuralNetwork searchingNeuralNetwork;
	FightingNeuralNetwork fightingNeuralNetwork;

	enum State {searching, fighting};
	State state;

	// Game pointer
	isi::Game& game;

	// Neural network inputs
	std::vector<Sensor>* activeSensors;
	std::vector<Sensor> searchingSensors;
	std::vector<Sensor> fightingSensors;

	float angleToLastSeenPlayerPos;

	// Bot field of view in degrees
	float fieldOfView;
	float viewRange;
	float fightingCircleRadius;

	// Training
	double fitness;
	float timeBeingTrained;
	cocos2d::Vec2 lastPosition;
	float updateFitnessTime = 0.0f;
	float rotationChange = 0.0f;

	// Debug
	cocos2d::DrawNode* sensorsDrawNode;
	std::vector<cocos2d::Label*> sensorDebugLabels;
	cocos2d::Label* debugInfoLabel;

public:
	Bot(isi::Game& game);
	bool Bot::init(std::string spritePath, int hp, BulletPool & bulletPool, cocos2d::Vec2 position);
	bool initNeuralNetworksRandomly();

	void update(float delta);

	bool isPlayerSeen();

	void drawDebugInfo();

	NeuralNetwork* getActiveNeuralNetwork() const { return activeNeuralNetwork; }
	SearchingNeuralNetwork& getSearchingNeuralNetwork() { return searchingNeuralNetwork; }
	cocos2d::DrawNode* getSensorDrawNode() const { return sensorsDrawNode; }

	double getFitness() { return fitness; }
	float getTimeBeingTrained() { return timeBeingTrained; }

	void initTrainingState();
	void updateFitness(float delta);
	void addFitness(double fitness);
};