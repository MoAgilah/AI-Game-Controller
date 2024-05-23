#ifndef ChuckH
#define ChuckH

#include "../GameObjects/Enemy.h"

class Chuck :
	public Enemy
{
public:
	Chuck(bool dir, int initAnim, float animSpd);
	~Chuck();
	void Die();
	bool Jumping();
private:
	bool m_goingUp;
	float m_waitTime;
	void Animate(float deltaTime);
};

#endif