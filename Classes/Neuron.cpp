#include "Neuron.h"
#include "cocos2d.h"

Neuron::Neuron(int inputCount)
	: inputCount(inputCount + 1)
{
	for (int i = 0; i < this->inputCount; ++i)
		weights.push_back(cocos2d::RandomHelper::random_real<double>(-1, 1));
}

Neuron::Neuron(std::vector<double>& weights)
{
	this->weights = weights;
	inputCount = weights.size();
}
