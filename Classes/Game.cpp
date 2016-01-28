#include "Game.h"
#include "Options.h"

USING_NS_CC;

isi::Game::Game() : player(new Player(*this))
{

}

isi::Game::~Game()
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
		delete *it;

	delete player;
}

bool isi::Game::init(bool trainingGame)
{
	// Tile map
	std::string mapFileName = "maps/map.tmx";
	auto str = String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(mapFileName.c_str()).c_str());
	tileMap = TMXTiledMap::createWithXML(str->getCString(), "maps/");
	walls = tileMap->getLayer("Walls");
	tileMap->setPosition((int) tileMap->getPosition().x, (int) tileMap->getPosition().y);
	addChild(tileMap, 0);

	// Set training game mode
	this->trainingGame = trainingGame;

	// Tile collision bodies - WALLS
	for (int x = 0; x < tileMap->getMapSize().width; ++x)
	{
		for (int y = 0; y < tileMap->getMapSize().height; ++y)
		{
			auto tileSprite = walls->getTileAt(Vec2(x, y));
			auto tileGid = walls->getTileGIDAt(Vec2(x, y));
			
			Value properties = tileMap->getPropertiesForGID(tileGid);
			if (!properties.isNull())
			{
				ValueMap dict = properties.asValueMap();
				Value collision = dict["collidable"];

				if (!collision.isNull() && collision.asInt() == 1)
				{
					auto physicsBody = cocos2d::PhysicsBody::createBox(Size(33, 33), cocos2d::PhysicsMaterial(0.0f, 0.0f, 0.0f));
					physicsBody->setDynamic(false);
					physicsBody->setCategoryBitmask(0x00000008);
					physicsBody->setContactTestBitmask(0x00000007);
					physicsBody->setTag(Entity::TAG_WALL);
					tileSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
					tileSprite->setPhysicsBody(physicsBody);
				}
			}
		}
	}

	// Bullets
	bulletPool.init();
	addChild(&bulletPool, 1);

	// Player
	TMXObjectGroup * playerSpawnPoints = tileMap->getObjectGroup("PlayerSpawnPoints");
	CCASSERT(NULL != playerSpawnPoints, "PlayerSpawnPoints group not found");
	int playerSpawnPointCount = playerSpawnPoints->getObjects().size();
	int playerSpawnPointIndex = cocos2d::RandomHelper::random_int(1, playerSpawnPointCount);
	auto playerSpawnPoint = playerSpawnPoints->getObject("playerSpawnPoint" + std::to_string(playerSpawnPointIndex));
	CCASSERT(!playerSpawnPoint.empty(), "playerSpawnPoint object not found");

	// setting position mmust be before init
	Vec2 position = Vec2(playerSpawnPoint["x"].asFloat(),
						 playerSpawnPoint["y"].asFloat() + tileMap->getTileSize().height / 2 + playerSpawnPoint["height"].asFloat());
	player->init("player.png", 200, bulletPool, position);
	addChild(player, 1);

	// Enemies
	TMXObjectGroup * enemySpawnPoints = tileMap->getObjectGroup("EnemySpawnPoints");
	CCASSERT(NULL != enemySpawnPoints, "EnemySpawnPoints group not found");
	int enemySpawnPointCount = enemySpawnPoints->getObjects().size();
	int enemyCount = isi::Options::getInstance().genomesPerInstance;
	for (int i = 0; i < enemyCount; ++i)
	{
		auto enemySpawnPoint = enemySpawnPoints->getObject("enemySpawnPoint" + std::to_string(RandomHelper::random_int(1, enemySpawnPointCount)));
		Bot * enemy = new Bot(*this);
		Vec2 position = Vec2(enemySpawnPoint["x"].asFloat(),
							 enemySpawnPoint["y"].asFloat() + tileMap->getTileSize().height / 2 + enemySpawnPoint["height"].asFloat());
		enemy->init("enemy.png", 75, bulletPool, position);
		enemies.push_back(enemy);
		addChild(enemy, 1);
	}

	// If the game is training game, set player waypoints
	if (trainingGame)
	{
		
	}

	// Contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();

    return true;
}

void isi::Game::update(float delta)
{
	for (Bot * enemy : enemies)
	{
		if (!enemy->isAlive()) continue;

		enemy->update(delta);
	}

	if (getAliveEnemiesCount() == 0 || !player->isAlive())
	{
		// Finish game
		//_running = false;
	}
}

bool isi::Game::onContactBegin(cocos2d::PhysicsContact & contact)
{
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	int bodyAtag = bodyA->getTag();
	int bodyBtag = bodyB->getTag();
	
	// Bullet collisions
	if (bodyAtag == Entity::TAG_BULLET)
	{
		if (bodyBtag == Entity::TAG_BULLET) return false;
		bulletCollisionWithBody((Bullet*)bodyA->getNode()->getParent(), bodyB);
		return false;
	}
	else if (bodyBtag == Entity::TAG_BULLET)
	{
		bulletCollisionWithBody((Bullet*)bodyB->getNode()->getParent(), bodyA);
		return false;
	}

	return true;
}

void isi::Game::bulletCollisionWithBody(Bullet * bullet, PhysicsBody * body)
{
	int bodyTag = body->getTag();
	bullet->onCollision();
	if (bodyTag == Entity::TAG_CHARACTER)
	{
		((Character*)body->getNode()->getParent())->decreaseHp(bullet->getPower());
	}
	else if (bodyTag == Entity::TAG_WALL) {}
}

int isi::Game::getAliveEnemiesCount()
{
	int aliveEnemiesCount = 0;
	for (Character * enemy : enemies)
		if (enemy->getHp() > 0) ++aliveEnemiesCount;
	return aliveEnemiesCount;
}

Vec2 isi::Game::tileCoordsFromPosition(Vec2 position)
{
	int x = position.x / tileMap->getTileSize().width;
	int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y) / tileMap->getTileSize().height;
	return Vec2(x, y);
}

void isi::Game::move(Character & character, cocos2d::Vec2 direction)
{
	auto sprite = character.getSprite();
	float x = sprite->getPosition().x + character.getSpeed() * direction.x;
	float y = sprite->getPosition().y + character.getSpeed() * direction.y;
	sprite->setPosition(x, y);
	character.updateHUDposition();
}

std::vector<cocos2d::PhysicsBody*> isi::Game::getAllPhysicsBodies()
{
	std::vector<cocos2d::PhysicsBody*> physicsBodies;
	physicsBodies.push_back(player->getSprite()->getPhysicsBody());
	for (auto* enemy : enemies)
		physicsBodies.push_back(enemy->getSprite()->getPhysicsBody());
	for (auto* bullet : getBulletPool()->getActiveBullets())
		physicsBodies.push_back(bullet->getSprite()->getPhysicsBody());
	return physicsBodies;
}
