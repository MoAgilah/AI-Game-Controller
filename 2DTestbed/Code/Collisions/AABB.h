#pragma once
#include "../Utilities/Point.h"
#include <SFML/Graphics.hpp>

class AABB
{
public:
	AABB();
	AABB(const sf::Vector2f& size);
	~AABB() = default;

	void Reset(const sf::Vector2f& size);

	void Update(const sf::Vector2f& pos);
	void Render(sf::RenderWindow& window);

	bool Intersects(AABB* box);
	bool IntersectsMoving(AABB* box, const Point& va, const Point& vb, float& tfirst, float& tlast);
	bool IntersectsRay(const Point& point, const Point& direction, float& tmin, Point& intersection);
	bool IntersectsSegment(const Point& point0, const Point& point1);

	AABB* GetBox() { return this; }
	const sf::RectangleShape& GetRect() { return m_rect; }

	void SetPosition(const sf::Vector2f& pos) { m_rect.setPosition(pos); }
	const sf::Vector2f& GetPosition() const { return m_rect.getPosition(); }

	void SetOrigin(const sf::Vector2f& origin) { m_rect.setOrigin(origin); }
	const sf::Vector2f& GetOrigin() const { return m_rect.getOrigin(); }
	const Point& GetExtents() const { return m_extents; }
	const Point& GetOverlap() const { return m_overlap; }
	const Point& GetPreviousOverlap() const { return m_previousOverlap; }

	void SetFillColour(const sf::Color& col) { m_rect.setFillColor(col); }
	void SetOutlineColour(const sf::Color& col) { m_rect.setOutlineColor(col); }

	void Move(float x, float y);
	void Move(const sf::Vector2f& pos);

	bool GetHit() const { return m_hit; }
	void SetHit(bool hit) { m_hit = hit; }
private:
	bool m_hit = false;
	int m_boxNumber;
	static int s_count;
	Point m_min;
	Point m_max;
	Point m_center;
	Point m_extents;
	Point m_overlap;
	Point m_previousOverlap;
	sf::RectangleShape m_rect;
};