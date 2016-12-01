#pragma once

#include "cocos2d.h"

class TrainingOptionsScene : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap * tileMap;
	cocos2d::TMXLayer * background;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TrainingOptionsScene);
};