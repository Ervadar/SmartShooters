#include "GeneticAlgorithm.h"

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
	timeToSearch = options.timeToSearch;

	setRunning(true);

	// Create first genome population from enemies' neural network

}

void GeneticAlgorithm::update(float delta)
{
	static float timePassed = 0.0f;
	timePassed += delta;

	if (isRunning())
	{
		if (genomes.size() < generationSize)
		{
			if (game.getActiveEnemies().size() == 0)
			{
				game.restart();
			}

			for (Bot* bot : game.getActiveEnemies())
			{
				CCLOG("Bot time trained: %f | Time to search: %f", bot->getTimeBeingTrained(), (float)timeToSearch);
				if (bot->getTimeBeingTrained() > timeToSearch || bot->isPlayerSeen())
				{
					genomes.push_back(bot->getGenomeFromNeuralNetwork());
					bot->deactivate();
				}
			}
				
		}
		else
		{
			std::vector<Genome> newGeneration;
			while (newGeneration.size() < generationSize)
			{
				newGeneration.push_back(Genome());
			}
			genomes.clear();
			// Dokopoki nie stworzono nowej generacji
				// Wybieramy 2 genomy z poprzedniej generacji przez Roullette Wheel Selection
				// Krzy¿ówka (CROSSOVER RATE)
				// Mutacja (MUTATION RATE)
			// Podmien generacje w algorytmie (moze zapisz poprzednia w pliku)
			// Continue
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
