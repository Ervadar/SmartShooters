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
	generationSize = pt.get<int>("GeneticAlgorithm.generationSize");
	CCLOG("%d", generationSize);
	genomesPerInstance = pt.get<int>("GeneticAlgorithm.genomesPerInstance");
	CCLOG("%d", genomesPerInstance);
	hiddenLayerCount = pt.get<int>("NeuralNetworks.hiddenLayerCount");
	CCLOG("%d", hiddenLayerCount);
	neuronBias = pt.get<double>("NeuralNetworks.neuronBias");
	CCLOG("%f", neuronBias);
	activationResponse = pt.get<double>("NeuralNetworks.activationResponse");
	CCLOG("%f", activationResponse);
	loadNeuralNetworks = pt.get<bool>("General.loadNeuralNetworks");
	CCLOG("%d", (int)loadNeuralNetworks);
	debugInfo = pt.get<bool>("General.debugInfo");
	CCLOG("%d", (int)loadNeuralNetworks);
}
