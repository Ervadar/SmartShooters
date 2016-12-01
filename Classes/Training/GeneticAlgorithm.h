#pragma once

#include "cocos2d.h"
#include <vector>
#include "Game/Game.h"
#include "Options.h"
#include "Training/Genome.h"

/*
Genetic algorithm class. Contains evolution logic.
*/
class GeneticAlgorithm
{
private:
	isi::Game game;

	bool running;
	int trainedNetwork;

	std::vector<Genome> genomes;		// Encoded neural network
	int generationSize;
	std::vector<Genome> newGeneration;
	int newGenerationIdx = 0;			// Index for traversing newGeneration
	int experimentTime;

	int tournamentCompetitors;
	float crossoverRate;
	float mutationRate;
	float maxPerturbationRate;
	int eliteGenomes;

	double bestFitness;

	std::vector<int> networkSplitPoints;

	int generationNumber;

	double averageFitness;
	double worstFitness;

public:
	~GeneticAlgorithm();

	void init();
	void update(float delta);

	isi::Game* getGame() { return &game; }
	void setRunning(bool running) { this->running = running; }
	bool isRunning() { return running; }

	void applySigmaScaling(std::vector<Genome>& genomes);

	Genome & rouletteWheelSelection();
	Genome & tournamentSelection();
	void crossover(Genome& mother, Genome& father, Genome& baby1, Genome& baby2);
	void mutate(Genome& genome);

	void pause();
	void unpause();

	void calculateBestWorstAverage();

	int getGenerationNumber() { return generationNumber; }
	int getExperimentTime() { return experimentTime; }
	int getGenomeCount() const { return genomes.size(); }
	int getGenerationSize() const { return generationSize; }
	int getBestFitness() const { return bestFitness; }

	void saveGenerationToFile();
};