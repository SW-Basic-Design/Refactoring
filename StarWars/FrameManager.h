#pragma once

#include "Frame.h"
#include "Object.h"
#include "PlayerCharacter.h"

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
	void drawStatus(Character* player1, Character* player2);
private:
	Frame frame;
};

#endif
