#include "../GameObjects/Bill.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/BoundingBox.h"
#include "../Game/Constants.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	:Enemy(TexID::Bill, 1, 1, (int)TexID::BillBB, dir)
{
	m_spawnData.m_initialPos = initPos;
	SetPosition(m_spawnData.m_initialPos);

	m_colbody.front.setOutlineColor(sf::Color::Red);
	m_colbody.front.setOutlineThickness(2.0f);
	m_colbody.front.setFillColor(sf::Color::Transparent);
	m_colbody.front.setRadius(74.f);
	m_colbody.front.setPointCount((size_t)30);
	m_colbody.front.setOrigin(74.f, 74.f);

	m_colbody.back.setOutlineColor(sf::Color::Red);
	m_colbody.back.setOutlineThickness(2.0f);
	m_colbody.back.setSize(sf::Vector2f(15.f, (float)m_curBBox->GetSprite()->getTexture()->getSize().y - 2.f));
	m_colbody.back.setOrigin(sf::Vector2f(7.f, (float)m_curSpr->GetTextureSize().y / 2.f));
	m_colbody.back.setScale(sX, sY);
	m_colbody.back.setFillColor(sf::Color::Transparent);
}

void Bill::Render(sf::RenderWindow& window)
{
	window.draw(*GetSprite());
	window.draw(m_colbody.front);
	window.draw(m_colbody.back);
}

void Bill::Die()
{
	m_timeLeftActive = 2.f;
}

void Bill::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		m_velocity.x = 3;
	}
	else
	{
		m_velocity.x = -3;
	}

	if (!GetIsAlive())
	{
		m_velocity.y = + 3;

		m_velocity.x = 0;

		m_curSpr->Move(0, m_velocity.y * FPS * deltaTime);
	}

	if (m_velocity.x != 0)
	{
		m_curSpr->Move(m_velocity.x * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	if (GetDirection())
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() - sf::Vector2f(m_curBBox->GetSprite()->getOrigin().x * sX - 17, -7));
	}
	else
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() + sf::Vector2f(m_curBBox->GetSprite()->getOrigin().x * sX - 17, 7));
	}

	//check for leftmost and rightmost boundary
	if (m_curSpr->GetPosition().x < m_curSpr->GetOrigin().x || m_curSpr->GetPosition().x > 11776 - m_curSpr->GetOrigin().x)
	{
		m_curSpr->Move(-m_velocity.x * FPS * deltaTime, 0);
		SetDirection(!m_direction);
	}
}