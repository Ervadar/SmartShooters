#include "Genome.h"

Genome::Genome() : fitness(0)
{
}

Genome::Genome(std::vector<double> weights, double fitness) : weights(weights), fitness(fitness)
{
}

bool Genome::operator<(const Genome & genome)
{
	return (fitness > genome.fitness);
}
