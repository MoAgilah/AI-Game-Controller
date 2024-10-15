#include "../Collisions/Tile.h"
#include <format>
#include "../Game/Constants.h"

Tile::Tile()
{
	m_aabb.SetOutlineColour(sf::Color::Black);
}

Tile::Tile(int x, int y, const sf::Font& font)
	: m_hasFont(true)
{
	m_colNum = x;
	m_rowNum = y;

	m_id = std::format("{},{}", m_colNum, m_rowNum);

	m_aabb.SetOutlineColour(sf::Color::Black);

	m_text.setFont(font);
	m_text.setCharacterSize(12);
	m_text.setOrigin(6, 6);
	m_text.setStyle(sf::Text::Bold);
	m_text.setString(std::format("{}\n{}", m_colNum, m_rowNum));
}

void Tile::SetPosition(sf::Vector2f pos)
{
	m_aabb.Update(pos);
	if (m_type == DIAGU)
	{
		m_slope.setPointCount(3);
		m_slope.setPoint(0, m_aabb.GetPosition() + sf::Vector2f(-m_aabb.GetExtents().x, m_aabb.GetExtents().y));
		m_slope.setPoint(1, m_aabb.GetPosition() + sf::Vector2f(m_aabb.GetExtents().x, -m_aabb.GetExtents().y));
		m_slope.setPoint(2, m_aabb.GetPosition() + m_aabb.GetExtents());
		m_slope.setFillColor(sf::Color::Yellow);
	}

	if (m_type == DIAGD)
	{
		m_slope.setPointCount(3);
		m_slope.setPoint(0, m_aabb.GetPosition() - sf::Vector2f(m_aabb.GetExtents().x, m_aabb.GetExtents().y));
		m_slope.setPoint(1, m_aabb.GetPosition() + m_aabb.GetExtents());
		m_slope.setPoint(2, m_aabb.GetPosition() - sf::Vector2f(m_aabb.GetExtents().x, -m_aabb.GetExtents().y));
		m_slope.setFillColor(sf::Color::Yellow);
	}

	if (m_type == LCRN)
	{
		m_edge.start = m_aabb.GetMin() + Point(m_aabb.GetExtents().x * 2, 0);
		m_edge.end = m_edge.start - Point(0, GetTileHeight());
	}

	if (m_type == RCRN)
	{
		m_edge.start = m_aabb.GetMin();
		m_edge.end = m_edge.start - Point(0, GetTileHeight());
	}

	m_text.setPosition(m_aabb.GetPosition().x - 10.f, m_aabb.GetPosition().y - 7.5f);
}

void Tile::Render(sf::RenderWindow& window)
{
	if (m_type == DIAGU || m_type == DIAGD)
		window.draw(m_slope);

	if (m_type == LCRN || m_type == RCRN)
	{
		sf::Vertex line[2];
		line[0].position = m_edge.start;
		line[0].color = sf::Color::Red;
		line[1].position = m_edge.end;
		line[1].color = sf::Color::Red;

		window.draw(line, 2, sf::Lines);
	}

	window.draw(m_aabb.GetBox()->GetRect());

	if (m_hasFont)
		window.draw(m_text);
}
