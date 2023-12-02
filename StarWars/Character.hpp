#pragma once

#include "Object.hpp"
#include "DroppedWeapon.hpp"

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

class Character : public Object {
public:
	Character();
	~Character() = default;
	void setHealth(const int h);
	void addHealth(const int h);
	void giveDamage(const int d);
	void setWeapon(const int w_id);
	int getWeapon();
	void setBuffTimer(const int t);
	int getBuffTimer();
	int getHealth();
	int getWeaponSpeed();
	int getWeaponDamage();
	int getWeaponRPM();
	int getWeaponMaxRange();
	bool isWeaponMelee();
	bool isWeaponShotgun();
	bool isWeaponHatoken();
	bool isFreeze = false;
	Vec2 direction;
	int bullet_count;
	long long last_shot;
	int current_buff;
	int buff_start;
	std::string getWeaponName();
	std::string getBuffName();
protected:
	int speed;
	int health;
	int weapon_id;
	int buff_timer;
};

#endif