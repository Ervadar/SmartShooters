#include "GameScene.h"
#include "MainMenuScene.h"
#include "Game/Game.h"
#include "Game/Bullet.h"
#include "Options.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0,0));
    auto layer = GameScene::create();
	layer->setName("game_scene");
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	// Init the game instance
	game.init(false);
	addChild(&game, 1);

	// Debug labels
	label = Label::createWithTTF(" ", "fonts/slkscr.ttf", 24);
	addChild(label, 2);
	
	// Listeners
	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// Menu
	Vector<MenuItem*> menuItems;
	auto label1 = Label::createWithTTF("Resume", "fonts/slkscr.ttf", 24);
	auto menuItem1 = MenuItemLabel::create(label1, CC_CALLBACK_1(GameScene::menuResumeCallback, this));
	menuItems.pushBack(menuItem1);

	menuItems.pushBack(MenuItemLabel::create(Label::createWithTTF("Restart", "fonts/slkscr.ttf", 24), CC_CALLBACK_1(GameScene::menuRestartCallback, this)));

	auto label2 = Label::createWithTTF("Quit", "fonts/slkscr.ttf", 24);
	auto menuItem2 = MenuItemLabel::create(label2, CC_CALLBACK_1(GameScene::menuQuitGameCallback, this));
	menuItems.pushBack(menuItem2);
	menu = Menu::createWithArray(menuItems);
	menu->alignItemsVertically();
	menu->setEnabled(false);
	this->addChild(menu, -1);
	
	// Hiding cursor
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	glview->setCursorVisible(false);

	// Crosshairs
	crosshairs = Sprite::create("sprites/crosshairs.png");
	crosshairs->setAnchorPoint(Vec2(0.5f, 0.5f));
	addChild(crosshairs, 10);

	// Info label
	infoLabel = Label::createWithTTF(" ", "fonts/slkscr.ttf", 14);
	addChild(infoLabel, 2);

	// Music
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	
	scheduleUpdate();

    return true;
}

void GameScene::update(float delta)
{
	checkHeldKeys();

	updateCameraPosition();

	updateHUD();

	// Game end condition
	if (game.getActiveEnemies().size() == 0 || !game.getPlayer().isAlive())
	{
		game.restart();
	}
}

void GameScene::checkHeldKeys()
{
	auto & player = game.getPlayer();
	if (leftKeyHeld) player.move(Vec2(-1, 0));
	if (rightKeyHeld) player.move(Vec2(1, 0));
	if (upKeyHeld) player.move(Vec2(0, 1));
	if (downKeyHeld) player.move(Vec2(0, -1));
}

void GameScene::updateCameraPosition()
{
	Camera* camera = getScene()->getDefaultCamera();
	camera->setPosition(game.getPlayer().getSprite()->getPosition());
}

void GameScene::updateHUD()
{
	// Debug labels
	Vec2 viewPoint = getScene()->getDefaultCamera()->getPosition();
	Vec2 winSize = Director::getInstance()->getWinSize();
	Vec2 position = game.getPlayer().getSprite()->getPosition();
	Vec2 tileCoords = game.tileCoordsFromPosition(position);
	std::string infoString = "F1 - show collision shapes\nF2 - draw debug info\nWASD to move\nLeft-click to shoot\n";
	infoLabel->setString(infoString);
	infoLabel->setPosition(Vec2(viewPoint.x - winSize.x / 2 + infoLabel->getContentSize().width / 2,
								viewPoint.y + winSize.y / 2 - infoLabel->getContentSize().height / 2));
	crosshairs->setPosition(Vec2(viewPoint.x - winSize.x / 2, viewPoint.y - winSize.y / 2) + crosshairsWindowPosition);
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Vec2 viewPoint = getScene()->getDefaultCamera()->getPosition();
		menu->setPosition(Vec2(viewPoint.x, viewPoint.y));
		menu->setLocalZOrder(1);
		menu->setEnabled(true);
		game.pause();
	}

	if (game.isPaused()) return;

	auto playerPhysicsBody = game.getPlayer().getSprite()->getPhysicsBody();
	auto playerSpeed = game.getPlayer().getSpeed();
	
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
		rightKeyHeld = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
		leftKeyHeld = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
		upKeyHeld = true;
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
		downKeyHeld = true;
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

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_D)
		rightKeyHeld = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_A)
		leftKeyHeld = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
		upKeyHeld = false;
	if (keyCode == EventKeyboard::KeyCode::KEY_S)
		downKeyHeld = false;
}

void GameScene::onMouseDown(cocos2d::Event * event)
{
	if (game.isPaused()) return;
	game.getPlayer().shoot();
}

void GameScene::onMouseMove(Event * event)
{
	EventMouse * em = (EventMouse*)event;
	Vec2 windowCenter = Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2);
	Vec2 cursorPosition = Vec2(em->getCursorX(), em->getCursorY());
	Vec2 playerDirection = cursorPosition - windowCenter;
	crosshairsWindowPosition = cursorPosition;

	if (game.isPaused()) return;

	playerDirection.normalize();
	Vec2 upVector = Vec2(0, 1);
	float angleInRadians = atan2f(playerDirection.y, playerDirection.x) - atan2f(upVector.y, upVector.x);
	float angleInDegrees = MATH_RAD_TO_DEG(angleInRadians);
	game.getPlayer().getSprite()->setRotation(-angleInDegrees);
}

void GameScene::menuResumeCallback(Ref* pSender)
{
	menu->setLocalZOrder(-1);
	menu->setEnabled(false);
	game.unpause();
}

void GameScene::menuRestartCallback(Ref * pSender)
{
	menu->setLocalZOrder(-1);
	menu->setEnabled(false);
	game.restart();
	game.unpause();
}

void GameScene::menuQuitGameCallback(Ref* pSender)
{
	auto director = Director::getInstance();
	director->getOpenGLView()->setCursorVisible(true);
	director->replaceScene(MainMenuScene::createScene());
}
