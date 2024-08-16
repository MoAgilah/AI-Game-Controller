#pragma once

#include <array>

#include "Point.h"
#include <SFML/Graphics.hpp>
#include "../Collisions/AABB.h"
#include "../Game/TextureManager.h"

bool IsPlayerObject(TexID id);
bool IsBoxObject(TexID id);
bool IsEnemyObject(TexID id);
bool IsCollectableObject(TexID id);

struct Line
{
	Line(const Point& start, const Point& end);
	Line(const sf::ConvexShape& tri, int bgn, int end);

	bool IsPointAboveLine(const Point& pnt) const;
	bool IntersectsPoint(const Point& pnt) const;

	float DistX() const { return end.x - start.x; }
	float DistY() const { return end.y - start.y; }

	Point start;
	Point end;
};

float GetXDist(const Point& p1, const Point& p2);
float GetYDist(const Point& p1, const Point& p2);

struct Circle
{
	Circle(const Point& position, float radius);
	Circle(AABB* box, float radius);

	bool IntersectsPoint(const Point& pnt) const;
	bool IntersectsLineSegment(const Line& line) const;

	float radius;
	Point center;
};

struct Capsule
{
	Capsule(const Line& line, float radius);

	bool IntersectsCircle(const Circle& circle) const;

	Line line;
	float radius;
};