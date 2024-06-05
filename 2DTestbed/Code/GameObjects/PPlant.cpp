#include "../GameObjects/PPlant.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"


PPlant::PPlant(const sf::Vector2f& initPos)
	:Enemy(TexID::PPlant, 2, 2, (int)TexID::PPlantBB, false, false)
{
	m_spawnData.m_initialPos = initPos;
	SetPosition(m_spawnData.m_initialPos);

	std::vector<int> cframes{ 1, 2};
	m_spr->SetFrames(cframes);
}

void PPlant::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
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
		Collisions::Get()->ProcessCollisions(this);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y > 390)
	{
		SetDirection(false);
		m_spr->ChangeAnim(0);
		//set wait timer
		//go up
	}

	if (currentPos.y < 160)
	{
		SetDirection(true);
		m_spr->ChangeAnim(1);
		//go down
	}

}