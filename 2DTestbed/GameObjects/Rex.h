#ifndef RexH
#define RexH

#include "../GameObjects/Enemy.h"

class BoundingBox;
class Rex :
	public Enemy
{
public:
	Rex(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc = 1, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	void Die();
	void Change();
	void Update(float deltaTime);
	void Reset();
	void Revive();
	~Rex() final = default;
private:
	void Animate(float deltaTime);
	bool onTheGround;
	BoundingBox* m_SmlBox;
};

#endif