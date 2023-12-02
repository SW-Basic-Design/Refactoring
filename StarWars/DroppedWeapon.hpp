#pragma once

#include "DroppedItem.hpp"
#include "Character.hpp"

#ifndef DROPPED_WEAPON_HPP
#define DROPPED_WEAPON_HPP

class DroppedWeapon : public DroppedItem {
public:
	DroppedWeapon();
	DroppedWeapon(int w_id);
	~DroppedWeapon() = default;
	static int getDamage(int weapon_id);
	static int getSpeed(int weapon_id);
	static int getBulletCount(int weapon_id);
	static int getRPM(int weapon_id);
	static int getMaxRange(int weapon_id);
	static bool isMelee(int weapon_id);
	static bool isShotgun(int weapon_id);
	static bool isHatoken(int weapon_id);
	void useItem(Object* itemuser, Object* elseuser, vector< Object*>& objects);
private:
	int weapon_id;
	int damage;
	int speed;
	int RPM;
	int max_range;
	const int TOTAL_WEAPON = 7;// 0 : ¡÷∏‘, 1 : ±«√—, 2 : µπ∞›º“√—
};

#endif