#pragma once
#include "NeuralNetwork.h"

class SearchingNeuralNetwork : public NeuralNetwork
{
public:
	SearchingNeuralNetwork(int hiddenLayerCount);

	void update();
};