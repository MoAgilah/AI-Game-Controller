#ifndef KoopaH
#define KoopaH

#include "../GameObjects/Enemy.h"

class Koopa :
	public Enemy
{
public:
	Koopa(int rows, int cols, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	void Die();
	~Koopa();
private:
	void Animate(float deltaTime);
};

#endif