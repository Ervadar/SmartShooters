#include "GeneticAlgorithm.h"

void GeneticAlgorithm::init()
{
	// Create game instances
	auto& options = isi::Options::getInstance();

	game = new isi::Game();
	game->init(true);

	// Load weights
	// TODO: ma by� tylko if (options.lnn) -> je�li true to prze�adowujemy losowe wagi (wczytane konstruktorami) podanymi
	if (options.loadNeuralNetworks)
		CCLOG("TODO: load weights from file\n");
	else
		loadRandomNeuralNetworks();
}

void GeneticAlgorithm::update(float delta)
{

}

void GeneticAlgorithm::loadRandomNeuralNetworks()
{
	
	//CCLOG("%d", game->getEnemies().size());
	//for (auto * enemy : game->getEnemies())
	//{
	//	enemy->initNeuralNetworksRandomly();
	//}
}