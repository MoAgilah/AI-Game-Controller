#include "../GameObjects/GameObject.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"

int GameObject::m_objectNum = 0;
int GameObject::s_numOfEnemies = 0;
int GameObject::s_numOfObjects = 0;

GameObject::GameObject(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
{
	m_spr = new AnimatedSprite(filepath,rows,cols,fps,strloc,symmetrical,initAnim,animSpd);

	//extract id for bounding box
	std::string s_id = filepath.substr(0, strloc);

	m_bbox = new BoundingBox(s_id, bTyp);

	initialDir = m_direction = dir;

	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	visible = prevVisibility = false;

	Collisions::Get()->AddCollidable(this);
	objectID = m_objectNum++;
	m_active = true;
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

sf::Sprite GameObject::GetSprite()
{
	return *m_spr->GetSpr();
}

BoundingBox * GameObject::GetBBox()
{
	return m_bbox;
}

void GameObject::SetInitialPos(sf::Vector2f pos)
{
	initialPos = pos;
}

sf::Vector2f GameObject::GetPosition()
{
	return m_spr->GetSpr()->getPosition();
}

void GameObject::SetPosition(sf::Vector2f pos)
{
	m_spr->GetSpr()->setPosition(pos);
}

void GameObject::SetPosition(float x, float y)
{
	m_spr->GetSpr()->setPosition((sf::Vector2f(x, y)));
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

sf::Vector2f GameObject::GetOrigin()
{	
	return m_spr->GetSpr()->getOrigin();
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
	return visible;
}

void GameObject::SetVisible(bool vis)
{
	prevVisibility = visible;
	visible = vis;
}

int GameObject::GetObjectNum()
{
	return objectID;
}

void GameObject::SetActive(bool act)
{
	m_active = act;
}

bool GameObject::GetActive()
{
	return m_active;
}

