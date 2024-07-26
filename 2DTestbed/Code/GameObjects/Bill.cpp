#include "Bill.h"
#include "../Game/GameManager.h"
#include "../Collisions/CollisionManager.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Bill, sf::Vector2f(64,46), 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());

	m_colbody.front.setOutlineColor(sf::Color::Red);
	m_colbody.front.setOutlineThickness(2.0f);
	m_colbody.front.setFillColor(sf::Color::Transparent);
	m_colbody.front.setRadius(73.f);
	m_colbody.front.setPointCount((size_t)30);
	m_colbody.front.setOrigin(73.f, 73.f);

	m_colbody.back.setOutlineColor(sf::Color::Red);
	m_colbody.back.setOutlineThickness(1.0f);
	m_colbody.back.setSize(sf::Vector2f(25.f, (float)(GetAABB()->GetExtents().y) - 2.f));
	m_colbody.back.setOrigin(sf::Vector2f(12.5f, (float)GetAABB()->GetExtents().y / 2.f));
	m_colbody.back.setScale(sX, sY);
	m_colbody.back.setFillColor(sf::Color::Transparent);

	UpdateBody();
}

void Bill::Render(sf::RenderWindow& window)
{
	window.draw(*GetSprite()->GetSprite());
	window.draw(m_colbody.front);
	window.draw(m_colbody.back);
}

void Bill::UpdateBody()
{
	if (GetDirection())
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() - sf::Vector2f(GetAABB()->GetOrigin().x * sX - 32, -3));
	}
	else
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() + sf::Vector2f(GetAABB()->GetOrigin().x * sX - 32, 3));
	}
}

void Bill::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	SetXVelocity((GetDirection() ? 1 : -1) * c_moveSpeed);

	if (GetIsAlive())
	{
		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * FPS * deltaTime, 0);
			GameManager::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
		}
	}
	else
	{
		SetVelocity(0, c_jumpSpeed);
		Move(0, GetYVelocity() * FPS * deltaTime);
	}

	UpdateBody();

	CheckForHorizontalBounds(deltaTime);
}
