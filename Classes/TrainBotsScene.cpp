#include "TrainBotsScene.h"

USING_NS_CC;

Scene* TrainBotsScene::createScene()
{
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));
    auto layer =  TrainBotsScene::create();
	layer->setName("train_bots_scene");
    scene->addChild(layer);
    return scene;
}

bool TrainBotsScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	geneticAlgorithm.init();

	// Debug labels
	infoLabel = Label::createWithTTF(" ", "fonts/slkscr.ttf", 14);
	addChild(infoLabel, 2);
	
	// Switch to first game (first enemy view)
	isi::Game* game = geneticAlgorithm.getGame();
	addChild(game, 1);
	povCharacter = game->getActiveEnemies()[0];

	// Create navigation panel
	Vector<MenuItem*> menuItems;
	auto label1 = Label::createWithTTF("Previous", "fonts/slkscr.ttf", 24);
	auto button1 = MenuItemLabel::create(label1, CC_CALLBACK_0(TrainBotsScene::switchToCharacter, this, -1));
	menuItems.pushBack(button1);
	auto label2 = Label::createWithTTF("Next", "fonts/slkscr.ttf", 24);
	auto button2 = MenuItemLabel::create(label2, CC_CALLBACK_0(TrainBotsScene::switchToCharacter, this, 1));
	menuItems.pushBack(button2);

	navigationPanel = Menu::createWithArray(menuItems);
	navigationPanel->alignItemsHorizontallyWithPadding(15);
	addChild(navigationPanel, 2);

	// Listeners
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(TrainBotsScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	scheduleUpdate();
    
    return true;
}

void TrainBotsScene::update(float delta)
{
	if (paused) return;
	geneticAlgorithm.update(delta);
	updateCameraPosition();
	updateHUD();
}

void TrainBotsScene::switchToCharacter(int indexDifference)
{
	int characterIndex = activeCharacterIndex + indexDifference;
	auto* game = geneticAlgorithm.getGame();
	auto& enemies = game->getActiveEnemies();
	Character* newPovCharacter;
	if (characterIndex < 0) characterIndex = enemies.size();
	else characterIndex = characterIndex % (enemies.size() + 1);

	if (characterIndex == 0)		// player character
		newPovCharacter = &(game->getPlayer());
	else
		newPovCharacter = enemies[characterIndex - 1];
	activeCharacterIndex = characterIndex;
	povCharacter = newPovCharacter;
}

void TrainBotsScene::updateCameraPosition()
{
	Camera* camera = getScene()->getDefaultCamera();
	camera->setPosition(povCharacter->getPosition());
	//updateHUD();
}

void TrainBotsScene::updateHUD()
{
	Vec2 viewPoint = getScene()->getDefaultCamera()->getPosition();
	Vec2 winSize = Director::getInstance()->getWinSize();
	Vec2 navPanelSize = navigationPanel->getContentSize();
	navigationPanel->setPosition(Vec2(viewPoint.x - winSize.x / 2 + navPanelSize.x / 2, viewPoint.y + winSize.y / 2 - 50));

	std::string infoString = "info";
	//if (activeCharacterIndex != 0)	// non-player
	//{
	//	Bot* activeBot = geneticAlgorithm.getGame()->getEnemies()[activeCharacterIndex - 1];
	//	for (double weight : activeBot->getActiveNeuralNetwork()->getNeuralNetworkWeights())
	//	{
	//		infoString += std::to_string(weight) + "\n";
	//	}
	//}
	infoLabel->setString(infoString);
	infoLabel->setPosition(Vec2(viewPoint.x - winSize.x / 2 + infoLabel->getContentSize().width / 2,
								viewPoint.y + winSize.y / 2 - infoLabel->getContentSize().height / 2));
}

void TrainBotsScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_F1)
	{
		getScene()->getPhysicsWorld()->setDebugDrawMask(
			!getScene()->getPhysicsWorld()->getDebugDrawMask()
			);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_F2)
	{
		auto& options = isi::Options::getInstance();
		options.debugInfo = !options.debugInfo;
	}
}