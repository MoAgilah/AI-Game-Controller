#include "../GameObjects/PPlant.h"
#include "../Collisions/Collisions.h"
#include "../Game/Constants.h"


PPlant::PPlant(const sf::Vector2f& initPos)
	:Enemy(TexID::PPlant, 2, 2, (int)TexID::PPlantBB, false, false)
{
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());

	std::vector<int> cframes{ 1, 2};
	GetAnimSpr()->SetFrames(cframes);
}

void PPlant::Animate(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetDirection())
	{
		SetYVelocity(c_jumpSpeed * 0.5f);
	}
	else
	{
		SetYVelocity(-c_jumpSpeed);
	}

	if (GetYVelocity() != 0)
	{
		GetAnimSpr()->Move(0, GetYVelocity() * FPS * deltaTime);
		Collisions::Get()->ProcessCollisions(this);
	}

	sf::Vector2f currentPos = GetPosition();

	if (currentPos.y > 390)
	{
		SetDirection(false);
		GetAnimSpr()->ChangeAnim(0);
		//set wait timer
		//go up
	}

	if (currentPos.y < 160)
	{
		SetDirection(true);
		GetAnimSpr()->ChangeAnim(1);
		//go down
	}

}