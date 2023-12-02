#pragma once

#include "includes.h"

#ifndef VEC2_HPP
#define VEC2_HPP

template <typename Elem = double>
class Vec2 {
public:
    Elem x = 0, y = 0;
    Vec2() = default;
    template <typename T>
    Vec2(const Vec2<T>& o);
    Vec2(Elem v);
    Vec2(Elem x, Elem y);
    Vec2<Elem> operator+(const Vec2<Elem>& rhs) const;
    Vec2<Elem> operator-(const Vec2<Elem>& rhs) const;
    Vec2<Elem> operator*(const Vec2<Elem>& rhs) const;
    Vec2<Elem> operator/(const Vec2<Elem>& rhs) const;
    Vec2<Elem>& operator+=(const Vec2<Elem>& rhs);
    Vec2<Elem>& operator-=(const Vec2<Elem>& rhs);
    Vec2<Elem>& operator*=(const Vec2<Elem>& rhs);
    Vec2<Elem>& operator/=(const Vec2<Elem>& rhs);
    bool operator==(const Vec2<Elem>& rhs) const;
    bool operator!=(const Vec2<Elem>& rhs) const;
    Elem dot(const Vec2<Elem>& rhs) const;
    Elem cross(const Vec2<Elem>& rhs) const;
    Elem norm() const;
    void normalize();
    Elem area() const;
    void setX(Elem e);
    void setY(Elem e);
    Elem getX();
    Elem getY();
};

#endif