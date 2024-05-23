#include "../GameObjects/GameObject.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"

int GameObject::s_objectNum = 0;
int GameObject::s_numOfEnemies = 0;
int GameObject::s_numOfObjects = 0;

GameObject::GameObject(std::string_view filepath, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: m_type(bTyp), m_initialDir(dir), m_direction(m_initialDir)
{
	m_spr = new AnimatedSprite(filepath, rows, cols, FPS, symmetrical, initAnim, animSpd);

	m_bbox = new BoundingBox(filepath.substr(0, filepath.find(".")), bTyp);

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

GameObject::~GameObject()
{
	if (m_spr)
	{
		delete m_spr;
		m_spr = nullptr;
	}

	if (m_bbox)
	{
		delete m_bbox;
		m_bbox = nullptr;
	}
}

sf::Sprite* GameObject::GetSprite()
{
	return m_spr->GetSprite();
}

BoundingBox * GameObject::GetBBox()
{
	return m_bbox;
}

void GameObject::Setm_initialPos(sf::Vector2f pos)
{
	m_initialPos = pos;
}

sf::Vector2f GameObject::GetPosition() const
{
	return m_spr->GetPosition();
}

void GameObject::SetPosition(sf::Vector2f pos)
{
	m_spr->SetPosition(pos);
}

void GameObject::SetPosition(float x, float y)
{
	m_spr->SetPosition((sf::Vector2f(x, y)));
}

sf::Vector2f GameObject::GetVelocity()
{
	return m_velocity;
}

void GameObject::SetVelocity(sf::Vector2f vec)
{
	m_velocity = vec;
}

void GameObject::SetVelocity(float x, float y)
{
	m_velocity = sf::Vector2f(x, y);
}

sf::Vector2f GameObject::GetOrigin() const
{
	return m_spr->GetOrigin();
}

void GameObject::SetPrevPosition(sf::Vector2f pos)
{
	m_prevPos = pos;
}

void GameObject::SetPrevPosition(float x, float y)
{
	m_prevPos = sf::Vector2f(x, y);
}

sf::Vector2f GameObject::GetPrevPostion()
{
	return m_prevPos;
}

bool GameObject::GetOnGround()
{
	return m_onGround;
}

void GameObject::SetOnGround(bool grnd)
{
	if (grnd)
	{
		m_onGround = true;
		m_falling = false;
	}
	else
	{
		m_onGround = false;
		m_falling = true;
	}
}

bool GameObject::GetAirbourne()
{
	return m_airbourne;
}

void GameObject::SetAirbourne(bool air)
{
	m_airbourne = air;
}

bool GameObject::GetFalling()
{
	return m_falling;
}

void GameObject::SetFalling(bool fall)
{
	m_falling = fall;
}

bool GameObject::GetDirection()
{
	return m_direction;
}

void GameObject::SetDirection(bool dir)
{
	m_direction = dir;
}

bool GameObject::GetVisible()
{
	return m_visible;
}

void GameObject::SetVisible(bool vis)
{
	m_prevVisibility = m_visible;
	m_visible = vis;
}

int GameObject::GetObjectNum()
{
	return m_objectID;
}

void GameObject::SetActive(bool act)
{
	m_active = act;
}

bool GameObject::GetActive()
{
	return m_active;
}

