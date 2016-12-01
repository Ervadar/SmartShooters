#include "TrainBotsScene.h"
#include "MainMenuScene.h"

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
	povCharacter = game->getActiveEnemies().at(0);

	// Create navigation panel
	Vector<MenuItem*> navItems;
	navItems.pushBack(MenuItemLabel::create(Label::createWithTTF("Previous", "fonts/slkscr.ttf", 24),
											CC_CALLBACK_0(TrainBotsScene::switchToCharacter, this, -1) ));
	navItems.pushBack(MenuItemLabel::create(Label::createWithTTF("Next", "fonts/slkscr.ttf", 24),
											CC_CALLBACK_0(TrainBotsScene::switchToCharacter, this, 1) ));
	navigationPanel = Menu::createWithArray(navItems);
	navigationPanel->alignItemsHorizontallyWithPadding(15);
	addChild(navigationPanel, 2);

	// Listeners
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(TrainBotsScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(TrainBotsScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	// Menu
	Vector<MenuItem*> menuItems;
	menuItems.pushBack(MenuItemLabel::create(Label::createWithTTF("Resume", "fonts/slkscr.ttf", 24),
											 CC_CALLBACK_1(TrainBotsScene::menuResumeCallback, this) ));
	menuItems.pushBack(MenuItemLabel::create(Label::createWithTTF("Save last generation", "fonts/slkscr.ttf", 24),
											 CC_CALLBACK_1(TrainBotsScene::saveLastGeneration, this) ));
	menuItems.pushBack(MenuItemLabel::create(Label::createWithTTF("Quit", "fonts/slkscr.ttf", 24),
											 CC_CALLBACK_1(TrainBotsScene::menuQuitGameCallback, this) ));
	menu = Menu::createWithArray(menuItems);
	menu->alignItemsVertically();
	menu->setEnabled(false);
	this->addChild(menu, -1);

	scheduleUpdate();
    
    return true;
}

void TrainBotsScene::update(float delta)
{
	geneticAlgorithm.update(delta);
	updateCameraPosition();
	updateHUD(delta);
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
		newPovCharacter = enemies.at(characterIndex - 1);
	activeCharacterIndex = characterIndex;
	povCharacter = newPovCharacter;
}

void TrainBotsScene::updateCameraPosition()
{
	Camera* camera = getScene()->getDefaultCamera();
	camera->setPosition(povCharacter->getPosition());
	//updateHUD();
}

void TrainBotsScene::updateHUD(float delta)
{
	Vec2 viewPoint = getScene()->getDefaultCamera()->getPosition();
	Vec2 winSize = Director::getInstance()->getWinSize();
	Vec2 navPanelSize = navigationPanel->getContentSize();
	navigationPanel->setPosition(Vec2(viewPoint.x - winSize.x / 2 + navPanelSize.x / 2, viewPoint.y + winSize.y / 2 - 50));

	std::string infoString = "F1 - show collision shapes\nF2 - draw debug info\n\n";
	infoString += "Generation: " + std::to_string(geneticAlgorithm.getGenerationNumber()) + "\n" +
		"Previous best fitness: " + std::to_string(geneticAlgorithm.getBestFitness()) + "\n"
		"Experiments done: " + std::to_string(geneticAlgorithm.getGenomeCount()) + "/" + std::to_string(geneticAlgorithm.getGenerationSize()) + "\n";
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

void TrainBotsScene::menuResumeCallback(Ref * pSender)
{
	menu->setLocalZOrder(-1);
	menu->setEnabled(false);
	geneticAlgorithm.unpause();
}

void TrainBotsScene::saveLastGeneration(Ref * pSender)
{
	geneticAlgorithm.saveGenerationToFile();
}

void TrainBotsScene::menuQuitGameCallback(cocos2d::Ref * pSender)
{
	auto director = Director::getInstance();
	removeChild(geneticAlgorithm.getGame());
	director->getOpenGLView()->setCursorVisible(true);
	director->replaceScene(MainMenuScene::createScene());
}

void TrainBotsScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Vec2 viewPoint = getScene()->getDefaultCamera()->getPosition();
		menu->setPosition(Vec2(viewPoint.x, viewPoint.y));
		menu->setLocalZOrder(1);
		menu->setEnabled(true);
		geneticAlgorithm.pause();
	}

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