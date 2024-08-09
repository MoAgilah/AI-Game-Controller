#include "AABB.h"
#include "../Utilities/Utilities.h"
#include "../Game/Constants.h"
#include <limits>

int AABB::s_count = 0;

AABB::AABB()
{
	m_boxNumber = s_count++;
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Red);
	m_rect.setOutlineThickness(1);
	Reset(sf::Vector2f(16, 16));
	Update(sf::Vector2f());
}

AABB::AABB(const sf::Vector2f& size)
{
	m_boxNumber = s_count++;
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Red);
	m_rect.setOutlineThickness(1);
	Reset(size);
	Update(sf::Vector2f());
}

void AABB::Reset(const sf::Vector2f& size)
{
	m_rect.setSize(size);
	m_rect.setScale(scale);
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
	for (size_t i = 0; i < 2; i++)
		if (std::abs(m_center[i] - box->m_center[i]) > m_extents[i] + box->m_extents[i]) return false;

	// calculate the overlap
	Point delta = box->GetPosition() - GetPosition();
	delta = Point(std::abs(delta.x), std::abs(delta.y));
	m_overlap = (GetExtents() + box->GetExtents()) - delta;

	// Overlapping on all axes means AABBs are intersecting
	return true;
}

bool AABB::IntersectsMoving(AABB* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	// Exit early if �a� and �b� initially overlapping
	if (Intersects(box))
	{
		tfirst = tlast = 0.0f;
		return true;
	}

	// Use relative velocity; effectively treating �a� as stationary
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

Line AABB::GetSide(Side side)
{
	switch (side)
	{
	case Left:
		return Line(m_min, Point(m_min.x, m_max.y));
	case Right:
		return Line(Point(m_max.x, m_min.y), m_max);
	case Top:
		return Line(m_min, Point(m_max.x, m_min.y));
	case Bottom:
		return Line(Point(m_min.x, m_max.y), m_max);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
}

Point AABB::GetPoint(Side side)
{
	switch (side)
	{
	case Left:
		return m_center - Point(m_rect.getSize().x, 0);
	case Right:
		return m_center + Point(m_rect.getSize().x, 0);
	case Top:
		return m_center - Point(0, m_rect.getSize().y);
	case Bottom:
		return m_center + Point(0, m_rect.getSize().y);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
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
