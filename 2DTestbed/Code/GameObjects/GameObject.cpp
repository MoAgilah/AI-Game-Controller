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

	m_spr = std::make_shared<AnimatedSprite>(id, rows, cols, FPS, symmetrical, initAnim, animSpd);
	m_bbox = std::make_shared<BoundingBox>((TexID)bTyp);

	SetDirection(m_spawnData.m_initialDir);

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

GameObject::GameObject(TexID id, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: m_type((int)id), m_direction(dir)
{
	m_spawnData.m_initialDir = m_direction;
	m_spawnData.m_initialAnim = initAnim;

	m_spr = std::make_shared<AnimatedSprite>(id, FPS, symmetrical, initAnim, animSpd);
	m_bbox = std::make_shared<BoundingBox>((TexID)bTyp);

	SetDirection(m_spawnData.m_initialDir);

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

void GameObject::Render(sf::RenderWindow& window)
{
	m_spr->Render(window);
	m_bbox->Render(window);
}

void GameObject::Reset()
{
	m_spr->ChangeAnim(m_spawnData.m_initialAnim);

	SetPosition(m_spawnData.m_initialPos);
	SetPrevPosition(m_spawnData.m_initialPos);

	m_visible = false;
	SetDirection(m_spawnData.m_initialDir);
	m_onGround = false;
	m_airbourne = false;
}

void GameObject::SetPosition(sf::Vector2f pos)
{
	m_spr->SetPosition(pos);
	if (GetDirection())
	{
		//+
		m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 2.f, m_spr->GetPosition().y + 3.5f));
	}
	else
	{
		//-
		m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 2.f, m_spr->GetPosition().y + 3.5f));
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_spr->SetPosition(sf::Vector2f(x, y));
	if (GetDirection())
	{
		//+
		m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 2.f, m_spr->GetPosition().y + 3.5f));
	}
	else
	{
		//-
		m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 2.f, m_spr->GetPosition().y + 3.5f));
	}
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

