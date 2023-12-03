#pragma once

#include "Frame.hpp"
#include "Object.hpp"
#include "PlayerCharacter.hpp"
#include "Particle.hpp"
#include <string>

#ifndef FRAME_MANAGER_HPP
#define FRAME_MANAGER_HPP

class FrameManager {
public:
	void InitFrame();
	void DestroyFrame();
	void ClearScreen();
	void SwapBuffer();
	void Print(const char*);
	void PrintWithPosition(const char*, COORD);
	void UpdateFrame();
	void ClearBuffer();
	void SetCursorPosition(COORD);
	COORD GetCursorPosition();
	void MakeFrame(std::vector<Object*>&);
	void MakeStageOverFrame(std::vector<Object*>& objects, Object * dead_player, int flag);
	void PrintStageOverMassage(int flag);
	void PrintCountDown(int flag);
	void drawStatus(Character* player1, Character* player2);
private:
	Frame frame;
	int i = 0;
	static int gameOverMassage[20][41];
	static int count[5][20][41];
};

#endif
