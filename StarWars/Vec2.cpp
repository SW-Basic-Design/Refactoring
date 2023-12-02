#include "Vec2.h"

template <typename Elem>
template <typename T>
Vec2<Elem>::Vec2(const Vec2<T>& o)
{
	x = static_cast<Elem>(o.x);
	y = static_cast<Elem>(o.y);
}

template <typename Elem>
Vec2<Elem>::Vec2(Elem v) : x(v), y(v) {}

template <typename Elem>
Vec2<Elem>::Vec2(Elem x, Elem y) : x(x), y(y) {}

template <typename Elem>
Vec2<Elem> Vec2<Elem>::operator+(const Vec2<Elem>& rhs) const
{
	return { x + rhs.x, y + rhs.y };
}

template <typename Elem>
Vec2<Elem> Vec2<Elem>::operator-(const Vec2<Elem>& rhs) const
{
	return { x - rhs.x, y - rhs.y };
}

template <typename Elem>
Vec2<Elem> Vec2<Elem>::operator*(const Vec2<Elem>& rhs) const
{
	return { x * rhs.x, y * rhs.y };
}

template <typename Elem>
Vec2<Elem> Vec2<Elem>::operator/(const Vec2<Elem>& rhs) const
{
	return { x / rhs.x, y / rhs.y };
}

template <typename Elem>
Vec2<Elem>& Vec2<Elem>::operator+=(const Vec2<Elem>& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template <typename Elem>
Vec2<Elem>& Vec2<Elem>::operator-=(const Vec2<Elem>& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template <typename Elem>
Vec2<Elem>& Vec2<Elem>::operator*=(const Vec2<Elem>& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
	return *this;
}

template <typename Elem>
Vec2<Elem>& Vec2<Elem>::operator/=(const Vec2<Elem>& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

template <typename Elem>
bool Vec2<Elem>::operator==(const Vec2<Elem>& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

template <typename Elem>
bool Vec2<Elem>::operator!=(const Vec2<Elem>& rhs) const
{
	return x != rhs.x || y != rhs.y;
}

template <typename Elem>
Elem Vec2<Elem>::dot(const Vec2<Elem>& rhs) const
{
	return x * rhs.x + y * rhs.y;
}

template <typename Elem>
Elem Vec2<Elem>::cross(const Vec2<Elem>& rhs) const
{
	return x * rhs.y - y * rhs.x;
}

template <typename Elem>
Elem Vec2<Elem>::norm() const
{
	return hypot(x, y);
}

template <typename Elem>
void Vec2<Elem>::normalize()
{
	*this /= norm();
}

template <typename Elem>
Elem Vec2<Elem>::area() const
{
	return x * y;
}

template <typename Elem>
void Vec2<Elem>::setX(Elem x)
{
	this->x = x;
}

template <typename Elem>
void Vec2<Elem>::setY(Elem y)
{
	this->y = y;
}

template <typename Elem>
Elem Vec2<Elem>::getX()
{
	return this->x;
}

template <typename Elem>
Elem Vec2<Elem>::getY()
{
	return this->y;
}
