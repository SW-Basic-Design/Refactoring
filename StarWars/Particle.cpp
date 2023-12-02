#include "Particle.h"

Particle::Particle() :Object() 
{
	this->object_type = ObjectType::PARTICLE;
	this->size = { 1, 1 };

	this->damage = 1;
	this->shooter = NULL;
	this->max_range = 1;
	this->cur_range = 0;
	this->isMelee = false;
	this->isShotgun = false;
	this->isBombing = false;
}

void Particle::setDamage(const int damage) 
{
	this->damage = damage;
}

int Particle::getDamage() 
{
	return damage;
}