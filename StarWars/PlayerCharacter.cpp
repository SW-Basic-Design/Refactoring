#include "PlayerCharacter.hpp"

PlayerCharacter::PlayerCharacter() : Character() 
{
	this->object_type = ObjectType::PLAYER_CHARACTER;
	this->jump_timer = 0;
	this->is_mid_air = false;
}

void PlayerCharacter::setJumpTimer(const int time)
{
	this->jump_timer = time;
}

int PlayerCharacter::getJumpTimer()
{
	return jump_timer;
}

int PlayerCharacter::getJumpLimit()
{
	return 5;
}
