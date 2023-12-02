#pragma once

#include "Vec2.h"

#ifndef OBJECT_HPP
#define OBJECT_HPP

enum class ObjectType {
	OBJECT,
	CHARACTER,
	NON_PLAYER_CHARACTER,
	ENEMY_NPC, FRIENDLY_NPC,
	PLAYER_CHARACTER,
	DROPPED_ITEM,
	DROPPED_SPECIAL_ITEM, DROPPED_WEAPON,
	PARTICLE,
	WALL
};

class Object {
public:
	Object();
	Object(Vec2<double>, Vec2<double>, Vec2<double>, int);
	virtual ~Object() = default;
	Vec2<double>& GetCoord();
	Vec2<int> GetCoordAsInt();
	Vec2<double>& GetNextCoord();
	Vec2<int> GetNextCoordAsInt();
	Vec2<double>& GetVelocity();
	int GetSpeed();
	void SetVelocity(const Vec2<double>&);
	void SetNextCoord(const Vec2<double>&);
	void SetCoord(const Vec2<double>&);
	void SetSpeed(int);
	bool IsCollisionWith(Object*);
	ObjectType GetObjectType();
	bool IsCharacter();
	bool IsItem();
	int GetCollisionPriority();
	long long last_updated = 0;

protected:
	ObjectType object_type = ObjectType::OBJECT;
	Vec2<double> coord;
	Vec2<double> nextCoord;
	Vec2<double> velocity;
	Vec2<double> acceleration;
	Vec2<int> size;
	int speed;
};

#endif