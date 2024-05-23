#include "../GameObjects/PPlant.h"
#include "../../Collisions/Collisions.h"



PPlant::PPlant(int initAnim, float animSpd)
	:Enemy("pplant.png", 2, 2, PPLANT, false, false)
{
	std::vector<int> cframes{ 1, 2};
	m_curSpr->SetFrames(cframes);
}

void PPlant::Die()
{
}


PPlant::~PPlant()
{
}

void PPlant::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (m_direction)
	{
		m_velocity.y = 1.5;
	}
	else
	{
		m_velocity.y = -3;
	}

	if (m_velocity.y != 0)
	{
		m_curSpr->Move(0, m_velocity.y * FPS * deltaTime);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y > 390)
	{
		m_direction = false;
		m_curSpr->ChangeAnim(0);
		//set wait timer
		//go up
	}

	if (currentPos.y < 160)
	{
		m_direction = true;
		m_curSpr->ChangeAnim(1);
		//go down
	}

}