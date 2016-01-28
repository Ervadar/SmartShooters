#pragma once

#include <vector>
#include "NeuronLayer.h"

class NeuralNetwork
{
private:
	std::vector<NeuronLayer> neuronLayers;
	int layerCount;
	int neuronsPerHiddenLayer;
	int inputCount;
	int outputCount;

	double fitness;

public:
	NeuralNetwork(int hiddenLayerCount, int neuronsPerHiddenLayer,
				  int inputCount, int outputCount);

	virtual void update() = 0;
	std::vector<double> getNetworkOutput(std::vector<double>& input) const;

	const float sigmoid(double input, double activationResponse) const;

	// Accessors
	std::vector<double> getNeuralNetworkWeights();
};