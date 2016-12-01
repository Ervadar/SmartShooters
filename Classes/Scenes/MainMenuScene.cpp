#include "MainMenuScene.h"
#include "GameScene.h"
#include "TrainBotsScene.h"
#include <SimpleAudioEngine.h>
#include "Options.h"


USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	isi::Options::getInstance().loadOptions();

   Size visibleSize = Director::getInstance()->getVisibleSize();
   Vec2 origin = Director::getInstance()->getVisibleOrigin();

   // Creating menu
   Vector<MenuItem*> menuItems;
   
   auto label1 = Label::createWithTTF("Play", "fonts/slkscr.ttf", 24);
   auto menuItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(MainMenuScene::playGameCallback, this));
   menuItems.pushBack(menuItem1);
   auto label2 = Label::createWithTTF("Train bots", "fonts/slkscr.ttf", 24);
   auto menuItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(MainMenuScene::trainBotsCallback, this));
   menuItems.pushBack(menuItem2);
   //auto label3 = Label::createWithTTF("Options", "fonts/slkscr.ttf", 24);
   //auto menuItem3 = MenuItemLabel::create(label3, CC_CALLBACK_1(MainMenuScene::optionsCallback, this));
   //menuItems.pushBack(menuItem3);
   auto label4 = Label::createWithTTF("Quit", "fonts/slkscr.ttf", 24);
   auto menuItem4 = MenuItemLabel::create(label4, CC_CALLBACK_1(MainMenuScene::quitCallback, this));
   menuItems.pushBack(menuItem4);

   auto menu = Menu::createWithArray(menuItems);
   menu->alignItemsVertically();
   this->addChild(menu, 1);

   // Music
   CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/menu.mp3", true);

   return true;

}

void MainMenuScene::playGameCallback(Ref* pSender)
{
	Scene* scene = GameScene::createScene();
	Director::getInstance()->replaceScene(scene);
	GameScene* gameScene = (GameScene*)scene->getChildByName("game_scene");
	gameScene->updateCameraPosition();
}

void MainMenuScene::trainBotsCallback(Ref* pSender)
{
	Scene* scene = TrainBotsScene::createScene();
	Director::getInstance()->replaceScene(scene);
	TrainBotsScene* trainBotsScene = (TrainBotsScene*)scene->getChildByName("train_bots_scene");
	trainBotsScene->updateCameraPosition();
}

void MainMenuScene::optionsCallback(Ref* pSender)
{
}

void MainMenuScene::quitCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
