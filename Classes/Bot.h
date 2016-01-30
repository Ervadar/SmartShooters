#pragma once

#include "Character.h"
#include "NeuralNetwork.h"
#include "SearchingNeuralNetwork.h"
#include "FightingNeuralNetwork.h"
#include "Options.h"
#include "Sensor.h"
#include "Character.h"
#include "Genome.h"
#include <vector>

namespace isi { class Game; }

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
	int fitness;
	float timeBeingTrained;

	// Debug
	cocos2d::DrawNode* sensorsDrawNode;
	std::vector<cocos2d::Label*> sensorDebugLabels;
	cocos2d::Label* debugInfoLabel;

public:
	Bot(isi::Game& game);
	~Bot();
	bool initNeuralNetworksRandomly();

	void update(float delta);

	bool isPlayerSeen();

	void drawDebugInfo();

	NeuralNetwork* getActiveNeuralNetwork() const { return activeNeuralNetwork; }
	cocos2d::DrawNode* getSensorDrawNode() const { return sensorsDrawNode; }

	int getFitness() { return fitness; }
	float getTimeBeingTrained() { return timeBeingTrained; }
	Genome getGenomeFromNeuralNetwork();

	void initTrainingState();
};