#include "Object.hpp"

Object::Object() : Object(Vec2{ 0, 0 }, Vec2{ 0, 0 }, Vec2{ 0,0 }, 1) {}

Object::Object(Vec2 coord, Vec2 nextCoord, Vec2 velocity, int speed) : coord(coord), velocity(velocity), speed(speed)
{
	this->object_type = ObjectType::OBJECT;

	this->nextCoord = coord;
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

	
	if (next == obj_next || current == obj_current || next == obj_current || current == obj_next)
		return true;

	if (next == obj_next || current == obj_current || current == obj_next || next == obj_current)
		return true;
	
	return false;
}
