#include "Bill.h"
#include "../Game/GameManager.h"
#include "../Collisions/CollisionManager.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Bill, sf::Vector2f(60,60), 2)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	m_colbody.front.setOutlineColor(sf::Color::Red);
	m_colbody.front.setOutlineThickness(2.0f);
	m_colbody.front.setFillColor(sf::Color::Transparent);
	m_colbody.front.setRadius(73.f);
	m_colbody.front.setPointCount((size_t)30);
	m_colbody.front.setOrigin(73.f, 73.f);

	m_colbody.back.Reset(sf::Vector2f(30, 58));
	GetAABB()->Update(GetPosition());

	UpdateBody();
}

void Bill::Render(sf::RenderWindow& window)
{
	GetSprite()->Render(window);
	window.draw(m_colbody.front);
	m_colbody.back.Render(window);
}

bool Bill::Intersects(Object* obj)
{
	return GetBody().Intersects(obj->GetAABB());
}

void Bill::UpdateBody()
{
	if (GetDirection())
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.Update(GetPosition() - sf::Vector2f(GetAABB()->GetExtents().x/2, 0));
	}
	else
	{
		m_colbody.front.setPosition(GetPosition());
		m_colbody.back.Update(GetPosition() + sf::Vector2f(GetAABB()->GetExtents().x/2, 0));
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

bool Body::Intersects(AABB* box)
{
	if (CircleToAABB(front, box))
		return true;

	return back.Intersects(box);
}

bool Body::CircleToAABB(sf::CircleShape circle, AABB* box)
{
	//convert object into sphere
	sf::Vector2f Obj1Size = sf::Vector2f(box->GetOrigin().x * 2, box->GetOrigin().y * 2);

	Obj1Size.x *= sX;
	Obj1Size.y *= sY;

	float Radius1 = (Obj1Size.x + Obj1Size.y) * 0.25f;

	float Radius2 = circle.getRadius();

	//collision check
	sf::Vector2f Distance = box->GetPosition() - circle.getPosition();

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}
