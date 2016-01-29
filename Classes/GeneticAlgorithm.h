#pragma once

#include "cocos2d.h"
#include <vector>
#include "Game.h"
#include "Options.h"
#include "Genome.h"

class GeneticAlgorithm
{
private:
	isi::Game * game;

	bool running;
	int trainedNetwork;

	std::vector<Genome> genomes;		// Encoded neural network
	int generationSize;
	int timeToSearch;

public:
	void init();
	void update(float delta);

	isi::Game* getGame() { return game; }
	void setRunning(bool running) { this->running = running; }
	bool isRunning() { return running; }
};