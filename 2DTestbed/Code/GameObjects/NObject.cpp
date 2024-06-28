#include "NObject.h"
#include "../Collisions/Collisions.h"

Mushroom::Mushroom(bool dir, const sf::Vector2f& pos)
	: StaticObject(TexID::Shroom, (int)TexID::ShroomBB, dir, pos)
{
	SetInitialDirection(dir);
	SetDirection(GetInitialDirection());
}

void Mushroom::Update(float deltaTime)
{
}
