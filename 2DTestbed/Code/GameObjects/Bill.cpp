#include "../GameObjects/Bill.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/BoundingBox.h"
#include "../Game/Constants.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	:Enemy(TexID::Bill, 1, 1, (int)TexID::BillBB, dir)
{
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	m_colbody.front.setOutlineColor(sf::Color::Red);
	m_colbody.front.setOutlineThickness(2.0f);
	m_colbody.front.setFillColor(sf::Color::Transparent);
	m_colbody.front.setRadius(74.f);
	m_colbody.front.setPointCount((size_t)30);
	m_colbody.front.setOrigin(74.f, 74.f);

	m_colbody.back.setOutlineColor(sf::Color::Red);
	m_colbody.back.setOutlineThickness(2.0f);
	m_colbody.back.setSize(sf::Vector2f(15.f, (float)GetBBox()->GetSprite()->getTexture()->getSize().y - 2.f));
	m_colbody.back.setOrigin(sf::Vector2f(7.f, (float)GetAnimSpr()->GetTextureSize().y / 2.f));
	m_colbody.back.setScale(sX, sY);
	m_colbody.back.setFillColor(sf::Color::Transparent);
}

void Bill::Render(sf::RenderWindow& window)
{
	window.draw(*GetAnimSpr()->GetSprite());
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
		SetXVelocity(c_moveSpeed);
	}
	else
	{
		SetXVelocity(-c_moveSpeed);
	}

	if (!GetIsAlive())
	{
		SetVelocity(0, c_jumpSpeed);

		GetAnimSpr()->Move(0, GetYVelocity() * FPS * deltaTime);
	}

	if (GetXVelocity() != 0)
	{
		GetAnimSpr()->Move(GetXVelocity() * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	if (GetDirection())
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() - sf::Vector2f(GetBBox()->GetSprite()->getOrigin().x * sX - 17, -7));
	}
	else
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.setPosition(GetPosition() + sf::Vector2f(GetBBox()->GetSprite()->getOrigin().x * sX - 17, 7));
	}

	//check for leftmost and rightmost boundary
	if (GetAnimSpr()->GetPosition().x < GetAnimSpr()->GetOrigin().x || GetAnimSpr()->GetPosition().x > 11776 - GetAnimSpr()->GetOrigin().x)
	{
		GetAnimSpr()->Move(-GetXVelocity() * FPS * deltaTime, 0);
		SetDirection(!GetDirection());
	}
}