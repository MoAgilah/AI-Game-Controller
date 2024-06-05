#ifndef RexH
#define RexH

#include "../GameObjects/Enemy.h"

class BoundingBox;
class Rex : public Enemy
{
public:
	Rex(bool dir, int initAnim, float animSpd, const sf::Vector2f& initPos);
	~Rex() final = default;

	void Update(float deltaTime) final;
	void Reset() final;
	void Die() final;

	bool Tall() const { return m_numLives == m_maxLives; }
	void Change();

private:
	void Animate(float deltaTime) final;
};

#endif