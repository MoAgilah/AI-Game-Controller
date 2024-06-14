#include "BoundingVolume.h"
#include "../Game/Constants.h"
#include "../Collisions/Tile.h"

#include <algorithm>
#include <cmath>

namespace
{
	float SqDistPointAABB(const Vector<float>& point, const AABB* box)
	{
		float sqDist = 0.0f;

		for (int i = 0; i < 2; i++)
		{
			float v = point[i];
			if (v < box->GetMin()[i]) sqDist += (box->GetMin()[i] - v) * (box->GetMin()[i] - v);
			if (v > box->GetMax()[i]) sqDist += (box->GetMax()[i] - v) * (box->GetMax()[i] - v);
		}

		return sqDist;
	}
}

BoundingVolume::BoundingVolume(sf::Shape* shape)
{
	m_shape.reset(shape);
}

AABB::AABB()
	: BoundingVolume(new sf::RectangleShape(sf::Vector2f(16, 16)))
{
	GetShape()->setOrigin(16 * .5f, 16 * .5f);
	GetShape()->setScale(sX,sY);
	GetShape()->setOutlineColor(sf::Color::Red);
	GetShape()->setOutlineThickness(1);
	GetShape()->setFillColor(sf::Color::Transparent);

	Update(sf::Vector2f());
}

AABB::AABB(const sf::Vector2f& ownerSize)
	: BoundingVolume(new sf::RectangleShape())
{
	sf::Vector2f sf(.6f, .6f);
	if (ownerSize.x > ownerSize.y)
		sf.y *= (ownerSize.y / ownerSize.x);
	else if (ownerSize.y > ownerSize.x)
		sf.x *= (ownerSize.x / ownerSize.y);

	auto rect = static_cast<sf::RectangleShape*>(GetShape());
	rect->setSize(sf::Vector2f(ownerSize.x * sf.x, ownerSize.y * sf.y));
	m_size = rect->getSize();

	GetShape()->setOrigin(m_size.x * .5f, m_size.y * .5f);
	GetShape()->setScale(sX, sY);
	GetShape()->setOutlineColor(sf::Color::Red);
	GetShape()->setOutlineThickness(1);
	GetShape()->setFillColor(sf::Color::Transparent);

	Update(sf::Vector2f());
}

void AABB::Update(const sf::Vector2f& position)
{
	GetShape()->setPosition(position);
	m_min = Vector<float>(GetShape()->getTransform().transformPoint(sf::Vector2f(0, 0)));
	m_max = Vector<float>(GetShape()->getTransform().transformPoint(sf::Vector2f(m_size.x, m_size.y)));
	SetCenter(sf::Vector2f((m_min.x + m_max.x) / 2, (m_min.y + m_max.y) / 2));
}

bool AABB::Intersects(const AABB* other) const
{
	for (size_t i = 0; i < 2; i++)
		if (GetMax()[0] < other->GetMin()[0] || GetMin()[0] > other->GetMax()[0]) return false;

	return true;
}

bool AABB::IntersectMoving(const AABB* other, const sf::Vector2f& va, const sf::Vector2f& vb, float& tfirst, float& tlast) const
{
	// exit early if initially overlapping with other
	if (Intersects(other))
	{
		tfirst = tlast = 0;
		return true;
	}

	// Use relative velocity; effectively treating ’a’ as stationary
	auto v = Vector<float>(vb - va);

	// Initialize times of first and last contact
	tfirst = 0.0f;
	tlast = 1.0f;

	for (int i = 0; i < 2; i++)
	{
		if (v[i] < 0.0f)
		{
			if (other->GetMax()[i] < GetMin()[i])
				return false; // Nonintersecting and moving apart

			if (GetMax()[i] < other->GetMin()[i])
				tfirst = std::min((GetMax()[i] - other->GetMin()[i]) / v[i], tfirst);

			if (other->GetMax()[i] > GetMin()[i])
				tlast = std::min((GetMin()[i] - other->GetMax()[i]) / v[i], tlast);
		}
		else if (v[i] > 0.0f)
		{
			if (other->GetMin()[i] > GetMax()[i])
				return false; // Nonintersecting and moving apart

			if (other->GetMax()[i] < GetMin()[i])
				tfirst = std::max((GetMin()[i] - other->GetMax()[i]) / v[i], tfirst);

			if (GetMax()[i] > other->GetMin()[i])
				tlast = std::min((GetMax()[i] - other->GetMin()[i]) / v[i], tlast);
		}

		// No overlap possible if time of first contact occurs after time of last contact
		if (tfirst > tlast) return false;
	}

	return true;
}

CBB::CBB(float radius)
	: BoundingVolume(new sf::CircleShape(radius)), m_radius(radius)
{
	GetShape()->setOrigin(m_radius, m_radius);
	GetShape()->setScale(sX, sY);
	GetShape()->setOutlineColor(sf::Color::Red);
	GetShape()->setOutlineThickness(1);
	GetShape()->setFillColor(sf::Color::Transparent);

	Update(sf::Vector2f());
}

void CBB::Update(const sf::Vector2f& position)
{
	GetShape()->setPosition(position);
	SetCenter(sf::Vector2f(GetShape()->getPosition().x + GetRadius(), GetShape()->getPosition().y + GetRadius()));
}

bool CBB::Intersects(const CBB* other) const
{
	// calculate square distance between centers
	Vector<float> d = Vector<float>(GetCenter() - other->GetCenter());
	float distSq = d.Dot(d);

	// spheres intersects if squared distance is less than square sum of the radii
	float radiusSum = GetRadius() + other->GetRadius();
	return distSq <= radiusSum * radiusSum;
}

bool CBB::IntersectMoving(const CBB* other, const sf::Vector2f& va, const sf::Vector2f& vb, float& t) const
{
	Vector<float> s = Vector<float>(other->GetCenter() - GetCenter()); // Vector between sphere centers
	Vector<float> v = Vector<float>(vb - va); // Relative motion of s1 with respect to stationary s0
	float r = GetRadius() + other->GetRadius(); // Sum of sphere radii
	float c = s.Dot(s) - r * r;

	// Spheres initially overlapping so exit directly
	if (c < 0.0f)
	{
		t = 0.0f;
		return true;
	}

	float a = v.Dot(v);
	if (a < std::numeric_limits<float>::epsilon()) return false; // Spheres not moving relative each other

	float b = v.Dot(s);
	if (b >= 0.0f) return false; // Spheres not moving towards each other

	float d = b * b - a * c;
	if (d < 0.0f) return false; // No real-valued root, spheres do not intersect

	t = (-b - std::sqrt(d)) / a;

	return true;
}
