#include "Wall.hpp"

Wall::Wall() :Object() 
{
	this->object_type = ObjectType::WALL;

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

void Wall::setHealth(int health)
{
	this->health = health;
}