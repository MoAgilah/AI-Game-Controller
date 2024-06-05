#ifndef RexH
#define RexH

#include "../GameObjects/Enemy.h"

class BoundingBox;
class Rex : public Enemy
{
public:
	Rex(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos);
	~Rex() final = default;



	bool Tall() const { return m_numLives == m_maxLives; }

	void Die();
	void Change();
	void Update(float deltaTime);

private:
	void Animate(float deltaTime);

	BoundingBox* m_SmlBox;
};

#endif