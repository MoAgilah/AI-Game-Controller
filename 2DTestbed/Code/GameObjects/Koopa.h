#pragma once
#include "../GameObjects/Enemy.h"

class Koopa :
	public Enemy
{
public:
	Koopa(int rows, int cols, bool dir = true, bool symmetrical = true, float animSpd = 1, const sf::Vector2f & initPos = sf::Vector2f());
	~Koopa() final = default;
	void Die() final;

private:
	void Animate(float deltaTime) final;
};