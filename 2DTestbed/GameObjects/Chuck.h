#ifndef ChuckH
#define ChuckH

#include "../GameObjects/Enemy.h"

class Chuck :
	public Enemy
{
public:
	Chuck(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc = 1, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Chuck();
	void Die();
	bool Jumping();
private:
	bool goingUp;
	float m_waitTime;
	void Animate(float deltaTime);
};

#endif