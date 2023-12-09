#pragma once

#include "Game.hpp"
#include "FrameManager.hpp"

#include "PlayerCharacter.hpp"
#include "Particle.hpp"
#include "Wall.hpp"
#include "Vec2.hpp"

#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

class Game;

class GameManager {
public:
	GameManager();
	void StartGame();
	bool PrecedeGame();
	void GetPlayerKeyInput();
	void resetStage();
	void removeAllComponents();
	void gotoNextStage();
	void makeNormalStage();
	void makeBossStage();
	void countDown();
	long long int last_item_SpawnTime;
	const static int spawn_term = 15000;

private:
	FrameManager frameManager;
	Game* game;
	void showStageOverScene();
};

#endif