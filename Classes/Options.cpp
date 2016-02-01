#include "Options.h"

void isi::Options::loadOptions()
{
	try
	{
		boost::property_tree::read_ini("config.ini", pt);
	}
	catch (std::exception &ex)
	{
		CCLOG("%s\n", ex.what());
	}
	/* Game */
	enemyCount = pt.get<int>("Game.enemyCount");

	/* General */
	loadNeuralNetworks = pt.get<bool>("General.loadNeuralNetworks");
	debugInfo = pt.get<bool>("General.debugInfo");

	/* Genetic Algorithm */
	generationSize = pt.get<int>("GeneticAlgorithm.generationSize");
	trainedNetwork = pt.get<int>("GeneticAlgorithm.trainedNetwork");

	/* SearchingNetworkTraining */
	searchingTrainBotCount = pt.get<int>("SearchingNetworkTraining.searchingTrainBotCount");
	timeToSearch = pt.get<int>("SearchingNetworkTraining.timeToSearch");
	searchingTrainTournamentCompetitors = pt.get<int>("SearchingNetworkTraining.tournamentCompetitors");
	searchingTrainCrossoverRate = pt.get<float>("SearchingNetworkTraining.crossoverRate");
	searchingTrainMutationRate = pt.get<float>("SearchingNetworkTraining.mutationRate");
	searchingTrainMaxPerturbationRate = pt.get<float>("SearchingNetworkTraining.maxPerturbationRate");
	searchingTrainInitialFitness = pt.get<double>("SearchingNetworkTraining.initialFitness");

	/* Neural Networks*/
	hiddenLayerCount = pt.get<int>("NeuralNetworks.hiddenLayerCount");
	neuronBias = pt.get<double>("NeuralNetworks.neuronBias");
	activationResponse = pt.get<double>("NeuralNetworks.activationResponse");

}
