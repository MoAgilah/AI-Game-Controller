#include "../Collisions/Tile.h"
#include "../Game/Camera.h"

Tile::Tile()
{
	m_rect.setSize(sf::Vector2f(16, 16));
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Black);
	m_rect.setOutlineThickness(1);
	m_rect.setScale(sX, sY);
	m_rect.setOrigin(8, 8);

	hasFont = false;
}

Tile::Tile(sf::Font& font)
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

	m_type = EMPTY;

	hasFont = true;
}

Tile::~Tile()
{
	if (m_font)
	{
		m_font = nullptr;
	}
}

void Tile::SetID(int gX, int gY)
{
	//flip y
	InvertY(gY);

	//set id
	m_id = std::to_string(gX) + std::to_string(gY);

	//numerical id
	m_colNum = gX;
	m_rowNum = gY;

	//set tile identifier text
	m_text.setString(std::to_string(gY) + "\n" + std::to_string(gX));
}

sf::Vector2f Tile::GetOrigin()
{
	return m_rect.getOrigin();
}

sf::Vector2f Tile::GetPosition()
{
	return m_rect.getPosition();
}

void Tile::SetPosition(sf::Vector2f pos)
{
	m_rect.setPosition(pos);
	m_text.setPosition(m_rect.getPosition().x - 10.f, m_rect.getPosition().y - 7.5f);
}

void Tile::InvertY(int& y)
{
	switch (y)
	{
	case 0: y = 14; break;
	case 1: y = 13; break;
	case 2: y = 12; break;
	case 3: y = 11; break;
	case 4: y = 10; break;
	case 5: y = 9; break;
	case 6: y = 8; break;
	case 7: break;
	case 8: y = 6; break;
	case 9: y = 5; break;
	case 10: y = 4; break;
	case 11: y = 3; break;
	case 12: y = 2; break;
	case 13: y = 1; break;
	case 14: y = 0; break;
	}
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

	if (hasFont)
	{
		window.draw(m_text);
	}

	for (int i = 0; i < m_slope.size(); i++)
	{
		window.draw(m_slope[i]);
	}
}

std::string Tile::GetID()
{
	return m_id;
}

int Tile::GetType()
{
	return m_type;
}

sf::RectangleShape Tile::GetRect()
{
	return m_rect;
}

std::vector<sf::RectangleShape> Tile::GetSlopeBBox()
{
	return m_slope;
}

bool Tile::GetActive()
{
	return m_active;
}

void Tile::SetActive(bool vis)
{
	m_active = vis;
}

int Tile::GetRowNum()
{
	return m_rowNum;
}
int Tile::GetCowNum()
{
	return m_colNum;
}

void Tile::SetFillColour(sf::Color col)
{
	m_rect.setFillColor(col);
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