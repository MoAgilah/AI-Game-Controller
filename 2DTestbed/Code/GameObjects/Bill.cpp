#include "Bill.h"
#include "../Game/GameManager.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Bill, sf::Vector2f(60,60), 1)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	m_colbody.front.setOutlineColor(sf::Color::Red);
	m_colbody.front.setOutlineThickness(2.0f);
	m_colbody.front.setFillColor(sf::Color::Transparent);
	m_colbody.front.setRadius(73.f);
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

bool Bill::IsPlayerAbove(Player* ply)
{
	Circle circle(ply->GetAABB(), 4);
	Capsule capsule(GetAABB()->GetSide(Side::Top), 6);

	if (capsule.line.IsPointAboveLine(circle.center))
		return capsule.IntersectsCircle(circle) && GetID() != TexID::PPlant;

	return false;
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

	if (HasLifes())
	{
		/*if (GetDirection())
		{
			SetXVelocity(GameConstants::ObjectSpeed);
		}
		else
		{
			SetXVelocity(-GameConstants::ObjectSpeed);
		}*/

		if (GetXVelocity() != 0)
		{
			Move(GetXVelocity() * GameConstants::FPS * deltaTime, 0);
			GameManager::GetGameMgr()->GetCollisionMgr()->ProcessCollisions(this);
		}
	}
	else
	{
		PhysicsController* physCtrl = GetPhysicsController();

		if (physCtrl->GetPhysicsType() != PhysicsType::drop)
			physCtrl->SetFalling();

		IncrementYVelocity(physCtrl->GetYAcceleration());

		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
	}

	UpdateBody();

	CheckForHorizontalBounds(deltaTime);
}

bool Body::Intersects(AABB* box)
{
	if (CircleToAABB(box))
		return true;

	return back.Intersects(box);
}

bool Body::CircleToAABB(AABB* box)
{
	float sqDist = box->SqDistPointAABB(front.getPosition());

	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= std::pow(front.getRadius(),2);
}
