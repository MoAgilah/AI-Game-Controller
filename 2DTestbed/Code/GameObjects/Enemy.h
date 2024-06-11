#pragma once

#include "../GameObjects/GameObject.h"

class Camera;
class Enemy : public AnimatedGameObject
{
public:
	Enemy(TexID id, int rows, int cols, int bTyp, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Enemy() override = default;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;
	void Reset() override;
	virtual void Die() {};

	bool GetIsAlive() const { return m_numLives > 0; }
	bool GetActive() const final { return GameObject::GetActive() && GetIsAlive(); }
	void DecrementLife();

protected:
	virtual void Animate(float deltaTime) = 0;

	bool m_crouched = false;
	bool m_resetAllowed = false;
	int m_numLives = 1;
	int m_maxLives = m_numLives;
	float m_airtime = 0;
	float m_tillReset = 0;
	float m_timeLeftActive = 0;
};