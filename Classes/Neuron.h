#pragma once
#include <vector>

class Neuron
{
public:
	std::vector<double> weights;	// Weights with bias weight included
	int inputCount;					// Input count with bias included
public:
	Neuron(int inputCount);
	Neuron(std::vector<double>& weights);

	void initWeights(std::vector<double> weights);
	std::vector<double> getWeights() const { return weights; }
	int getInputCount() const { return inputCount; }
};