#include "Object.h"
#include "../Collisions/Collisions.h"

int Object::s_objectNum = 0;

Object::Object(TexID sprId, TexID boxId)
	: m_type(sprId)
{
	m_sprite = std::make_shared<Sprite>(sprId);
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_objectID = s_objectNum++;
	Collisions::Get()->AddCollidable(this);
}

Object::Object(AnimatedSprite* sprite, TexID boxId)
{
	m_sprite.reset(std::move(sprite));
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_objectID = s_objectNum++;
	Collisions::Get()->AddCollidable(this);
}

void Object::Render(sf::RenderWindow& window)
{
	m_sprite->Render(window);
	m_bbox->Render(window);
}

void Object::Reset()
{
	m_active = false;
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
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

void Object::SetPosition(const sf::Vector2f& pos)
{
	m_sprite->SetPosition(pos);
	m_bbox->Update(sf::Vector2f(m_sprite->GetPosition().x, m_sprite->GetPosition().y + 3.5f));
}

void Object::SetPosition(float x, float y)
{
	m_sprite->SetPosition(sf::Vector2f(x, y));
	m_bbox->Update(sf::Vector2f(m_sprite->GetPosition().x, m_sprite->GetPosition().y + 3.5f));
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
	GetBBox()->GetSprite()->move(sf::Vector2f(x, y));
}

void DynamicObject::Move(const sf::Vector2f& pos)
{
	GetSprite()->Move(pos.x, pos.y);
	GetBBox()->GetSprite()->move(pos);
}