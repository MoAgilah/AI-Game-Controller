#pragma once

#include "../Collisions/AABB.h"
#include "../Utilities/Utilities.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum Types
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, CRN, OWAY
};

class Tile
{
public:
	Tile();
	Tile(int gX, int gY, const sf::Font& font);
	~Tile() = default;

	std::string_view GetID() const { return m_id; }

	int GetRowNum() const { return m_rowNum; }
	int GetColNum() const { return m_colNum; }

	void SetType(int type) { m_type = type; };
	int GetType() const { return m_type; }

	bool GetActive() const { return m_visible; }
	void SetActive(bool vis) { m_visible = vis; }

	void SetPosition(sf::Vector2f pos);
	const sf::Vector2f& GetPosition() { return m_aabb.GetPosition(); }
	const sf::Vector2f& GetOrigin() { return m_aabb.GetOrigin(); }

	AABB* GetAABB() { return &m_aabb; }
	const sf::RectangleShape& GetRect() { return m_aabb.GetRect(); }
	Line GetSlope(int bgn, int end) { return Line(m_slope, bgn, end); }

	float GetTileHeight() { return m_aabb.GetRect().getSize().y; }

	void SetFillColour(sf::Color col) { m_aabb.SetFillColour(col); }
	void SetOutlineColour(sf::Color col) { m_aabb.SetOutlineColour(col); }

	void Render(sf::RenderWindow& window);

private:

	bool m_visible = false;
	bool m_hasFont = false;
	int m_colNum = -1;
	int m_rowNum = -1;
	int m_type = EMPTY;

	AABB m_aabb;
	sf::Text m_text;
	std::string m_id;
	sf::ConvexShape m_slope;
};