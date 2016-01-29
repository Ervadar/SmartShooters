#include "GeneticAlgorithm.h"

void GeneticAlgorithm::init()
{
	// Create game instances
	auto& options = isi::Options::getInstance();

	game = new isi::Game();
	game->init(true);

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
			CCLOG("SIZE < GS, active enemeies %d", game->getActiveEnemies().size());
			for (Bot* bot : game->getActiveEnemies())
			{
				if (bot->getTimeBeingTrained() > timeToSearch || bot->isPlayerSeen())
				{
					genomes.push_back(bot->getGenomeFromNeuralNetwork());
					bot->deactivate();
				}
			}
			// Jesli liczba botow aktywnych == 0
				// Zrespawnuj na nowo gracza w losowym miejscu i kolejn¹ partiê botów
		}
		else
		{
			// Dokopoki nie stworzono nowej generacji
				// Wybieramy 2 genomy z poprzedniej generacji przez Roullette Wheel Selection
				// Krzy¿ówka (CROSSOVER RATE)
				// Mutacja (MUTATION RATE)
			// Podmien generacje w algorytmie (moze zapisz poprzednia w pliku)
			// Continue
		}
	}

}