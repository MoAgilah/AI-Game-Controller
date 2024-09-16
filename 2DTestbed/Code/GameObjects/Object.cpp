#include "Object.h"
#include <format>
#include <iostream>
#include "../Game/GameManager.h"

int Object::s_objectNum = 0;

Object::Object(TexID sprId, const sf::Vector2f& boxSize)
	: m_type(sprId)
{
	m_sprite = std::make_unique<Sprite>(sprId);
	m_aabb = std::make_unique<AABB>(boxSize);
	m_objectID = s_objectNum++;
	GameManager::GetGameMgr()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(AnimatedSprite* sprite, const sf::Vector2f& boxSize)
	: m_type(sprite->GetTexID())
{
	m_sprite.reset(std::move(sprite));
	m_aabb = std::make_unique<AABB>(boxSize);
	m_objectID = s_objectNum++;
	GameManager::GetGameMgr()->GetCollisionMgr()->AddCollidable(this);
}

void Object::Render(sf::RenderWindow& window)
{
	m_sprite->Render(window);
#if defined _DEBUG
	m_aabb->Render(window);
#endif
}

bool Object::Intersects(Object* obj)
{
	return GetAABB()->Intersects(obj->GetAABB());
}

void Object::Reset()
{
	m_active = false;
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
}

void Object::SetDirection(bool dir)
{
	m_direction = dir;
	if (m_direction)
	{
		// flip X
		m_sprite->SetScale(GameConstants::Scale);
	}
	else
	{
		//unflip x
		m_sprite->SetScale({ -GameConstants::Scale.x, GameConstants::Scale.y });
	}
}

DynamicObject::DynamicObject(TexID sprId, const sf::Vector2f& boxSize)
	: Object(sprId, boxSize)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

DynamicObject::DynamicObject(AnimatedSprite* sprite, const sf::Vector2f& boxSize)
	: Object(sprite, boxSize)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

void DynamicObject::Reset()
{
	Object::Reset();
	SetVelocity(sf::Vector2f(0.0f, 0.0f));
	m_onGround = false;
}

void DynamicObject::IncrementXVelocity(float x)
{
	m_velocity.x += x;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.x > physicsCtrl->GetMaxXVelocity())
		m_velocity.x = physicsCtrl->GetMaxXVelocity();
}

void DynamicObject::DecrementXVelocity(float x)
{
	m_velocity.x -= x;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.x < -physicsCtrl->GetMaxXVelocity())
		m_velocity.x = -physicsCtrl->GetMaxXVelocity();
}

void DynamicObject::IncrementYVelocity(float y)
{
	m_velocity.y += y;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.y > physicsCtrl->GetMaxYVelocity())
		m_velocity.y = physicsCtrl->GetMaxYVelocity();
}

void DynamicObject::DecrementYVelocity(float y)
{
	m_velocity.y -= y;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.y < -physicsCtrl->GetMaxYVelocity())
		m_velocity.y = -physicsCtrl->GetMaxYVelocity();
}

void DynamicObject::SetOnSlope(bool slp)
{
	m_onSlope = slp;
	if (m_onSlope)
		m_onGround = true;
}

void DynamicObject::SetShouldSlideLeft(bool left)
{
	m_shouldSlideLeft = left;
	if (m_shouldSlideLeft)
		m_shouldSlideRight = false;
}

void DynamicObject::SetSlideLeft(bool left)
{
	m_slideLeft = left;
	if (m_slideLeft)
		m_slideRight = false;
}

void DynamicObject::SetShouldSlideRight(bool right)
{
	m_shouldSlideRight = right;
	if (m_shouldSlideRight)
		m_shouldSlideLeft = false;
}

void DynamicObject::SetSlideRight(bool right)
{
	m_slideRight = right;
	if (m_slideRight)
		m_slideLeft = false;
}

void DynamicObject::Move(float x, float y)
{
	GetSprite()->Move(x, y);
	GetAABB()->Move(sf::Vector2f(x, y));
}

void DynamicObject::Move(const sf::Vector2f& pos)
{
	GetSprite()->Move(pos.x, pos.y);
	GetAABB()->Move(pos);
}

void DynamicObject::CheckForHorizontalBounds(float deltaTime)
{
	if (GetAABB()->GetPoint(Side::Left).x <= GameConstants::LeftMost)
	{
		Move(-GetXVelocity() * GameConstants::FPS * deltaTime, 0);
		if (!IsPlayerObject(GetID()))
			SetDirection(!GetDirection());
	}
}