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
		int generationSize;
		int genomesPerInstance;
		int hiddenLayerCount;
		double neuronBias;
		double activationResponse;
		bool loadNeuralNetworks;
		bool debugInfo;
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