#include "Particle.hpp"

Particle::Particle() :Object() 
{
	this->object_type = ObjectType::PARTICLE;

	this->damage = 1;
	this->shooter = NULL;
	this->max_range = 1;
	this->current_range = 0;
	this->isMelee = false;
	this->isShotgun = false;
	this->isLaser = false;
	this->isHatoken = false;
}

void Particle::setDamage(const int damage) 
{
	this->damage = damage;
}

int Particle::getDamage() 
{
	return damage;
}