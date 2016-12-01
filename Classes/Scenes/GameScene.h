#pragma once

#include "cocos2d.h"
#include "Game/Game.h"

class GameScene : public cocos2d::Layer
{
private:
	// Game instance
	isi::Game game;

	cocos2d::Camera* camera;

	// Crosshairs
	cocos2d::Vec2 crosshairsWindowPosition;
	cocos2d::Sprite * crosshairs;

	// Debug labels
	cocos2d::Label * label;
	cocos2d::Label * infoLabel;

	// Menu
	cocos2d::Menu * menu;

	// Event dispatcher
	cocos2d::EventDispatcher eventDispatcher;

	// Listeners
	cocos2d::EventListenerKeyboard* keyboardListener;
	cocos2d::EventListenerMouse* mouseListener;

	bool upKeyHeld = false;
	bool downKeyHeld = false;
	bool leftKeyHeld = false;
	bool rightKeyHeld = false;
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onMouseDown(cocos2d::Event *event);
	void onMouseMove(cocos2d::Event *event);

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void update(float delta);

	void checkHeldKeys();

	void updateCameraPosition();

	void updateHUD();

    // a selector callback
	void menuResumeCallback(Ref* pSender);
	void menuRestartCallback(Ref* pSender);
    void menuQuitGameCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};