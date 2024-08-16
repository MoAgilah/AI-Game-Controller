#include "Utilities.h"

namespace
{
	float distance(const Point& p1, const Point& p2)
	{
		return std::sqrtf(std::powf((p2.x - p1.x), 2) + std::powf((p2.y - p1.y), 2));
	}

	float length(const Point& p)
	{
		return std::sqrtf(p.x * p.x + p.y * p.y);
	}

	float Dot(const Point& a, const Point& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float SqDistPointSegment(const Point& a, const Point& b, const Point& c)
	{
		Point ab = b - a;
		Point ac = c - a;
		Point bc = c - b;

		float e = Dot(ac, ab);

		// Handle cases where c projects outside ab
		if (e <= 0.0f)
			return Dot(ac, ac);

		float f = Dot(ab, ab);
		if (e >= f)
			return Dot(bc, bc);

		// Handle cases where c projects onto ab
		return Dot(ac, ac) - e * e / f;
	}
}

Line::Line(const Point& start, const Point& end)
	: start(start), end(end)
{}

Line::Line(const sf::ConvexShape& tri, int bgn, int end)
	: Line(tri.getPoint(bgn), tri.getPoint(end))
{}

bool Line::IsPointAboveLine(const Point & pnt) const
{
	float s = (end.x - start.x) * (pnt.y - start.y) - (end.y - start.y) * (pnt.x - start.x);
	return s > 0;
}

bool Line::IntersectsPoint(const Point& pnt) const
{
	// get distance from the point to the two ends of the line
	float d1 = distance(pnt, end);
	float d2 = distance(pnt, start);

	float lineLen = distance(start, end);

	// a little buffer zone that will give collision
	float buffer = 0.1f;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	return d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer;
}

Circle::Circle(const Point& position, float radius)
	: center(position), radius(radius)
{}

Circle::Circle(AABB* box, float radius)
	: Circle(box->GetPoint(Side::Bottom), radius)
{}

bool Circle::IntersectsPoint(const Point& pnt) const
{
	// get distance between the point and circle's center
	float d = distance(pnt, center);
	return d <= radius;
}

bool Circle::IntersectsLineSegment(const Line & line) const
{
	if (IntersectsPoint(line.start))
		return true;

	if (IntersectsPoint(line.end))
		return true;

	// get length of the line
	auto lineLen = distance(line.start, line.end);

	// get dot product of the line and circle
	float dot = (((center.x - line.start.x) * (line.end.x - line.start.x)) +
		((center.y - line.start.y) * (line.end.y - line.start.y)))
		/ std::powf(lineLen, 2);

	// find the closest point on the line
	Point closestPnt = line.start + (dot * (line.end - line.start));

	// is this point actually on the line segment?
	// if so keep going, but if not, return false
	if (!line.IntersectsPoint(closestPnt))
		return false;

	// get distance to closest point
	float dist = distance(center, closestPnt);

	return dist <= radius;
}

Capsule::Capsule(const Line& line, float radius)
	: line(line), radius(radius)
{}

bool Capsule::IntersectsCircle(const Circle & circle) const
{
	float r = circle.radius + radius;

	float dist2 = SqDistPointSegment(line.start, line.end, circle.center);

	return dist2 <= r * r;
}

bool IsPlayerObject(TexID id)
{
	return id == TexID::Mario || id == TexID::Super;
}

bool IsBoxObject(TexID id)
{
	return id == TexID::QBox || id == TexID::SBox;
}

std::array<TexID, 5> enemyObject =
{
	TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck
};

bool IsEnemyObject(TexID id)
{
	return std::find(enemyObject.begin(), enemyObject.end(), id) != enemyObject.end();
}

std::array<TexID, 5> collectableObject =
{
	TexID::Coin, TexID::YCoin, TexID::ChkPnt, TexID::Shroom, TexID::Goal
};

bool IsCollectableObject(TexID id)
{
	return std::find(collectableObject.begin(), collectableObject.end(), id) != collectableObject.end();
}

float GetXDist(const Point& p1, const Point& p2)
{
	return p2.x - p1.x;
}

float GetYDist(const Point& p1, const Point& p2)
{
	return p2.y - p1.y;
}
