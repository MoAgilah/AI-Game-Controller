#include "../GameObjects/PPlant.h"
#include "../../Collisions/Collisions.h"



PPlant::PPlant(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
	:Enemy(filepath, rows, cols, fps, bTyp, strloc, dir, symmetrical, initAnim, animSpd)
{
	std::vector<int> cframes{ 1, 2};
	m_spr->SetFrames(cframes);
	m_type = PPLANT;
	numLives = maxLives = 1;
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
		m_spr->Move(0, m_velocity.y * FPS * deltaTime);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y > 390)
	{
		m_direction = false;
		m_spr->ChangeAnim(0);
		//set wait timer
		//go up
	}

	if (currentPos.y < 160)
	{
		m_direction = true;
		m_spr->ChangeAnim(1);
		//go down
	}
	
}