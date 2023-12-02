#include "Vec2.hpp"


Vec2::Vec2(const int v) : x(v), y(v) {}

Vec2::Vec2(const int x, const int y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return { x + rhs.x, y + rhs.y };
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return { x - rhs.x, y - rhs.y };
}

Vec2 Vec2::operator*(const Vec2& rhs) const
{
	return { x * rhs.x, y * rhs.y };
}

Vec2 Vec2::operator/(const Vec2& rhs) const
{
	return { x / rhs.x, y / rhs.y };
}

Vec2& Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vec2& Vec2::operator*=(const Vec2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

Vec2& Vec2::operator/=(const Vec2& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

void Vec2::setX(int x)
{
	this->x = x;
}

void Vec2::setY(int y)
{
	this->y = y;
}

int Vec2::getX()
{
	return this->x;
}

int Vec2::getY()
{
	return this->y;
}