#include "DroppedWeapon.hpp"

DroppedWeapon::DroppedWeapon() : DroppedWeapon(0)
{
}

DroppedWeapon::DroppedWeapon(int weapon_id) : DroppedItem()
{
	this->object_type = ObjectType::DROPPED_WEAPON;

	this->weapon_id = weapon_id;
	this->damage = getDamage(weapon_id);
	this->speed = getSpeed(weapon_id);
	this->RPM = getRPM(weapon_id);
	this->max_range = getMaxRange(weapon_id);
}

int DroppedWeapon::getDamage(int weapon_id)
{
	switch (weapon_id)
	{
	case 1://pistol
		return 5;

	case 2://rifle
		return 8;

	case 3://sword
		return 15;

	case 4://shotgun
		return 5;

	case 5://sniper
		return 20;

	case 6://Hatoken
		return 10;

	case 99://Ai weapon
		return 3;

	default://fist
		return 3;
	}

	return 0;
}
int DroppedWeapon::getSpeed(int weapon_id)
{
	switch (weapon_id)
	{
	case 1://pistol
		return 25;

	case 2://rifle
		return 40;

	case 3://sword
		return 25;

	case 4://shotgun
		return 30;

	case 5://sniper
		return 1000;

	case 6:
		return 30;

	case 99://Ai Weapon
		return 50;

	default://fist
		return 25;
	}
}

int DroppedWeapon::getBulletCount(int weapon_id)
{
	switch (weapon_id)
	{
	case 1://pistol
		return 20;

	case 2://rifle
		return 60;

	case 3://sword
		return 10;

	case 4://shotgun
		return 15;

	case 5://sniper
		return 10;

	case 6://Hatoken
		return 10;

	case 99:
		return MAXINT;
	}

	return 0;
}

int DroppedWeapon::getRPM(int weapon_id)
{
	switch (weapon_id)
	{
	case 1://pistol
		return 5;
	
	case 2://rifle
		return 10;

	case 3: //sword
		return 5;

	case 4://shotgun
		return 2;

	case 5://sniper
		return 2;

	case 6://Hatoken 
		return 5;

	case 99://Ai Weapon
		return 2;

	default://fist
		return 5;
	}
}

int DroppedWeapon::getMaxRange(int weapon_id)
{
	switch (weapon_id)
	{
	case 1://pistol
		return 10;

	case 2://rifle
		return  20;

	case 3: // sword
		return 2;

	case 4://shotgun
		return 7;

	case 5://sniper
		return 40;

	case 6://Hatoken
		return 10;

	case 99:
		return 15;

	default://fist
		return 2;
	}
}

bool DroppedWeapon::isMelee(int weapon_id)
{
	switch (weapon_id)
	{
	case 0: 
	case 3:
		return true;

	default:
		return false;
	}
}

bool DroppedWeapon::isShotgun(int weapon_id)
{
	switch (weapon_id)
	{
	case 4:
		return true;
	
	default:
		return false;
	}
}

bool DroppedWeapon::isHatoken(int weapon_id)
{
	switch (weapon_id)
	{
	case 6:
		return true;

	default:
		return false;
	}
}

void DroppedWeapon::useItem(Object* itemuser, Object* elseuser, vector< Object*>& objects) {
	((Character*)itemuser)->setWeapon(weapon_id);
}

