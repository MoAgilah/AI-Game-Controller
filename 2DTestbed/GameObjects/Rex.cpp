#include "../GameObjects/Rex.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"

Rex::Rex(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
	:Enemy(filepath, rows, cols, fps, bTyp, strloc, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> frames{ 2, 2, 1 };
	m_spr->SetFrames(frames);

	m_type = REX;
	numLives = maxLives = 2;

	m_curBbox = m_bbox.get();

	m_SmlBox = new BoundingBox(filepath.substr(0, strloc) + "c", bTyp);
}

void Rex::Die()
{
	m_alive = false;
	m_spr->ChangeAnim(2);
	timeLeftActive = 0.5f;
}


void Rex::Change()
{
	m_spr->ChangeAnim(1);

	m_curBbox = m_SmlBox;

	//change bbox
	//smaller one
}

void Rex::Update(float deltaTime)
{
	if (m_active)
	{
		if (m_visible)
		{
			if (timeLeftActive > 0)
			{
				timeLeftActive -= deltaTime;
			}

			if (timeLeftActive < 0)
			{
				m_active = false;
			}

			if (resetAllowed)
			{
				resetAllowed = false;
			}
		}
		//if off screen
		else
		{
			//and wasn't previousily off screen
			if (m_visible != m_prevVisibility)
			{

				m_tillReset = 1;
				resetAllowed = true;
			}
		}


		if (resetAllowed)
		{
			m_tillReset -= deltaTime;
			if (m_tillReset <= 0)
			{
				if (!Camera::GetCamera()->IsInView(m_initialPos,GetOrigin()))
				{
					Reset();
				}
			}
		}


		if (m_visible)
		{
			if (m_alive)
			{
				Animate(deltaTime);
			}
		}

		m_spr->Update(deltaTime, m_direction);

		if (m_direction)
		{
			if (numLives == maxLives)
			{
				//+
				m_curBbox->Update(sf::Vector2f(m_spr->GetSpr()->getPosition().x - 2.f, m_spr->GetSpr()->getPosition().y));
			}
			else
			{
				//+
				m_curBbox->Update(sf::Vector2f(m_spr->GetSpr()->getPosition().x - 4.f, m_spr->GetSpr()->getPosition().y + 18.5f));
			}

		}
		else
		{
			if (numLives == maxLives)
			{
				//+
				m_curBbox->Update(sf::Vector2f(m_spr->GetSpr()->getPosition().x + 2.f, m_spr->GetSpr()->getPosition().y));
			}
			else
			{
				//-
				m_curBbox->Update(sf::Vector2f(m_spr->GetSpr()->getPosition().x + 4.f, m_spr->GetSpr()->getPosition().y + 18.5f));
			}
		}
	}
}

void Rex::Reset()
{
	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;

	numLives = maxLives;

	m_curBbox = m_bbox.get();
	m_spr->ChangeAnim(0);
}

void Rex::Revive()
{
	m_direction = m_initialDir;
	SetPosition(m_initialPos);
	m_prevPos = GetPosition();
	m_velocity = sf::Vector2f(0, 0);

	resetAllowed = false;
	m_onGround = false;
	m_falling = true;
	m_airbourne = false;
	m_visible = m_prevVisibility = false;

	m_tillReset = 0;
	timeLeftActive = 0;
	numLives = maxLives;

	m_curBbox = m_bbox.get();
	m_spr->ChangeAnim(m_initialAnim);
	m_alive = true;
	m_active = true;
}

Rex::~Rex()
{
}

void Rex::Animate(float deltaTime)
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
		m_velocity.y += m_gravity;
	}

	if (m_velocity.x != 0)
	{
		m_spr->Move(m_velocity.x * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}


	//check for leftmost and rightmost boundary
	if (m_spr->GetSpr()->getPosition().x < m_spr->GetSpr()->getOrigin().x || m_spr->GetSpr()->getPosition().x > 11776 - m_spr->GetSpr()->getOrigin().x)
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