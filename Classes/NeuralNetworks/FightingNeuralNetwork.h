#pragma once

#include "NeuralNetwork.h"

class FightingNeuralNetwork : public NeuralNetwork
{
public:
	FightingNeuralNetwork(int hiddenLayerCount);

	void update();
};