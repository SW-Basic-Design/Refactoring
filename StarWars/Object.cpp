#include "Object.h"

Object::Object() : Object({ 0, 0 }, { 0, 0 }, { 0,0 }, 1) {}

Object::Object(Vec2<double> coord, Vec2<double> nextCoord, Vec2<double> velocity, int speed) : coord(coord), velocity(velocity), speed(speed)
{
	this->object_type = ObjectType::OBJECT;
	this->size = { 1, 1 };

	this->nextCoord = coord;
}

Vec2<double>& Object::GetCoord()
{
	return coord;
}

Vec2<int> Object::GetCoordAsInt()
{
	Vec2<int> int_vector = coord;

	return int_vector;
}

Vec2<double>& Object::GetVelocity()
{
	return velocity;
}

Vec2<double>& Object::GetNextCoord()
{
	return nextCoord;
}

Vec2<int> Object::GetNextCoordAsInt()
{
	Vec2<int> int_vector = nextCoord;

	return int_vector;
}

int Object::GetSpeed()
{
	return speed;
}

ObjectType Object::GetObjectType() 
{
	return object_type;
}

void Object::SetVelocity(const Vec2<double>& velocity)
{
	this->velocity = velocity;
}

void Object::SetNextCoord(const Vec2<double>& nextCoord)
{
	this->nextCoord = nextCoord;
}

void Object::SetCoord(const Vec2<double>& coord)
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
	Vec2<int> object1_size = size;
	Vec2<int> object2_size = obj->size;

	Vec2<int> current = GetCoordAsInt();
	Vec2<int> next = GetCoordAsInt();

	Vec2<int> obj_current = obj->GetCoordAsInt();
	Vec2<int> obj_next = obj->GetNextCoordAsInt();

	for (int i = 0; i < object1_size.getX(); ++i)
	{
		for (int j = 0; j < object1_size.getY(); ++j)
		{
			if (next + Vec2<int>{ i, j } == obj->nextCoord || current + Vec2<int>{ i, j } == obj_current)
				return true;
		}
	}

	for (int i = 0; i < object2_size.getX(); ++i)
	{
		for (int j = 0; j < object2_size.getY(); ++j)
		{
			if (next == obj_next + Vec2<int>{ i, j } || current == obj_current + Vec2<int>{ i, j })
				return true;
			}
	}

	return false;
}
