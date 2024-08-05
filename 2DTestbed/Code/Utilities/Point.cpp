#include "Point.h"
#include <stdexcept>

namespace pnt
{
	float distance(const Point& p1, const Point& p2)
	{
		return std::sqrt(std::pow((p2.x - p1.x), 2) + std::pow((p2.y - p1.y), 2));
	}

	float length(const Point& p)
	{
		return std::sqrt(p.x * p.x + p.y * p.y);
	}
}

Point::Point()
	: x(0), y(0)
{
}

Point::Point(float x, float y)
	: x(x), y(y)
{
}

Point::Point(const sf::Vector2f& vec)
	: x(vec.x), y(vec.y)
{
}

bool Point::PointToCircle(const Point& center, float radius) const
{
	// get distance between the point and circle's center
	float d = pnt::distance(*this, center);
	return d <= radius;
}

bool Point::PointToLineSegmentIntersects(const Point& start, const Point& end) const
{
	// get distance from the point to the two ends of the line
	float d1 = pnt::distance(*this, start);
	float d2 = pnt::distance(*this, end);

	// get the length of the line
	float lineLen = pnt::distance(start, end);

	// since floats are so minutely accurate, add
	// a little buffer zone that will give collision
	float buffer = 0.1;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	return d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer;
}

float& Point::operator[](std::size_t index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Point subscript out of range");
	}
}

const float& Point::operator[](std::size_t index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Point subscript out of range");
	}
}

Point operator-(const Point& a, const Point& b)
{
	return Point(a.x - b.x, a.y - b.y);
}

Point operator+(const Point& a, const Point& b)
{
	return Point(a.x + b.x, a.y + b.y);
}

Point operator*(const Point& a, float scalar)
{
	return Point(a.x * scalar, a.y * scalar);
}

Point operator*(float scalar, const Point& a)
{
	return Point(a.x * scalar, a.y * scalar);
}

Point operator/(const Point& a, const Point& b)
{
	return Point(a.x / b.x, a.y / b.y);
}

Point& operator+=(Point& a, const Point& b)
{
	a.x += b.x;
	a.y += b.y;

	return a;
}

Point& operator-=(Point& a, const Point& b)
{
	a.x -= b.x;
	a.y -= b.y;

	return a;
}

Point& operator*=(Point& a, float scalar)
{
	a.x *= scalar;
	a.y *= scalar;

	return a;
}

Point& operator/=(Point& a, const Point& b)
{
	a.x /= b.x;
	a.y /= b.y;

	return a;
}

Point& Point::operator=(const Point& other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

bool operator==(const Point& a, const Point& b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point& a, const Point& b)
{
	return a.x != b.x || a.y != b.y;
}

Point::operator sf::Vector2f() const
{
	return sf::Vector2f(x, y);
}
