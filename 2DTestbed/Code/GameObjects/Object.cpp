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
		m_sprite->SetScale({ sX, sY });
	}
	else
	{
		//unflip x
		m_sprite->SetScale({ -sX, sY });
	}
}

DynamicObject::DynamicObject(TexID sprId, const sf::Vector2f& boxSize)
	: Object(sprId, boxSize)
{
}

DynamicObject::DynamicObject(AnimatedSprite* sprite, const sf::Vector2f& boxSize)
	: Object(sprite, boxSize)
{
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
	if (GetPosition().x < GetOrigin().x || GetPosition().x > RightMost - GetOrigin().x)
		Move(-GetXVelocity() * FPS * deltaTime, 0);
}