#include "../GameObjects/GameObject.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"

int GameObject::s_objectNum = 0;

GameObject::GameObject(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: m_type((int)id),  m_direction(dir)
{
	m_spawnData.m_initialDir = m_direction;
	m_spawnData.m_initialAnim = initAnim;

	m_spr = std::make_unique<AnimatedSprite>(id, rows, cols, FPS, symmetrical, initAnim, animSpd);
	m_curSpr = m_spr.get();

	m_bbox = std::make_unique<BoundingBox>((TexID)bTyp);
	m_curBBox = m_bbox.get();

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

GameObject::~GameObject()
{
	m_curSpr = nullptr;
	m_curBBox = nullptr;
}

void GameObject::Reset()
{
	m_curBBox = m_bbox.get();
	m_curSpr = m_spr.get();
	m_curSpr->ChangeAnim(m_spawnData.m_initialAnim);

	SetPosition(m_spawnData.m_initialPos);
	SetPrevPosition(m_spawnData.m_initialPos);

	m_visible = false;
	m_direction = m_spawnData.m_initialDir;
	m_onGround = false;
	m_airbourne = false;
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

