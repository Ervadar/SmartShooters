#pragma once
#include <vector>
#include <cocos2d.h>

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
		CCLOG("Genome weights:");
		for (double weight : weights)
		{
			CCLOG("%f", weight);
		}
	}
};