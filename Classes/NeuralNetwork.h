#pragma once

#include <vector>
#include "NeuronLayer.h"

/*
Class representing a Neural Network.
It is initializing its neurons with random weights on construction.
*/
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

	std::vector<int> calculateSplitPoints() const;

	void initWeights(std::vector<double> weights);
	void initWeightsRandomly();
};