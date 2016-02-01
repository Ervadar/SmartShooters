#include "GeneticAlgorithm.h"
#include "Utils.h"
#include <fstream>

GeneticAlgorithm::~GeneticAlgorithm()
{
}

void GeneticAlgorithm::init()
{
	// Create game instances
	auto& options = isi::Options::getInstance();

	game.init(true);

	// Load weights from file and override default random initialization
	// TODO: ma byæ tylko if (options.lnn) -> jeœli true to prze³adowujemy losowe wagi (wczytane konstruktorami) podanymi
	if (options.loadNeuralNetworks)
	{
		CCLOG("TODO: load weights from file into enemmies \n");
	}

	trainedNetwork = options.trainedNetwork;
	generationSize = options.generationSize;
	experimentTime = options.timeToSearch;
	tournamentCompetitors = options.searchingTrainTournamentCompetitors;
	crossoverRate = options.searchingTrainCrossoverRate;
	mutationRate = options.searchingTrainMutationRate;
	maxPerturbationRate = options.searchingTrainMaxPerturbationRate;
	bestFitness = options.searchingTrainInitialFitness;

	networkSplitPoints = game.getEnemies()[0]->getActiveNeuralNetwork()->calculateSplitPoints();

	setRunning(true);

	generationNumber = 1;

	// Clean fittest genome file
	std::string fittestGenomesFilePath = "average_fitness.txt";
	std::ofstream fittestGenomesFile(fittestGenomesFilePath, std::ofstream::trunc);
	fittestGenomesFile.close();


	averageFitness = 0.0f;
	worstFitness = options.searchingTrainInitialFitness;

	// Create saving directories
	//std::string dirPath = cocos2d::FileUtils::sharedFileUtils()->getWritablePath();
	//cocos2d::FileUtils::sharedFileUtils()->createDirectory(dirPath + "searching");

}

void GeneticAlgorithm::update(float delta)
{
	static float timePassed = 0.0f;
	timePassed += delta;

	if (isRunning())
	{
		if (genomes.size() < generationSize)	// Bots werent tested yet
		{
			if (game.getActiveEnemies().size() == 0)	// Experiment ended
			{
				game.restart();
				if (generationNumber == 1)		// Generation 1 - keep initializing randomly
				{
					for (Bot* bot : game.getEnemies()) bot->initNeuralNetworksRandomly();
				}
				else							// Generation > 1 - initialize from new genome generation
				{
					for (Bot* bot : game.getEnemies())
					{
						bot->getActiveNeuralNetwork()->initWeights(newGeneration[newGenerationIdx].weights);
						++newGenerationIdx;
					}
				}
			}

			for (Bot* bot : game.getActiveEnemies())	// For each boot see if it finished its training, if yes - push genome and deativate bot
			{
				if (bot->getTimeBeingTrained() > experimentTime)
				{
					int botFitness = bot->getFitness();
					genomes.push_back(Genome(bot->getActiveNeuralNetwork()->getNeuralNetworkWeights(), botFitness));
					if (botFitness > bestFitness)
					{
						fittestGenomeId = genomes.size()-1;
					}
					bot->deactivate();
					bot->setVisible(false);
				}
			}

		}
		else     // Bots were tested, creating new generation
		{
			newGeneration.clear();
			newGenerationIdx = 0;

			calculateBestWorstAverage();

			// Elitism (1 genome always pushed)
			newGeneration.push_back(genomes[fittestGenomeId]);

			// Sigma scaling	// can use rank scaling instead
			applySigmaScaling(genomes);

			while (newGeneration.size() < generationSize)
			{
				// Tournament selection (can also use rouletteWheelSelection() here)
				Genome& mother = tournamentSelection();
				Genome& father = tournamentSelection();

				// Crossover
				Genome baby1, baby2;
				crossover(mother, father, baby1, baby2);

				// Mutation
				mutate(baby1);
				mutate(baby2);

				newGeneration.push_back(baby1);
				newGeneration.push_back(baby2);
			}

			// Save generation to file
			saveGenerationToFile();

			genomes.clear();
			++generationNumber;
		}
	}
}

void GeneticAlgorithm::applySigmaScaling(std::vector<Genome>& genomes)
{
	double runningTotal = 0.0f;
	for (int genomeIdx = 0; genomeIdx < genomes.size(); ++genomeIdx)
	{
		runningTotal += (genomes[genomeIdx].fitness - averageFitness) * (genomes[genomeIdx].fitness - averageFitness);
	}
	double variance = runningTotal / (double)generationSize;
	double sigma = sqrt(variance);

	// Reassign fitness scores
	for (int genomeIdx = 0; genomeIdx < genomes.size(); ++genomeIdx)
	{
		double oldFitness = genomes[genomeIdx].fitness;
		genomes[genomeIdx].fitness = ((oldFitness - averageFitness) / (2 * sigma));
		if (genomes[genomeIdx].fitness < 0.0f) genomes[genomeIdx].fitness = 0.0f;
	}
}

Genome & GeneticAlgorithm::rouletteWheelSelection()
{
	int fitnessSum = 0;
	for (Genome& genome : genomes) fitnessSum += genome.fitness;
	int randomNumber = cocos2d::RandomHelper::random_int(0, fitnessSum);
	int genomeId = 0;
	int partialFitnessSum = 0;

	for (int i = 0; i < genomes.size(); ++i)
	{
		partialFitnessSum += genomes[i].fitness;
		if (partialFitnessSum > randomNumber)
		{
			genomeId = i;
			break;
		}
	}

	return genomes[genomeId];
}

Genome & GeneticAlgorithm::tournamentSelection()
{
	double bestFitnessSoFar = 0.0f;
	int chosenOneIdx = 0;

	for (int i = 0; i < tournamentCompetitors; ++i)
	{
		int thisTryIdx = cocos2d::RandomHelper::random_int((unsigned int)0, genomes.size() - 1);

		if (genomes[thisTryIdx].fitness > bestFitnessSoFar)
		{
			chosenOneIdx = thisTryIdx;
			bestFitnessSoFar = genomes[thisTryIdx].fitness;
		}
	}

	return genomes[chosenOneIdx];
}

void GeneticAlgorithm::crossover(Genome& mother, Genome& father, Genome& baby1, Genome& baby2)
{
	// Return parents as offspring if parents are the same
	if (cocos2d::RandomHelper::random_real(0.0f, 1.0f) > crossoverRate || &mother == &father)
	{
		baby1 = mother;
		baby2 = father;
		return;
	}
	unsigned int splitPointIndex1 = cocos2d::RandomHelper::random_int((unsigned int)0, networkSplitPoints.size() - 2);
	unsigned int splitPointIndex2 = cocos2d::RandomHelper::random_int((unsigned int)splitPointIndex1, networkSplitPoints.size() - 1);

	unsigned int crossoverPoint1 = networkSplitPoints[splitPointIndex1];
	unsigned int crossoverPoint2 = networkSplitPoints[splitPointIndex2];

	for (int i = 0; i < mother.weights.size(); ++i)
	{
		if ((i < crossoverPoint1) || (i >= crossoverPoint2))	// Outside of crossover points
		{
			// Keep the same genes
			baby1.weights.push_back(mother.weights[i]);
			baby2.weights.push_back(father.weights[i]);
		}
		else
		{
			// Switch the inside of genome (block between crossover points)
			baby1.weights.push_back(father.weights[i]);
			baby2.weights.push_back(mother.weights[i]);
		}
	}
	return;
}

void GeneticAlgorithm::mutate(Genome& genome)
{
	for (int i = 0; i < genome.weights.size(); ++i)
	{
		if (cocos2d::RandomHelper::random_real(0.0f, 1.0f) < mutationRate)
		{
			genome.weights[i] += (cocos2d::RandomHelper::random_real(-1.0f, 1.0f) * maxPerturbationRate);
			genome.weights[i] = Utils::clamp(genome.weights[i], -1.0f, 1.0f);
		}
	}
}


void GeneticAlgorithm::pause()
{
	game.pause();
	setRunning(false);
}

void GeneticAlgorithm::unpause()
{
	game.unpause();
	setRunning(true);
}

void GeneticAlgorithm::calculateBestWorstAverage()
{
	float fitnessSum = 0.0f;
	bestFitness = isi::Options::getInstance().searchingTrainInitialFitness;
	worstFitness = (double) LONG_MAX;
	for (int i = 0; i < genomes.size(); ++i)
	{
		fitnessSum += genomes[i].fitness;
		if (genomes[i].fitness > bestFitness)
		{
			bestFitness = genomes[i].fitness;
		}
		if (genomes[i].fitness < worstFitness)
		{
			worstFitness = genomes[i].fitness;
		}
	}
	averageFitness = fitnessSum / (float)genomes.size();
}

void GeneticAlgorithm::saveGenerationToFile()
{
	std::string filePath = ""; // cocos2d::FileUtils::sharedFileUtils()->getWritablePath();
	if (trainedNetwork == isi::Options::SEARCHING_NEURAL_NETWORK) filePath += "searching/";
	else if (trainedNetwork == isi::Options::FIGHTING_NEURAL_NETWORK) filePath += "fighting/";
	std::string fittestGenomesFilePath = "average_fitness.txt";
	filePath += std::to_string(generationNumber) + ".txt";

	CCLOG("PATH: %s", filePath.c_str());
	std::ofstream fittestGenomesFile(fittestGenomesFilePath, std::ofstream::app);
	fittestGenomesFile << averageFitness << "\n";
	fittestGenomesFile.close();
	std::ofstream file(filePath);

	for (Genome & genome : newGeneration)
	{
		for (double weight : genome.weights)
		{
			file << weight << " ";
		}
		file << "\n";
	}
	file.close();
	CCLOG("Finished saving");
}
