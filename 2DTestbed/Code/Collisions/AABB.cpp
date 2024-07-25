#include "AABB.h"
#include "../Game/Constants.h"
#include <limits>

int AABB::s_count = 0;

AABB::AABB()
{
	m_boxNumber = s_count++;
	Reset(sf::Vector2f(16, 16));
	Update(sf::Vector2f());
}

AABB::AABB(const sf::Vector2f& size)
{
	m_boxNumber = s_count++;
	Reset(size);
	Update(sf::Vector2f());
}

void AABB::Reset(const sf::Vector2f& size)
{
	m_rect.setSize(size);
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Red);
	m_rect.setOutlineThickness(1);
	m_rect.setScale(sX, sY);
	m_rect.setOrigin(size * 0.5f);
}

void AABB::Update(const sf::Vector2f& pos)
{
	SetPosition(pos);

	auto floatRect = m_rect.getGlobalBounds();

	m_min[0] = std::min(floatRect.left, static_cast<float>(floatRect.left + floatRect.width));
	m_min[1] = std::min(floatRect.top, static_cast<float>(floatRect.top + floatRect.height));

	m_max[0] = std::max(floatRect.left, static_cast<float>(floatRect.left + floatRect.width));
	m_max[1] = std::max(floatRect.top, static_cast<float>(floatRect.top + floatRect.height));

	m_center[0] = GetPosition().x;
	m_center[1] = GetPosition().y;

	m_extents[0] = floatRect.width / 2;
	m_extents[1] = floatRect.height / 2;
}

void AABB::Render(sf::RenderWindow& window)
{
	window.draw(m_rect);
}

bool AABB::Intersects(AABB* box)
{
	m_previousOverlap = m_overlap;

	for (size_t i = 0; i < 2; i++)
		if (std::abs(m_center[i] - box->m_center[i]) > m_extents[i] + box->m_extents[i]) return false;

	// calculate the overlap
	sf::Vector2f delta = sf::Vector2f(std::abs(box->GetPosition().x - GetPosition().x), std::abs(box->GetPosition().y - GetPosition().y));
	m_overlap = Point((GetExtents().x + box->GetExtents().x) - delta.x, (GetExtents().y + box->GetExtents().y) - delta.y);

	box->SetHit(true);
	// Overlapping on all axes means AABBs are intersecting
	return true;
}

bool AABB::IntersectsMoving(AABB* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	// Exit early if ‘a’ and ‘b’ initially overlapping
	if (Intersects(box))
	{
		tfirst = tlast = 0.0f;
		return true;
	}

	// Use relative velocity; effectively treating ’a’ as stationary
	Point v = va - vb;

	// Initialize times of first and last contact
	tfirst = 0.0f;
	tlast = 1.0f;

	for (int i = 0; i < 2; i++)
	{
		if (v[i] < 0.0f)
		{
			if (box->m_max[i] < m_min[i]) return false; // Nonintersecting and moving apart
			if (m_max[i] < box->m_min[i]) tfirst = std::max((m_max[i] - box->m_min[i]) / v[i], tfirst);
			if (box->m_max[i] > m_min[i]) tlast = std::min((m_min[i] - box->m_max[i]) / v[i], tlast);
		}

		if (v[i] > 0.0f)
		{
			if (box->m_min[i] > m_max[i]) return false; // Nonintersecting and moving apart
			if (box->m_max[i] < m_min[i]) tfirst = std::max((m_min[i] - box->m_max[i]) / v[i], tfirst);
			if (m_max[i] > box->m_min[i]) tlast = std::min((m_max[i] - box->m_min[i]) / v[i], tlast);
		}

		// No overlap possible if time of first contact occurs after time of last contact
		if (tfirst > tlast) return false;
	}

	return true;
}

bool AABB::IntersectsRay(const Point& point, const Point& direction, float& tmin, Point& intersection)
{
	tmin = 0.0f;	// set to -FLT_MAX to get first hit on line
	float tmax = std::numeric_limits<float>::max();	// set to max distance ray can travel (for segment)

	// For both slabs
	for (int i = 0; i < 2; i++)
	{
		if (std::abs(direction[i]) < std::numeric_limits<float>::epsilon())
		{
			// Ray is parallel to slab. No hit if origin not within slab
			if (point[i]< m_min[i] || point[i] > m_min[i]) return false;
		}
		else
		{
			// Compute intersection t value of ray with near and far plane of slab
			float ood = 1.0f / direction[i];
			float t1 = (m_min[i] - point[i]) * ood;
			float t2 = (m_max[i] - point[i]) * ood;
			// Make t1 be intersection with near plane, t2 with far plane
			if (t1 > t2) std::swap(t1, t2);
			// Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 > tmax) tmax = t2;
			// Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}
	}

	// Ray intersects both slabs. Return point (q) and intersection t value (tmin)
	intersection = point + direction * tmin;
	return true;
}

bool AABB::IntersectsSegment(const Point& point0, const Point& point1)
{
	return false;
}

void AABB::Move(float x, float y)
{
	m_rect.move(sf::Vector2f(x, y));
	Update(GetPosition());
}

void AABB::Move(const sf::Vector2f& pos)
{
	m_rect.move(pos);
	Update(GetPosition());
}
