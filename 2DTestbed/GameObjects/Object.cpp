#include "../GameObjects/Object.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"

Object::Object(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
	:GameObject(filepath, rows, cols, fps, bTyp, strloc, dir, symmetrical, initAnim, animSpd)
{
	isAnimating = false;
	goingUp = false;
	initialAnim = initAnim;
	m_objectNum = s_numOfObjects++;
}

Object::~Object()
{
}

void Object::Initialise(bool hasAnim, float moveSpd, float jmpSpeed)
{
	m_hasAnim = hasAnim;
	m_moveSpeed = moveSpd;
	m_jumpSpeed = jmpSpeed;
}

void Object::Update(float deltaTime)
{
	if (m_active)
	{
			if (isAnimating && this->GetBBox()->GetID() == SPBOX)
			{
				if (m_spr->PlayedNumTimes(2))
				{
					isAnimating = false;
					m_spr->ChangeAnim(0);
				}
			}

			if (visible)
			{
				Animate(deltaTime);
			}

			m_spr->Update(deltaTime, m_direction);

			if (m_direction)
			{
				//+
				m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x - 2, m_spr->GetPosition().y));
			}
			else
			{
				//-
				m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x + 2, m_spr->GetPosition().y));
			}
	}
}

void Object::Render(sf::RenderWindow & window)
{
	window.draw(*m_spr->GetSprite());
}

void Object::SetIsAnimated(bool isAnim)
{
	isAnimating = isAnim;
}

bool Object::IsAnimated()
{
	return isAnimating;
}

void Object::Animate(float deltaTime)
{
	if (this->GetBBox()->GetID() == SHROOM)
	{
		SetPrevPosition(GetPosition());
	
		if (m_direction)
		{
			m_velocity.x = 2;
		}
		else
		{
			m_velocity.x = -2;
		}
	
		if (m_onGround)
		{
			m_velocity.y = 0;
			m_airtime = 0;
		}
		else
		{
			m_falling = true;
			m_velocity.y += gravity;
		}
	
		if (m_velocity.x != 0)
		{
			m_spr->Move(m_velocity.x * FPS * deltaTime, 0);
			Collisions::Get()->ProcessCollisions(this);
		}
	
		//check for leftmost and rightmost boundary
		if (m_spr->GetPosition().x < m_spr->GetOrigin().x || m_spr->GetPosition().x > 11776 - m_spr->GetOrigin().x)
		{
			m_spr->Move(-m_velocity.x * FPS * deltaTime, 0);
			m_direction = !m_direction;
		}
	
		if (m_velocity.y != 0)
		{
			m_spr->Move(0, m_velocity.y * FPS * deltaTime);
			Collisions::Get()->ProcessCollisions(this);
		}
	}

	if (this->GetBBox()->GetID() == GOAL)
	{
		if (goingUp)
		{
			m_velocity.y = 2.5;
		}
		else
		{
			m_velocity.y = -2.5;
		}

		if (m_velocity.y != 0)
		{
			m_spr->Move(0, m_velocity.y * FPS * deltaTime);
		}

		sf::Vector2f currentPos = GetPosition();
		if (currentPos.y > 470)
		{
			goingUp = false;
		}

		if (currentPos.y < 150)
		{
			goingUp = true;
		}

		Collisions::Get()->ProcessCollisions(this);
	}
}

void Object::SetPosition(sf::Vector2f pos)
{
	m_spr->SetPosition(pos);
	if (m_direction)
	{
		//+
		m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x-2, m_spr->GetPosition().y));
	}
	else
	{
		//-
		m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x+2, m_spr->GetPosition().y));
	}
}

void Object::Reset()
{
	isAnimating = false;
	goingUp = false;
	m_spr->ChangeAnim(initialAnim);
	m_active = true;
}

int Object::GetObjectNum()
{
	return m_objectNum;
}
