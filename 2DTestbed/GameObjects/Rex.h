#ifndef RexH
#define RexH

#include "../GameObjects/Enemy.h"

class BoundingBox;
class Rex : public Enemy
{
public:
	Rex(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos);
	void Die();
	void Change();
	void Update(float deltaTime);

	~Rex();
private:
	void Animate(float deltaTime);

	BoundingBox* m_SmlBox;
};

#endif