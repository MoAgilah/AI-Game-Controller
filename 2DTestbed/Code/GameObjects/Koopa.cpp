#include "../GameObjects/Koopa.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"


Koopa::Koopa(int rows, int cols, bool dir, bool symmetrical, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:Enemy(TexID::Koopa, rows, cols, (int)TexID::KoopaBB, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> frames{ 1, 2, 1};
	m_spr->SetFrames(frames);

	m_spawnData.m_initialPos = sf::Vector2f(400, 524);
	m_spr->SetPosition(m_spawnData.m_initialPos);
	m_type = KOOPA;
	m_numLives = m_maxLives = 1;
}

void Koopa::Die()
{
	m_spr->ChangeAnim(2);
	m_timeLeftActive = 0.5f;
}

void Koopa::Animate(float deltaTime)
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
		SetYVelocity(c_gravity);
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