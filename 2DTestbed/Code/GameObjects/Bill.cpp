#include "Bill.h"
#include "../Game/GameManager.h"

Bill::Bill(bool dir, const sf::Vector2f& initPos)
	: Enemy(TexID::Bill, sf::Vector2f(60,60), 1)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	m_halfCap.circle.setOutlineColor(sf::Color::Red);
	m_halfCap.circle.setOutlineThickness(2.0f);
	m_halfCap.circle.setFillColor(sf::Color::Transparent);
	m_halfCap.circle.setRadius(74.f);
	m_halfCap.circle.setOrigin(74.f, 74.f);

	m_halfCap.box.Reset(sf::Vector2f(28, 58));
	GetAABB()->Update(GetPosition());

	UpdateBody();
}

void Bill::Render(sf::RenderWindow& window)
{
	GetSprite()->Render(window);
	window.draw(m_halfCap.circle);
	m_halfCap.box.Render(window);
}

bool Bill::Intersects(Object* obj)
{
	return m_halfCap.Intersects(obj->GetAABB());
}

bool Bill::IsPlayerAbove(Player* ply)
{
	auto cnt = m_halfCap.circle.getPosition();
	auto rad = m_halfCap.circle.getRadius();
	auto end = cnt - Point(0, rad);

	Point mid = ((cnt - Point(rad, 0)) - cnt) + (end - cnt);
	mid = pnt::Normalize(mid) * rad;
	mid = cnt + mid;

	bool col = false;

	Circle circle(ply->GetAABB()->GetMax(), 4);
	Capsule capsule(Line(mid, end), 6);
	col = capsule.IntersectsCircle(circle);

	if (!col)
	{
		circle = Circle(ply->GetAABB(), 4);
		col = capsule.IntersectsCircle(circle);

		if (!col)
			col = capsule.IntersectsCircle(circle) && GetID() != TexID::PPlant;
	}

	return col;
}

void Bill::UpdateBody()
{
	m_halfCap.circle.setPosition(GetPosition());
	if (GetDirection())
	{
		m_halfCap.box.Update(GetPosition() - sf::Vector2f(GetAABB()->GetExtents().x/2 + 5, 0));
	}
	else
	{
		m_halfCap.box.Update(GetPosition() + sf::Vector2f(GetAABB()->GetExtents().x/2 + 5, 0));
	}
}

void Bill::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (HasLifes())
	{
		if (GetDirection())
		{
			SetXVelocity(GameConstants::ObjectSpeed);
		}
		else
		{
			SetXVelocity(-GameConstants::ObjectSpeed);
		}

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

bool HalfCapsule::Intersects(AABB* other)
{
	if (CircleToAABB(other))
		return true;

	return box.Intersects(other);
}

bool HalfCapsule::CircleToAABB(AABB* box)
{
	float sqDist = box->SqDistPointAABB(circle.getPosition());

	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= std::pow(circle.getRadius(),2);
}
