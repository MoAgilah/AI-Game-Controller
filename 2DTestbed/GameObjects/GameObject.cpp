#include "../GameObjects/GameObject.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"

int GameObject::s_objectNum = 0;

GameObject::GameObject(std::string_view filepath, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: m_type(bTyp),  m_direction(dir)
{
	m_spawnData.m_initialDir = m_direction;
	m_spawnData.m_initialAnim = initAnim;

	m_spr = std::make_unique<AnimatedSprite>(filepath, rows, cols, FPS, symmetrical, initAnim, animSpd);
	m_curSpr = m_spr.get();

	m_bbox = std::make_unique<BoundingBox>(filepath.substr(0, filepath.find(".")), bTyp);
	m_curBBox = m_bbox.get();

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

GameObject::~GameObject()
{
	m_curSpr = nullptr;
	m_curBBox = nullptr;
}

void GameObject::SetPosition(sf::Vector2f pos)
{
	m_curSpr->SetPosition(pos);
	if (m_direction)
	{
		//+
		m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 2.f, m_curSpr->GetPosition().y + 3.5f));
	}
	else
	{
		//-
		m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 2.f, m_curSpr->GetPosition().y + 3.5f));
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_curSpr->SetPosition(sf::Vector2f(x, y));
	if (m_direction)
	{
		//+
		m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 2.f, m_curSpr->GetPosition().y + 3.5f));
	}
	else
	{
		//-
		m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 2.f, m_curSpr->GetPosition().y + 3.5f));
	}
}

