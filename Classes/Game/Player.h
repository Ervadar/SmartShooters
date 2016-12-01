#pragma once
#include "Character.h"
#include "Game.h"

namespace isi { class Game; }

/*
Class representing the player
*/
class Player : public Character
{
private:
	isi::Game& game;

	// Training only
	cocos2d::Vec2 spawnPoint;
	float ellipseWidth;
	float ellipseHeight;

public:
	Player(isi::Game& game);
	bool init(std::string spritePath, int hp, BulletPool & bulletPool, cocos2d::Vec2 position);
	virtual void initState(cocos2d::Vec2 position, int hp);
	virtual void update(float delta);

	void shoot();
};