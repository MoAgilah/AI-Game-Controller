#include "Object.h"
#include "../Game/GameManager.h"
#include "../Collisions/CollisionManager.h"

int Object::s_objectNum = 0;

Object::Object(TexID sprId, const sf::Vector2f& boxSize)
	: m_type(sprId)
{
	m_sprite = std::make_shared<Sprite>(sprId);
	m_aabb = std::make_shared<AABB>(boxSize);
	m_objectID = s_objectNum++;
	GameManager::GetGameMgr()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(AnimatedSprite* sprite, const sf::Vector2f& boxSize)
	: m_type(sprite->GetTexID())
{
	m_sprite.reset(std::move(sprite));
	m_aabb = std::make_shared<AABB>(boxSize);
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
		m_sprite->SetScale(scale);
	}
	else
	{
		//unflip x
		m_sprite->SetScale({ -scale.x, scale.y });
	}
}

DynamicObject::DynamicObject(TexID sprId, const sf::Vector2f& boxSize)
	: Object(sprId, boxSize)
{
	m_physicsCtrl = std::make_shared<PhysicsController>();
}

DynamicObject::DynamicObject(AnimatedSprite* sprite, const sf::Vector2f& boxSize)
	: Object(sprite, boxSize)
{
	m_physicsCtrl = std::make_shared<PhysicsController>();
}

void DynamicObject::Reset()
{
	Object::Reset();
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
	if (GetPosition().x < GetAABB()->GetExtents().x)
	{
		Move(-GetXVelocity() * FPS * deltaTime, 0);
		if (!IsPlayerObject(GetSprite()->GetTexID()))
			SetDirection(!GetDirection());
	}
}