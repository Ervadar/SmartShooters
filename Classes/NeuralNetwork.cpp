#include "NeuralNetwork.h"
#include "Options.h"
#include "cocos2d.h"

NeuralNetwork::NeuralNetwork(int hiddenLayerCount, int neuronsPerHiddenLayer,
							 int inputCount, int outputCount)
	: layerCount(hiddenLayerCount + 1), neuronsPerHiddenLayer(neuronsPerHiddenLayer),
	inputCount(inputCount), outputCount(outputCount), fitness(0)
{
	if (hiddenLayerCount > 0)
	{
		// First hidden layer
		neuronLayers.push_back(NeuronLayer(neuronsPerHiddenLayer, inputCount));

		// The rest of hidden layers
		for (int i = 0; i < hiddenLayerCount - 1; ++i)
		{
			neuronLayers.push_back(NeuronLayer(neuronsPerHiddenLayer, neuronsPerHiddenLayer));
		}
		// Output layer
		neuronLayers.push_back(NeuronLayer(outputCount, neuronsPerHiddenLayer));
	}
	else
	{
		// Output layer
		neuronLayers.push_back(NeuronLayer(outputCount, inputCount));
	}
}

std::vector<double> NeuralNetwork::getNetworkOutput(std::vector<double>& input) const
{
	std::vector<double> output;

	int weightIndex = 0;

	for (int i = 0; i < layerCount; ++i)
	{
		auto& layer = neuronLayers[i];
		if (i > 0) input = output;
		output.clear();
		weightIndex = 0;

		for (auto& neuron : layer.getNeurons())
		{
			double netInput = 0;
			int inputCount = neuron.getInputCount();
			std::vector<double>&  weights = neuron.getWeights();
			// For each weight
			for (int j = 0; j < inputCount - 1; ++j)
			{
				CCASSERT(j == weightIndex, "Something wrong or did I miss something?");
				CCASSERT(weightIndex < input.size(), "too high weight index");
				netInput += weights[j] * input[weightIndex++];
			}
			// Adding bias
			netInput += weights[inputCount - 1] * isi::Options::getInstance().neuronBias;
			
			output.push_back(sigmoid(netInput, isi::Options::getInstance().activationResponse));
			weightIndex = 0;
		}
	}
	return output;
}

const float NeuralNetwork::sigmoid(double input, double activationResponse) const
{
	return 0.5 * (input * activationResponse / (1 + abs(input * activationResponse))) + 0.5;
}

std::vector<double> NeuralNetwork::getNeuralNetworkWeights()
{
	std::vector<double> weights;
	for (NeuronLayer layer : neuronLayers)
		for (auto neuron : layer.getNeurons())
			for (double weight : neuron.getWeights())
				weights.push_back(weight);
	return weights;
}

std::vector<int> NeuralNetwork::calculateSplitPoints() const
{
	std::vector<int> splitPoints;

	int weightCounter = 0;

	for (NeuronLayer layer : neuronLayers)
	{
		for (auto neuron : layer.getNeurons())
		{
			for (double weight : neuron.getWeights())
			{
				++weightCounter;
			}
			splitPoints.push_back(weightCounter - 1);
		}
	}

	return splitPoints;
}

void NeuralNetwork::initWeights(std::vector<double> weights)
{
	int inputWeightIndex = 0;
	for (int layerIdx = 0; layerIdx < neuronLayers.size(); ++layerIdx)
	{
		for (int neuronIdx = 0; neuronIdx < neuronLayers[layerIdx].getNeurons().size(); ++neuronIdx)
		{
			for (int weightIdx = 0; weightIdx < neuronLayers[layerIdx].getNeurons()[neuronIdx].getWeights().size(); ++weightIdx)
			{
				neuronLayers[layerIdx].neurons[neuronIdx].weights[weightIdx] = weights[inputWeightIndex];
				++inputWeightIndex;
			}
		}
	}
	//for (NeuronLayer & layer : neuronLayers)
	//{
	//	for (Neuron & neuron : layer.getNeurons())
	//	{
	//		std::vector<double> neuronWeights = neuron.getWeights();
	//		for (int i = 0; i < neuronWeights.size(); ++i)
	//		{
	//			neuron.setWeight(i, weights[weightIndex]);
	//			++weightIndex;
	//		}
	//	}
	//}
}

void NeuralNetwork::initWeightsRandomly()
{
	int weightsCount = getNeuralNetworkWeights().size();
	std::vector<double> randomWeights;
	for (int i = 0; i < weightsCount; ++i)
	{
		randomWeights.push_back(cocos2d::RandomHelper::random_real(-1.0f, 1.0f));
	}
	initWeights(randomWeights);
}
