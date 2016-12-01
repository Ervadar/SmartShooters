#pragma once
#include "Neuron.h"

/*
Class representing a neuron layer
*/
class NeuronLayer
{
public:
	std::vector<Neuron> neurons;
	int neuronCount;

public:
	NeuronLayer(int neuronCount, int inPutsPerNeuron);

	std::vector<Neuron> getNeurons() const { return neurons; }
};