#pragma once

#include "../GameObjects/Enemy.h"

class PPlant :
	public Enemy
{
public:
	PPlant(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc = 1, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	void Die();
	~PPlant() final = default;
private:
	void Animate(float deltaTime);
};