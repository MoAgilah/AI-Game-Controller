#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"

Object::Object(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:AnimatedGameObject(id, rows, cols, bTyp, dir, symmetrical, initAnim, animSpd)
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
			if (GetAnimSpr()->PlayedNumTimes(2))
			{
				m_isAnimating = false;
				GetAnimSpr()->ChangeAnim(0);
			}
		}

		Animate(deltaTime);

		GetAnimSpr()->Update(deltaTime);

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
	GetAnimSpr()->ChangeAnim(m_spawnData.m_initialAnim);
	SetActive(false);
}

void Object::Animate(float deltaTime)
{
	if (this->GetBBox()->GetID() == (int)TexID::ShroomBB)
	{
		SetPrevPosition(GetPosition());

		if (GetDirection())
		{
			SetXVelocity(2);
		}
		else
		{
			SetXVelocity(-2);
		}

		if (GetOnGround())
		{
			SetYVelocity(0);
			m_airtime = 0;
		}
		else
		{
			IncrementYVelocity(c_gravity);
		}

		if (GetXVelocity() != 0)
		{
			m_spr->Move(GetXVelocity() * FPS * deltaTime, 0);
			Collisions::Get()->ProcessCollisions(this);
		}

		//check for leftmost and rightmost boundary
		if (m_spr->GetPosition().x < m_spr->GetOrigin().x || m_spr->GetPosition().x > 11776 - m_spr->GetOrigin().x)
		{
			m_spr->Move(-GetXVelocity() * FPS * deltaTime, 0);
			SetDirection(!GetDirection());
		}

		if (GetYVelocity() != 0)
		{
			m_spr->Move(0, GetYVelocity() * FPS * deltaTime);
			Collisions::Get()->ProcessCollisions(this);
		}
	}

	if (this->GetBBox()->GetID() == (int)TexID::GoalBB)
	{
		if (m_goingUp)
		{
			SetYVelocity(2.5);
		}
		else
		{
			SetYVelocity(-2.5);
		}

		if (GetYVelocity() != 0)
		{
			m_spr->Move(0, GetYVelocity() * FPS * deltaTime);
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


