#pragma once

#include <SFML/Graphics.hpp>
#include "../Collisions/BoundingVolume.h"

#include <vector>

enum Types
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, CRN, OWAY
};

class Camera;
class Tile
{
public:
	Tile() = default;
	explicit Tile(const sf::Font& font);
	~Tile() = default;

	void SetID(int gX, int gY);
	std::string_view GetID() const { return m_id; }

	int GetRowNum() const { return m_rowNum; }
	int GetCowNum() const { return m_colNum; }

	void SetType(int type);
	int GetType() const { return m_type; }

	bool GetActive() const { return m_visible; }
	void SetActive(bool vis) { m_visible = vis; }

	void SetPosition(sf::Vector2f pos);
	const sf::Vector2f& GetPosition() { return m_aabb.GetShape()->getPosition(); }
	const sf::Vector2f& GetOrigin() { return m_aabb.GetShape()->getOrigin(); }

	sf::RectangleShape GetRect() { return *static_cast<sf::RectangleShape*>(m_aabb.GetShape()); }
	const std::vector<sf::RectangleShape>& GetSlopeBBox() const { return m_slope; }

	void SetFillColour(sf::Color col) { m_aabb.GetShape()->setFillColor(col); }

	void Render(sf::RenderWindow& window);

private:
	void CreateDesSlope();
	void CreateAscSlope();

	bool m_visible = false;
	bool m_hasFont = false;
	int m_colNum = -1;
	int m_rowNum = -1;
	int m_type = EMPTY;

	std::string m_id;
	AABB m_aabb;
	sf::Text m_text;
	sf::RectangleShape m_srect;
	std::vector<sf::RectangleShape> m_slope;
};