#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"

Object::Object(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:GameObject(id, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
{
	m_spawnData.m_initialPos = initPos;
	SetPosition(m_spawnData.m_initialPos);

	isAnimating = false;
	m_goingUp = false;
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
		if (isAnimating && this->GetBBox()->GetID() == (int)TexID::Box)
		{
			if (m_curSpr->PlayedNumTimes(2))
			{
				isAnimating = false;
				m_curSpr->ChangeAnim(0);
			}
		}

		//Animate(deltaTime);

		m_curSpr->Update(deltaTime, m_direction);

		if (m_direction)
		{
			//+
			m_curBBox->Update(sf::Vector2f(m_curSpr->GetPosition().x - 2, m_curSpr->GetPosition().y));
		}
		else
		{
			//-
			m_bbox->Update(sf::Vector2f(m_curSpr->GetPosition().x + 2, m_curSpr->GetPosition().y));
		}
	}
}

void Object::Render(sf::RenderWindow & window)
{
	window.draw(*m_curSpr->GetSprite());
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
			m_velocity.y += gravity;
		}

		if (m_velocity.x != 0)
		{
			m_curSpr->Move(m_velocity.x * FPS * deltaTime, 0);
			Collisions::Get()->ProcessCollisions(this);
		}

		//check for leftmost and rightmost boundary
		if (m_curSpr->GetPosition().x < m_curSpr->GetOrigin().x || m_curSpr->GetPosition().x > 11776 - m_curSpr->GetOrigin().x)
		{
			m_curSpr->Move(-m_velocity.x * FPS * deltaTime, 0);
			m_direction = !m_direction;
		}

		if (m_velocity.y != 0)
		{
			m_curSpr->Move(0, m_velocity.y * FPS * deltaTime);
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
			m_curSpr->Move(0, m_velocity.y * FPS * deltaTime);
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

void Object::Reset()
{
	isAnimating = false;
	m_goingUp = false;
	m_curSpr->ChangeAnim(m_spawnData.m_initialAnim);
	m_active = true;
}
