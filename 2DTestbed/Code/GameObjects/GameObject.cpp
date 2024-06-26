#include "GameObject.h"
#include "../Game/Constants.h"

int GameObject::s_objectNum = 0;

GameObject::GameObject(TexID boxId)
{
	m_bbox = std::make_shared<BoundingBox>(boxId);
	m_objectID = s_objectNum++;
}

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

StaticObject::StaticObject(TexID id, bool dir, const sf::Vector2f& pos)
	: GameObject(id)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialPosition(pos);
	SetPosition(GetInitialPosition());
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

AnimatedObject::AnimatedObject(TexID id, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: GameObject(new AnimatedSprite(id, FPS, symmetrical, initAnim, animSpd), (TexID)bTyp)
{
	SetID(id);
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialAnim(initAnim);
	GetAnimSpr()->ChangeAnim(m_initialAnim);
}

AnimatedObject::AnimatedObject(TexID id, int bTyp, bool dir, Cells cell, bool symmetrical, int initAnim, float animSpd)
	: GameObject(new AnimatedSprite(id, cell.m_rows, cell.m_cols, FPS, symmetrical, initAnim, animSpd), (TexID)bTyp)
{
	SetID(id);
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
	SetInitialAnim(initAnim);
	GetAnimSpr()->ChangeAnim(m_initialAnim);
}

void AnimatedObject::Reset()
{
	GameObject::Reset();
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	GetAnimSpr()->ChangeAnim(m_initialAnim);
}
