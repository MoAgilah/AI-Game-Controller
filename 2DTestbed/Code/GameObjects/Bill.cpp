#include "Bill.h"
#include "../Collisions/Collisions.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Bill, sf::Vector2f(64,59), 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));

	m_colbody.front.setOutlineColor(sf::Color::Red);
	m_colbody.front.setOutlineThickness(2.0f);
	m_colbody.front.setFillColor(sf::Color::Transparent);
	m_colbody.front.setRadius(74.f);
	m_colbody.front.setPointCount((size_t)30);
	m_colbody.front.setOrigin(74.f, 74.f);

	m_colbody.back.setOutlineColor(sf::Color::Red);
	m_colbody.back.setOutlineThickness(2.0f);
	m_colbody.back.setSize(sf::Vector2f(15.f, (float)(GetAABB()->GetExtents().y * 2) - 2.f));
	m_colbody.back.setOrigin(sf::Vector2f(7.f, (float)GetAABB()->GetExtents().y / 2.f));
	m_colbody.back.setScale(sX, sY);
	m_colbody.back.setFillColor(sf::Color::Transparent);
}

void Bill::Render(sf::RenderWindow& window)
{
	window.draw(*GetSprite()->GetSprite());
	window.draw(m_colbody.front);
	window.draw(m_colbody.back);
}

void Bill::ResolveCollisions(Object* other)
{
}

void Bill::ResolveCollisions(Tile* tile)
{
}

void Bill::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(1);
	}
	else
	{
		SetXVelocity(-1);
	}

	if (GetIsAlive())
	{
		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * FPS * deltaTime, 0);
			Collisions::Get()->ProcessCollisions(this);
			UpdateBoundingBox();
		}
	}
	else
	{
		SetVelocity(0, c_jumpSpeed);
		Move(0, GetYVelocity() * FPS * deltaTime);
	}

	if (GetDirection())
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() - sf::Vector2f(GetAABB()->GetOrigin().x * sX - 17, -7));
	}
	else
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() + sf::Vector2f(GetAABB()->GetOrigin().x * sX - 17, 7));
	}

	CheckForHorizontalBounds(deltaTime);
}
