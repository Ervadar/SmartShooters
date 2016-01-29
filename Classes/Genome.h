#pragma once
#include <vector>

class Genome
{
public:
	std::vector<double> weights;
	double fitness;

	Genome();
	Genome(std::vector<double> weights, double fitness);

	bool operator<(const Genome& genome);
};