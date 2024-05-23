#ifndef EnemyH
#define EnemyH

#include "../GameObjects/GameObject.h"

class Camera;
class Enemy : public GameObject
{
public:
	Enemy(std::string_view filepath, int rows, int cols, int bTyp, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Enemy();
	virtual void Update(float deltaTime);
	virtual void Render(sf::RenderWindow& window);
	int DecrementLife();
	void ResetLives();
	virtual void Die() = 0;
	bool GetIsAlive();
	virtual void Change();
	BoundingBox *GetBBox();

	int GetLives();
	virtual void Revive();
	int GetEnemyNum();
protected:
	virtual void Animate(float deltaTime) = 0;
	virtual void Reset();


	float m_tillReset = 0;
	bool m_resetAllowed = false;

	float m_airtime = 0;
	const float c_maxAirTime = 1.0f;
	bool m_crouched = false;

	int m_numLives = 1;
	int m_maxLives = m_numLives;
	bool m_alive = true;

	float timeLeftActive = 0;

	int m_enemyNum;
};

#endif