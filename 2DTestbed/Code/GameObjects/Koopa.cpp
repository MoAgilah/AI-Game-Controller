#include "../GameObjects/Koopa.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"


Koopa::Koopa(int rows, int cols, bool dir, bool symmetrical, int initAnim, float animSpd, const sf::Vector2f& initPos)
	:Enemy(TexID::Koopa, rows, cols, (int)TexID::KoopaBB, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> frames{ 1, 2, 1};
	GetAnimSpr()->SetFrames(frames);

	SetInitialPosition(sf::Vector2f(400, 524));
	GetAnimSpr()->SetPosition(GetInitialPosition());
	SetID(TexID::Koopa);
	m_numLives = m_maxLives = 1;
}

void Koopa::Die()
{
	GetAnimSpr()->ChangeAnim(2);
	m_timeLeftActive = 0.5f;
}

void Koopa::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetXVelocity(c_moveSpeed);
	}
	else
	{
		SetXVelocity(-c_moveSpeed);
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
		GetAnimSpr()->Move(GetXVelocity() * FPS * deltaTime, 0);
		Collisions::Get()->ProcessCollisions(this);
	}

	//check for leftmost and rightmost boundary
	if (GetAnimSpr()->GetPosition().x < GetAnimSpr()->GetOrigin().x || GetAnimSpr()->GetPosition().x > 11776 - GetAnimSpr()->GetOrigin().x)
	{
		GetAnimSpr()->Move(-GetXVelocity() * FPS * deltaTime, 0);
		SetDirection(!GetDirection());
	}

	if (GetYVelocity() != 0)
	{
		GetAnimSpr()->Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}
}