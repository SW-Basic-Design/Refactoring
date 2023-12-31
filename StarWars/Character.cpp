#include "Character.h"


Character::Character() : Object() 
{
	this->object_type = ObjectType::CHARACTER;
	this->size = { 1, 2 };
	
	this->weapon_id = 0;
	this->isFreeze = false;
	this->health = 100;
	this->buff_timer = 0;
	this->speed = 1;
	this->direction = { 0, 0 };
	this->bullet_count = 0;
	this->last_shot = 0;
	this->current_buff = 0;
}

void Character::setWeapon(int weapon_id) 
{
	this->weapon_id = weapon_id;

	bullet_count = DroppedWeapon::getBulletCount(weapon_id);
}

void Character::setHealth(const int health) 
{
	this->health = health;
}

void Character::addHealth(const int health) 
{
	this->health += health;
}

void Character::giveDamage(const int damage) 
{
	this->health -= damage;
}

int Character::getWeapon()
{
	return weapon_id;
}

std::string Character::getWeaponName()
{
	switch (weapon_id)
	{
	case 1 :
		return "����";

	case 2 :
		return "���ݼ���";

	case 3:
		return "��";

	case 4:
		return "����";

	case 5:
		return "������";

	case 6:
		return "��ǳ";

	default :
		return "�ָ�";
	}
}

std::string Character::getBuffName()
{
	switch (this->current_buff)
	{
	case 1:
		return "�ӵ� x1.5";

	case 2:
		return "�����!";

	default:
		return "����";
	}
}

void Character::setBuffTimer(const int time) 
{
	buff_timer = time;
}

int Character::getBuffTimer() 
{
	return buff_timer;
}

int Character::getHealth()
{
	return health;
}

int Character::getWeaponSpeed()
{
	return DroppedWeapon::getSpeed(weapon_id);
}

int Character::getWeaponDamage()
{
	return DroppedWeapon::getDamage(weapon_id);
}

int Character::getWeaponRPM()
{
	return DroppedWeapon::getRPM(weapon_id);
}

int Character::getWeaponMaxRange()
{
	return DroppedWeapon::getMaxRange(weapon_id);
}

bool Character::isWeaponMelee()
{
	return DroppedWeapon::isMelee(weapon_id);
}

bool Character::isWeaponShotgun()
{
	return DroppedWeapon::isShotgun(weapon_id);
}

bool Character::isWeaponHatoken()
{
	return DroppedWeapon::isHatoken(weapon_id);
}