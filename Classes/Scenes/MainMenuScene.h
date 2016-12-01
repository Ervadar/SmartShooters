#pragma once

#include "cocos2d.h"

class MainMenuScene : public cocos2d::Layer
{
private:

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
	void playGameCallback(cocos2d::Ref* pSender);
	void trainBotsCallback(cocos2d::Ref* pSender);
	void optionsCallback(cocos2d::Ref* pSender);
    void quitCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};
