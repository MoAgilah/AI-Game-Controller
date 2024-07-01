#include "GameObject.h"
#include "../Game/Constants.h"

int GameObject::s_objectNum = 0;

GameObject::GameObject(TexID sprId, TexID boxId)
	: m_type((int)sprId)
{
	m_spr = std::make_shared<Sprite>(sprId);
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_objectID = s_objectNum++;
}

GameObject::GameObject(AnimatedSprite* sprite, TexID boxId)
{
	m_spr.reset(std::move(sprite));
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_objectID = s_objectNum++;
}

void GameObject::Render(sf::RenderWindow& window)
{
	m_spr->Render(window);
	m_bbox->Render(window);
}

void GameObject::Reset()
{
	m_visible = false;
}

void GameObject::SetDirection(bool dir)
{
	m_direction = dir;
	if (m_direction)
	{
		// flip X
		m_spr->SetScale({ sX, sY });
	}
	else
	{
		//unflip x
		m_spr->SetScale({ -sX, sY });
	}
}

void GameObject::Move(float x, float y)
{
	m_spr->GetSprite()->move(sf::Vector2f(x, y));
	m_bbox->GetSprite()->move(sf::Vector2f(x, y));
}

void GameObject::Move(const sf::Vector2f& pos)
{
	m_spr->GetSprite()->move(pos);
	m_bbox->GetSprite()->move(pos);
}

void GameObject::SetPosition(const sf::Vector2f& pos)
{
	m_spr->SetPosition(pos);
	auto yOffset = (float)(m_bbox->GetSprite()->getTextureRect().getSize().y / 2);
	m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x, m_spr->GetPosition().y + yOffset));
}
void GameObject::SetPosition(float x, float y)
{
	m_spr->SetPosition(sf::Vector2f(x, y));
	auto yOffset = (float)(m_bbox->GetSprite()->getTextureRect().getSize().y / 2);
	m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x, m_spr->GetPosition().y + yOffset));
}

StaticObject::StaticObject(TexID id, int bTyp, bool dir, const sf::Vector2f& pos)
	: GameObject(id, (TexID)bTyp)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
}

void StaticObject::Reset()
{
	GameObject::Reset();
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
}

AnimatedObject::AnimatedObject(TexID id, int bTyp, bool dir, bool symmetrical, float animSpd)
	: GameObject(new AnimatedSprite(id, FPS, symmetrical, animSpd), (TexID)bTyp)
{
	SetID(id);
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
}

AnimatedObject::AnimatedObject(TexID id, int bTyp, bool dir, Cells cell, bool symmetrical, float animSpd)
	: GameObject(new AnimatedSprite(id, cell.m_rows, cell.m_cols, FPS, symmetrical, animSpd), (TexID)bTyp)
{
	SetID(id);
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
}

void AnimatedObject::Reset()
{
	GameObject::Reset();
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	GetAnimSpr()->ChangeAnim(0);
}
