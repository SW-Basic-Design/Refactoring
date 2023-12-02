#include "Wall.h"

Wall::Wall() :Object() 
{
	this->object_type = ObjectType::WALL;
	this->size = { 1, 1 };

	this->health = 50;
}

void Wall::giveDamage(const int damage)
{
	this->health -= damage;
}

int Wall::getHealth()
{
	return health;
}