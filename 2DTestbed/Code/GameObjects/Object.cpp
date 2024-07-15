#include "Object.h"
#include "../Collisions/Collisions.h"

int Object::s_objectNum = 0;

Object::Object(TexID sprId, TexID boxId)
	: m_type(sprId)
{
	m_sprite = std::make_shared<Sprite>(sprId);
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_box = std::make_shared<AABB>();
	m_objectID = s_objectNum++;
	Collisions::Get()->AddCollidable(this);
}

Object::Object(AnimatedSprite* sprite, TexID boxId)
	: m_type(sprite->GetTexID())
{
	m_sprite.reset(std::move(sprite));
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_box = std::make_shared<AABB>();
	m_objectID = s_objectNum++;
	Collisions::Get()->AddCollidable(this);
}

void Object::Render(sf::RenderWindow& window)
{
	m_sprite->Render(window);
	m_box->Render(window);
//#if defined _DEBUG
//	m_bbox->Render(window);
//#endif
}

void Object::Reset()
{
	m_active = false;
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	GetBBox()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f));
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

DynamicObject::DynamicObject(TexID sprId, TexID boxId)
	: Object(sprId, boxId)
{
}

DynamicObject::DynamicObject(AnimatedSprite* sprite, TexID boxId)
	: Object(sprite, boxId)
{
}

void DynamicObject::Move(float x, float y)
{
	GetSprite()->Move(x, y);
	GetBox()->GetShape()->move(sf::Vector2f(x, y));
	GetBBox()->GetSprite()->move(sf::Vector2f(x, y));
}

void DynamicObject::Move(const sf::Vector2f& pos)
{
	GetSprite()->Move(pos.x, pos.y);
	GetBBox()->GetSprite()->move(pos);
}

void DynamicObject::CheckForHorizontalBounds(float deltaTime)
{
	if (GetPosition().x < GetOrigin().x || GetPosition().x > RightMost - GetOrigin().x)
	{
		Move(-GetXVelocity() * FPS * deltaTime, 0);
		SetDirection(!GetDirection());
	}
}