#pragma once

#include "../Utilities/MathHelper.h"
#include <SFML/Graphics.hpp>

class BoundingVolume
{
public:
	BoundingVolume() = default;
	explicit BoundingVolume(sf::Shape* shape);
	virtual ~BoundingVolume() = default;

	virtual void Update(const sf::Vector2f& position) = 0;
	void Render(sf::RenderWindow& window) const { window.draw(*m_shape); }

	sf::Shape* GetShape() { return m_shape.get(); }

	Vector<float> GetCenter() const { return m_center; }
	void SetCenter(const sf::Vector2f& center) { m_center = center; }

private:
	Vector<float> m_center;
	std::unique_ptr<sf::Shape> m_shape;
};

class AABB : public BoundingVolume
{
public:
	AABB();
	explicit AABB(const sf::Vector2f& ownerSize);
	~AABB() final = default;

	void Update(const sf::Vector2f& position) final;

	Vector<float> GetMin() const { return m_min; }
	Vector<float> GetMax() const { return m_max; }

	bool Intersects(const AABB* other) const;
	bool IntersectMoving(const AABB* other, const sf::Vector2f& va, const sf::Vector2f& vb, float& first, float& last) const;

private:
	Vector<float> m_size;
	Vector<float> m_min;
	Vector<float> m_max;
};

class CBB : public BoundingVolume
{
public:
	explicit CBB(float radius);
	~CBB() final = default;

	void Update(const sf::Vector2f& position) final;

	float GetRadius() const { return m_radius; }

	bool Intersects(const CBB* other) const;
	bool IntersectMoving(const CBB* other, const sf::Vector2f& va, const sf::Vector2f& vb, float& t) const;

private:
	float m_radius;
};