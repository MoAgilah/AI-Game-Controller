#pragma once

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
	const sf::Vector2f& GetPosition() { return m_rect.getPosition(); }
	const sf::Vector2f& GetOrigin() { return m_rect.getOrigin(); }

	const sf::RectangleShape& GetRect() { return m_rect; }
	const std::vector<sf::RectangleShape>& GetSlopeBBox() const { return m_slope; }

	void SetFillColour(sf::Color col) { m_rect.setFillColor(col); }

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
	sf::RectangleShape m_rect;
	sf::Text m_text;
	sf::RectangleShape m_srect;
	std::vector<sf::RectangleShape> m_slope;
};