#pragma once

#include "Object.h"

#ifndef WALL_HPP
#define WALL_HPP

class Wall : public Object {
public:
	Wall();
	~Wall() = default;
	void giveDamage(const int damage);
	int getHealth();
private :
	int health;
};

#endif
