#include "../Collisions/Tile.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"
#include <format>

Tile::Tile(const sf::Font& font)
	: m_hasFont(true)
{
	m_rect.setSize(sf::Vector2f(16, 16));
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Black);
	m_rect.setOutlineThickness(1);
	m_rect.setScale(sX, sY);
	m_rect.setOrigin(8, 8);

	m_srect.setSize(sf::Vector2f(1, 1));
	m_srect.setFillColor(sf::Color::Transparent);
	m_srect.setOutlineColor(sf::Color::Black);
	m_srect.setOutlineThickness(1);
	m_srect.setScale(sX, sY);
	m_srect.setOrigin(0.5, 0.5);

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
	m_rect.setPosition(pos);
	m_text.setPosition(m_rect.getPosition().x - 10.f, m_rect.getPosition().y - 7.5f);
}

void Tile::CreateDesSlope()
{
	//creates a set of small rects to act like a slope inside of the slope tiles
	//this is because I was unsure how to do it the smooth tile method
	m_srect.setPosition(GetPosition() + sf::Vector2f(0, 5));
	m_slope.push_back(m_srect);

	//up from center
	for (size_t i = 0; i < 8; i++)
	{
		m_srect.setPosition(m_slope.back().getPosition().x - (m_slope.back().getOrigin().x*sX) * 2.f, m_slope.back().getPosition().y - (m_slope.back().getOrigin().y*sY) * 2.f);
		m_slope.push_back(m_srect);
	}

	m_srect.setPosition(m_slope[0].getPosition().x + (m_slope[0].getOrigin().x*sX) * 2.f, m_slope[0].getPosition().y + (m_slope[0].getOrigin().y*sY) * 2.f);
	m_slope.push_back(m_srect);

	//down from center
	for (size_t i = 0; i < 8; i++)
	{
		m_srect.setPosition(m_slope.back().getPosition().x + (m_slope.back().getOrigin().x*sX) * 2.f, m_slope.back().getPosition().y + (m_slope.back().getOrigin().y*sY) * 2.f);
		m_slope.push_back(m_srect);
	}
}

void Tile::CreateAscSlope()
{
	//creates a set of small rects to act like a slope inside of the slope tiles
	//this is because I was unsure how to do it the smooth tile method
	m_srect.setPosition(GetPosition() + sf::Vector2f(0, 5));
	m_slope.push_back(m_srect);

	//up from center
	for (size_t i = 0; i < 8; i++)
	{
		m_srect.setPosition(m_slope.back().getPosition().x + (m_slope.back().getOrigin().x*sX) * 2.f, m_slope.back().getPosition().y - (m_slope.back().getOrigin().y*sY) * 2.f);
		m_slope.push_back(m_srect);
	}

	m_srect.setPosition(m_slope[0].getPosition().x - (m_slope[0].getOrigin().x*sX) * 2.f, m_slope[0].getPosition().y + (m_slope[0].getOrigin().y*sY) * 2.f);
	m_slope.push_back(m_srect);

	//down from center
	for (size_t i = 0; i < 6; i++)
	{
		m_srect.setPosition(m_slope.back().getPosition().x - (m_slope.back().getOrigin().x*sX) * 2.f, m_slope.back().getPosition().y + (m_slope.back().getOrigin().y*sY) * 2.f);
		m_slope.push_back(m_srect);
	}
}

void Tile::Render(sf::RenderWindow& window)
{
	window.draw(m_rect);

	if (m_hasFont)
		window.draw(m_text);

	for (int i = 0; i < m_slope.size(); i++)
	{
		window.draw(m_slope[i]);
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