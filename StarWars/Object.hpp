#pragma once

#include "Vec2.hpp"

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
	Object(Vec2, Vec2, Vec2, int);
	virtual ~Object() = default;
	Vec2& GetCoord();
	Vec2& GetNextCoord();
	Vec2& GetVelocity();
	int GetSpeed();
	void SetVelocity(const Vec2&);
	void SetNextCoord(const Vec2&);
	void SetCoord(const Vec2&);
	void SetSpeed(int);
	bool IsCollisionWith(Object*);
	ObjectType GetObjectType();
	bool IsPlayer();
	bool IsItem();
	int GetCollisionPriority();
	long long last_updated = 0;

protected:
	ObjectType object_type = ObjectType::OBJECT;
	Vec2 coord;
	Vec2 nextCoord;
	Vec2 velocity;
	int speed;
};

#endif