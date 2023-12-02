#pragma once

#include "Game.h"
#include "FrameManager.h"

#include "PlayerCharacter.h"
#include "Particle.h"
#include "Wall.h"
#include "DroppedSpecialItem.h"

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

class GameManager {
public:
	GameManager();
	void MakePlayer();
	void MakeItem();
	void MakeMap();
	void StartGame();
	bool PrecedeGame();
	void GetPlayerKeyInput();
	void PlayerShoot(PlayerCharacter*);
	Vec2 SetItemCoord();
	int last_item_SpawnTime;
	const static int spawn_term = 15000;
private:
	FrameManager frameManager;
	Game* game;
};

#endif