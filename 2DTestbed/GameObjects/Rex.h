#ifndef RexH
#define RexH

#include "../GameObjects/Enemy.h"

class BoundingBox;
class Rex :
	public Enemy
{
public:
	Rex(bool dir = true, int initAnim = 0, float animSpd = 1);
	void Die();
	void Change();
	void Update(float deltaTime);
	void Reset();
	void Revive();
	~Rex();
private:
	void Animate(float deltaTime);
	bool onTheGround;
	BoundingBox* m_SmlBox;
};

#endif