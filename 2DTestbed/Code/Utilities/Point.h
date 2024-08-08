#pragma once
#include <SFML/Graphics.hpp>

class Point
{
public:
	Point();
	Point(float x, float y);
	Point(const sf::Vector2f& vec);
	~Point() = default;

	float& operator[] (std::size_t index);
	const float& operator[] (std::size_t index) const;
	friend Point operator-(const Point& a, const Point& b);
	friend Point operator+(const Point& a, const Point& b);
	friend Point operator*(const Point& a, float scalar);
	friend Point operator*(float scalar, const Point& a);
	friend Point operator/(const Point& a, const Point& b);

	friend Point& operator+=(Point& a, const Point& b);
	friend Point& operator-=(Point& a, const Point& b);
	friend Point& operator*=(Point& a, float scalar);
	friend Point& operator/=(Point& a, const Point& b);

	Point& operator=(const Point& other);
	friend bool operator==(const Point& a, const Point& b);
	friend bool operator!=(const Point& a, const Point& b);

	operator sf::Vector2f() const;

	float x;
	float y;
};

namespace pnt
{
	float distance(const Point& p1, const Point& p2);
	float length(const Point& p);
}