#pragma once

#include "includes.h"
#include "Object.h"
#include "Character.h"
#include "Particle.h"
#include "DroppedItem.h"
#include "PlayerCharacter.h"
#include "Wall.h"

#ifndef GAME_HPP
#define GAME_HPP

class Game {
public:
	Game(bool gameOver);
	bool IsGameOver();
	void SetGameOver(bool);
	void UpdateObjectNextPosition();
	void UpdateObjectPosition();
	void UpdateObjects();
	bool isOutOfMap(Object * obj);
	std::vector<Object*>& GetObjects();
	const int WIDTH = 41;
	const int HEIGHT = 20;
	const static int map[20][41];
	static Object* Curmap[20][41];
private:
	bool gameOver;
	std::vector<Object*> objects;
};

#endif