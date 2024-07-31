#pragma once

#include "../Collisions/AABB.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum Types
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, CRN, OWAY
};

class Camera;
class Tile
{
public:
	Tile();
	Tile(int gX, int gY, const sf::Font& font);
	~Tile() = default;

	void SetID(int gX, int gY);
	std::string_view GetID() const { return m_id; }

	int GetRowNum() const { return m_rowNum; }
	int GetColNum() const { return m_colNum; }

	void SetType(int type);
	int GetType() const { return m_type; }

	bool GetActive() const { return m_visible; }
	void SetActive(bool vis) { m_visible = vis; }

	void SetPosition(sf::Vector2f pos);
	const sf::Vector2f& GetPosition() { return m_aabb.GetPosition(); }
	const sf::Vector2f& GetOrigin() { return m_aabb.GetOrigin(); }

	AABB* GetAABB() { return &m_aabb; }
	const sf::RectangleShape& GetRect() { return m_aabb.GetRect(); }
	std::vector<AABB>& GetSlopeBBox() { return m_slope; }

	void SetFillColour(sf::Color col) { m_aabb.SetFillColour(col); }
	void SetOutlineColour(sf::Color col) { m_aabb.SetOutlineColour(col); }

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
	std::vector<AABB> m_slope;
};