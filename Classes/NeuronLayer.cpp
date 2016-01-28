#include "NeuronLayer.h"

NeuronLayer::NeuronLayer(int neuronCount, int inPutsPerNeuron)
	: neuronCount(neuronCount)
{
	for (int i = 0; i < neuronCount; ++i)
		neurons.push_back(Neuron(inPutsPerNeuron));
}
