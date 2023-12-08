#include "Object.hpp"

Object::Object() : Object(Vec2{ 0, 0 }, Vec2{ 0, 0 }, Vec2{ 0,0 }, 1) {}

Object::Object(Vec2 coord, Vec2 nextCoord, Vec2 velocity, int speed) : coord(coord), velocity(velocity), speed(speed)
{
	this->object_type = ObjectType::OBJECT;

	this->original_speed = speed;
	this->nextCoord = coord;

	this->target = nullptr;
	this->master = nullptr;
}

Vec2& Object::GetCoord()
{
	return coord;
}

Vec2& Object::GetVelocity()
{
	return velocity;
}

Vec2& Object::GetNextCoord()
{
	return nextCoord;
}

int Object::GetSpeed()
{
	return speed;
}

ObjectType Object::GetObjectType() 
{
	return object_type;
}

void Object::SetVelocity(const Vec2& velocity)
{
	this->velocity = velocity;
}

void Object::SetNextCoord(const Vec2& nextCoord)
{
	this->nextCoord = nextCoord;
}

void Object::SetCoord(const Vec2& coord)
{
	this->coord = coord;
}

void Object::SetSpeed(int speed) 
{
	this->speed = speed;
}

bool Object::IsPlayer()
{
	return object_type == ObjectType::PLAYER_CHARACTER;
}

bool Object::IsCharacter()
{
	return object_type == ObjectType::CHARACTER || object_type == ObjectType::NON_PLAYER_CHARACTER || object_type == ObjectType::ENEMY_NPC || object_type == ObjectType::FRIENDLY_NPC || object_type == ObjectType::PLAYER_CHARACTER;
}

bool Object::IsItem() 
{
	if (object_type == ObjectType::DROPPED_ITEM || object_type == ObjectType::DROPPED_SPECIAL_ITEM || object_type == ObjectType::DROPPED_WEAPON)
		return true;

	return false;
}

int Object::GetCollisionPriority() 
{
	switch (object_type)
	{
	case ObjectType::WALL:
		return 1;

	case ObjectType::CHARACTER:
		return 2;

	case ObjectType::DROPPED_ITEM:
	case ObjectType::DROPPED_SPECIAL_ITEM:
	case ObjectType::DROPPED_WEAPON:
		return 3;

	case ObjectType::PARTICLE:
		return 4;
	}

	return 0;
}

bool Object::IsCollisionWith(Object* obj)
{
	Vec2 current = GetCoord();
	Vec2 next = GetNextCoord();

	Vec2 obj_current = obj->GetCoord();
	Vec2 obj_next = obj->GetNextCoord();

	Vec2 size = this->size;
	Vec2 obj_size = obj->size;
	
	for (int i = -size.getY() / 2; i <= size.getY() / 2; ++i)
	{
		for (int j = -size.getX() / 2; j <= size.getX() / 2; ++j)
		{
			if (next + Vec2{ i, j } == obj_next || current + Vec2{ i, j } == obj_current || next + Vec2{ i, j } == obj_current || current + Vec2{ i, j } == obj_next)
				return true;
		}
	}

	for (int i = -obj_size.getY() / 2; i <= obj_size.getY() / 2; ++i)
	{
		for (int j = -obj_size.getX() / 2; j <= obj_size.getX() / 2; ++j)
		{
			if (next == obj_next + Vec2{ i, j } || current== obj_current + Vec2{ i, j } || next == obj_current + Vec2{ i, j } || current == obj_next + Vec2{ i, j })
				return true;
		}
	}
	
	return false;
}

Object* Object::getTarget()
{
	return target;
}

Object* Object::getMaster()
{
	return master;
}

int Object::getOriginalSpeed()
{
	return original_speed;
}

void Object::setOriginalSpeed(int original_speed)
{
	this->original_speed = original_speed;
}

void Object::setTarget(Object* target)
{
	this->target = target;
}

void Object::setMaster(Object* master)
{
	this->master = master;
}