#pragma once

#include "Character.hpp"

#ifndef PLAYER_CHARACTER_HPP
#define PLAYER_CHARACTER_HPP

class PlayerCharacter : public Character {
public:
	PlayerCharacter();
	~PlayerCharacter() = default;
	void setJumpTimer(const int time);
	int getJumpTimer();
	int getJumpLimit();
	bool is_mid_air;
private:
	int jump_timer;
};

#endif