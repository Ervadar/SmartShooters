# SmartShooters
A simple 2d shooter with intelligent bots. Under the hood there is actually an artificial neural network, which is trained by a genetic algorithm.

# How to run?
Just download the latest release and run the program.

# Introduction
In the game, there are 2 types of bots' neural networks:
* searching - network which is used to walk around the map and avoid obstacles,
* fighting - network which is used to shoot the player's character.

So far only the enviroment to train searching networks has been prepared. Thus, when a bot gets into fighting state, instead of switching its neural network to fighting one, it simply rotates into the player's direction and shoots him, until he gets out of sight.<br /><br />
After running the program the user has two options, either play the game or train bots.

# Playing the game
For the game to start properly, there must be a file named "searching_generation.txt" placed inside the executable directory. Inside this file there are neural network weights (one network - one line). You can transfer your own trained neural neural networks to this file.

# Training bots
During trainig, the program will save every evolved generation of neural networks to a distinct file in a specific directory (for example: "searching" for searching networks). Each file is named in the manner \<generation_number\>.txt

# Configuration file
For the program to run properly, there must exist a "config.ini" file in the executable directory. Below there is a quick description of the most important sections of variables in this file. <br />

### Game
* enemyCount - the number of enemy bots in game

### GeneticAlgorithm
* generationSize - size of each generation of the genetic algorithm
* trainedNetwork - the index of trained network (should be 1 for searching network)

### SearchingNetworkTraining
* searchingTrainBotCount - number of bots in one training experiment
* timeToSearch - length of one training experiment in seconds
* tournamentCompetitors - number of "picks" for tournament selection
* crossoverRate - probability of crossover happening
* mutationRate - probability of mutation happening
* maxPerturbationRate - maximum deviation of a single neuron weight during mutation
* eliteGenomes - number of the best genomes, which will be automatically passed to the next generation
* initialFitness - the initial fitness of bots

### NeuralNetworks
* hiddenLayerCount - number of hidden layers in neural networks (should be 1, but changing it to higher value shouldn't be an issue)
* neuronBias - neural network layer bias (-1 by default, do not change it)
* activationResponse - sigmoid activation function parameter
