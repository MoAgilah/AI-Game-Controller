#include "../Collisions/Tile.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"
#include <format>

Tile::Tile()
{
	m_aabb.SetOutlineColour(sf::Color::Black);
}

Tile::Tile(int gX, int gY, const sf::Font& font)
	: m_hasFont(true)
{
	m_aabb.SetOutlineColour(sf::Color::Black);

	m_text.setFont(font);
	m_text.setCharacterSize(12);
	m_text.setOrigin(6, 6);
	m_text.setStyle(sf::Text::Bold);
}

void Tile::SetID(int gX, int gY)
{
	//set id
	m_id = std::format("{},{}", gX, gY);

	//numerical id
	m_colNum = gX;
	m_rowNum = gY;

	//set tile identifier text
	m_text.setString(std::format("{}\n{}", gX, gY));
}

void Tile::SetPosition(sf::Vector2f pos)
{
	m_aabb.Update(pos);
	m_text.setPosition(m_aabb.GetPosition().x - 10.f, m_aabb.GetPosition().y - 7.5f);
}

void Tile::CreateDesSlope()
{
	//creates a set of small rects to act like a slope inside of the slope tiles
	//this is because I was unsure how to do it the smooth tile method
	AABB box = AABB(sf::Vector2f(1, 1));
	box.SetOutlineColour(sf::Color::Black);
	box.Update(GetPosition() + sf::Vector2f(0, 5));
	m_slope.push_back(box);

	//up from center
	for (size_t i = 0; i < 8; i++)
	{
		box.Update(sf::Vector2f(m_slope.back().GetPosition().x - (m_slope.back().GetOrigin().x * sX) * 2.f, m_slope.back().GetPosition().y - (m_slope.back().GetOrigin().y * sY) * 2.f));
		m_slope.push_back(box);
	}

	box.Update(sf::Vector2f(m_slope[0].GetPosition().x + (m_slope[0].GetOrigin().x * sX) * 2.f, m_slope[0].GetPosition().y + (m_slope[0].GetOrigin().y * sY) * 2.f));
	m_slope.push_back(box);

	//down from center
	for (size_t i = 0; i < 8; i++)
	{
		box.Update(sf::Vector2f(m_slope.back().GetPosition().x + (m_slope.back().GetOrigin().x * sX) * 2.f, m_slope.back().GetPosition().y + (m_slope.back().GetOrigin().y * sY) * 2.f));
		m_slope.push_back(box);
	}
}

void Tile::CreateAscSlope()
{
	//creates a set of small rects to act like a slope inside of the slope tiles
	//this is because I was unsure how to do it the smooth tile method
	AABB box = AABB(sf::Vector2f(1, 1));
	box.SetOutlineColour(sf::Color::Black);
	box.Update(GetPosition() + sf::Vector2f(0, 5));
	m_slope.push_back(box);

	//up from center
	for (size_t i = 0; i < 8; i++)
	{
		box.Update(sf::Vector2f(m_slope.back().GetPosition().x + (m_slope.back().GetOrigin().x * sX) * 2.f, m_slope.back().GetPosition().y - (m_slope.back().GetOrigin().y * sY) * 2.f));
		m_slope.push_back(box);
	}

	box.Update(sf::Vector2f(m_slope[0].GetPosition().x - (m_slope[0].GetOrigin().x * sX) * 2.f, m_slope[0].GetPosition().y + (m_slope[0].GetOrigin().y * sY) * 2.f));
	m_slope.push_back(box);

	//down from center
	for (size_t i = 0; i < 6; i++)
	{
		box.Update(sf::Vector2f(m_slope.back().GetPosition().x - (m_slope.back().GetOrigin().x * sX) * 2.f, m_slope.back().GetPosition().y + (m_slope.back().GetOrigin().y * sY) * 2.f));
		m_slope.push_back(box);
	}
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(m_aabb.GetBox()->GetRect());

	if (m_hasFont)
		window.draw(m_text);

	for (int i = 0; i < m_slope.size(); i++)
	{
		window.draw(m_slope[i].GetRect());
	}
}

void Tile::SetType(int type)
{
	m_type = type;

	if (m_type == DIAGU )
	{
		CreateAscSlope();
	}

	if (m_type == DIAGD)
	{
		CreateDesSlope();
	}
}