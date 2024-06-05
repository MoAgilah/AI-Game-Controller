#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"

Object::Object(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:GameObject(id, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
{
	m_spawnData.m_initialPos = initPos;
	SetPosition(m_spawnData.m_initialPos);
}

void Object::Update(float deltaTime)
{
	if (GetActive())
	{
		if (m_isAnimating && this->GetBBox()->GetID() == (int)TexID::Box)
		{
			if (m_spr->PlayedNumTimes(2))
			{
				m_isAnimating = false;
				m_spr->ChangeAnim(0);
			}
		}

		Animate(deltaTime);

		m_spr->Update(deltaTime);

		if (GetDirection())
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

void Object::Reset()
{
	m_isAnimating = false;
	m_goingUp = false;
	m_spr->ChangeAnim(m_spawnData.m_initialAnim);
	m_visible = false;
}

void Object::Animate(float deltaTime)
{
	if (this->GetBBox()->GetID() == SHROOM)
	{
		SetPrevPosition(GetPosition());

		if (GetDirection())
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
			SetDirection(!m_direction);
		}

		if (m_velocity.y != 0)
		{
			m_spr->Move(0, m_velocity.y * FPS * deltaTime);
			Collisions::Get()->ProcessCollisions(this);
		}
	}

	if (this->GetBBox()->GetID() == GOAL)
	{
		if (m_goingUp)
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
			m_goingUp = false;
		}

		if (currentPos.y < 150)
		{
			m_goingUp = true;
		}

		Collisions::Get()->ProcessCollisions(this);
	}
}


