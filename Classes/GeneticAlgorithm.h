#pragma once

#include "cocos2d.h"
#include <vector>
#include "Game.h"
#include "Options.h"

class GeneticAlgorithm
{
private:
	isi::Game * game;

public:
	void init();
	void update(float delta);

	void loadRandomNeuralNetworks();

	isi::Game* getGame() { return game; }
};