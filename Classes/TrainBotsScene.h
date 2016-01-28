#pragma once

#include "cocos2d.h"
#include "Game.h"
#include "GeneticAlgorithm.h"

class TrainBotsScene : public cocos2d::Layer
{
private:
	// General
	bool paused = false;

	// Genetic algorithm
	GeneticAlgorithm geneticAlgorithm;

	int activeCharacterIndex = 1;	// Point to first enemy (0 == bot-player)

	Character* povCharacter;
	// GUI
	cocos2d::Menu* navigationPanel;

	cocos2d::Label* infoLabel;

	// Listeners
	cocos2d::EventListenerKeyboard* keyboardListener;

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	void update(float delta);

	void switchToCharacter(int indexDifference);

	void updateCameraPosition();

	void updateHUD();

    // implement the "static create()" method manually
    CREATE_FUNC(TrainBotsScene);
};