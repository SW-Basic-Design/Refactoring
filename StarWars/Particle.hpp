#pragma once

#include "Object.hpp"

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Particle : public Object {
public:
	Particle();
	~Particle() = default;
	void setDamage(const int d);
	int getDamage();
	Object* shooter;
	int max_range;
	int current_range;
	bool isMelee;
	bool isShotgun;
	bool isBombing;
	bool isHatoken;
private:
	int damage;//1~5 : Low, 6 ~ 10 Mid, 11 ~ High
};

#endif