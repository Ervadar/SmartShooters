#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "cocos2d.h"

namespace isi
{
	using boost::property_tree::ptree;

	class Options
	{
	public:
		static const int SEARCHING_NEURAL_NETWORK = 1;
		static const int FIGHTING_NEURAL_NETWORK = 2;

		/* Game */
		int enemyCount;

		/* General */
		bool loadNeuralNetworks;
		bool debugInfo;

		/* Genetic Algorithm */
		int generationSize;
		int trainedNetwork;

		/* SearchingNetworkTraining */
		int searchingTrainBotCount;
		int timeToSearch;
		int searchingTrainTournamentCompetitors;
		float searchingTrainCrossoverRate;
		float searchingTrainMutationRate;
		float searchingTrainMaxPerturbationRate;
		double searchingTrainInitialFitness;

		/* Neural Networks*/
		int hiddenLayerCount;
		double neuronBias;
		double activationResponse;


	private:
		ptree pt;

	public:
		void loadOptions();

		static Options& getInstance()
		{
			static Options * instance = new Options();
			return *instance;
		}

	private:
		Options() {}
	};
}