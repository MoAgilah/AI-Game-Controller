#include "../GameObjects/Bill.h"
#include "../../Collisions/Collisions.h"

Bill::Bill(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
	:Enemy(filepath, rows, cols, fps, bTyp, strloc, dir, symmetrical, initAnim, animSpd)
{
	m_type = BILL;
	numLives = maxLives = 1;
	m_falling = false;


	colbody.front.setOutlineColor(sf::Color::Red);
	colbody.front.setOutlineThickness(2.0f);
	colbody.front.setFillColor(sf::Color::Transparent);
	colbody.front.setRadius(74.f);
	colbody.front.setPointCount((size_t)30);
	colbody.front.setOrigin(74.f, 74.f);


	colbody.back.setOutlineColor(sf::Color::Red);
	colbody.back.setOutlineThickness(2.0f);
	colbody.back.setSize(sf::Vector2f(15.f, m_bbox->GetSprite()->getTexture()->getSize().y-2.f));
	colbody.back.setOrigin(sf::Vector2f(7.f,
		m_spr->GetSpr()->getTexture()->getSize().y / 2.f));
	colbody.back.setScale(sX, sY);
	colbody.back.setFillColor(sf::Color::Transparent);
}

void Bill::Die()
{
	m_alive = false;
	m_falling = true;
	timeLeftActive = 2.f;
}

void Bill::Render(sf::RenderWindow & window)
{
	window.draw(*m_spr->GetSpr());

#ifdef DEBUG
	#ifdef DRender
		window.draw(colbody.front);
		window.draw(colbody.back);
	#endif // DRender
#endif // DEBUG
}

Body Bill::GetBody()
{
	return colbody;
}


void Bill::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (m_direction)
	{
		m_velocity.x = 3;
	}
	else
	{
		m_velocity.x = -3;
	}

	if (m_alive == false)
	{
		m_velocity.y = + 3;

		m_velocity.x = 0;

		m_spr->Move(0, m_velocity.y * FPS * deltaTime);
	}

	if (m_velocity.x != 0)
	{
		m_spr->Move(m_velocity.x * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	colbody.front.setPosition(GetPosition());
	colbody.back.setPosition(GetPosition() + sf::Vector2f(m_bbox->GetSprite()->getOrigin().x*sX -17, 7));

	//check for leftmost and rightmost boundary
	if (m_spr->GetSpr()->getPosition().x < m_spr->GetSpr()->getOrigin().x || m_spr->GetSpr()->getPosition().x > 11776 - m_spr->GetSpr()->getOrigin().x)
	{
		m_spr->Move(-m_velocity.x * FPS * deltaTime, 0);
		m_direction = !m_direction;
	}
}