#include "TrainingOptionsScene.h"

USING_NS_CC;

Scene* TrainingOptionsScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TrainingOptionsScene::create();
    scene->addChild(layer);
    return scene;
}

bool TrainingOptionsScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	std::string mapFileName = "maps/map.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(mapFileName.c_str()).c_str());
	tileMap = TMXTiledMap::createWithXML(str->getCString(), "maps/");
	background = tileMap->getLayer("Background");

	addChild(tileMap, 1);

   // Size visibleSize = Director::getInstance()->getVisibleSize();
   // Vec2 origin = Director::getInstance()->getVisibleOrigin();
   //
   // auto closeItem = MenuItemImage::create(
   //                                        "CloseNormal.png",
   //                                        "CloseSelected.png",
   //                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
   // 
	//closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
   //                             origin.y + closeItem->getContentSize().height/2));
   //
   // auto menu = Menu::create(closeItem, NULL);
   // menu->setPosition(Vec2::ZERO);
   // this->addChild(menu, 1);
   //
   // auto label = Label::createWithTTF("Hi world! Kurwa robim prajekt! 12345!", "fonts/slkscr.ttf", 24);
   //
   // label->setPosition(Vec2(origin.x + visibleSize.width/2,
   //                         origin.y + visibleSize.height - label->getContentSize().height));
   //
   // this->addChild(label, 1);
    
    return true;
}


void TrainingOptionsScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
