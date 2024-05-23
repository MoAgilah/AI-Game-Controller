#ifndef ChuckH
#define ChuckH

#include "../GameObjects/Enemy.h"

class Chuck :
	public Enemy
{
public:
	Chuck(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos);
	~Chuck();

	virtual void Reset() final;
	void Die();
	bool Jumping();
private:
	bool m_goingUp = false;
	float m_waitTime = 0;
	void Animate(float deltaTime);
};

#endif