#pragma once
#include <vector>
#include <cocos2d.h>

/*
Class representing bot's genome - neural network weights ina single vector
*/
class Genome
{
public:
	std::vector<double> weights;
	double fitness;

	Genome();
	Genome(std::vector<double> weights, double fitness);

	bool operator<(const Genome& genome);

	void printGenome()
	{
		for (double weight : weights)
		{
			CCLOG("%f", weight);
		}
	}
};