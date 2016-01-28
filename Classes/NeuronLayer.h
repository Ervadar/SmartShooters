#pragma once
#include "Neuron.h"

class NeuronLayer
{
private:
	std::vector<Neuron> neurons;
	int neuronCount;

public:
	NeuronLayer(int neuronCount, int inPutsPerNeuron);

	std::vector<Neuron> getNeurons() const { return neurons; }
};