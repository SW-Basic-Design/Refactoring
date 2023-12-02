#pragma once

#include "includes.hpp"

#ifndef VEC2_HPP
#define VEC2_HPP

class Vec2 {
public:
    Vec2() = default;
    Vec2(const int v);
    Vec2(const int x, const int y);
    Vec2 operator+(const Vec2& rhs) const;
    Vec2 operator-(const Vec2& rhs) const;
    Vec2 operator*(const Vec2& rhs) const;
    Vec2 operator/(const Vec2& rhs) const;
    Vec2& operator+=(const Vec2& rhs);
    Vec2& operator-=(const Vec2& rhs);
    Vec2& operator*=(const Vec2& rhs);
    Vec2& operator/=(const Vec2& rhs);
    bool operator==(const Vec2& rhs) const;
    bool operator!=(const Vec2& rhs) const;
    void setX(const int e);
    void setY(const int e);
	int getX();
	int getY();
private:
	int x;
	int y;
};

#endif