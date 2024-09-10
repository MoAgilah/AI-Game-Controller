#pragma once

#include <SFML/Graphics.hpp>
#include "../Game/Timer.h"
#include "../GameObjects/Object.h"

class Enemy : public DynamicObject
{
public:
	Enemy(TexID sprId, const sf::Vector2f& boxSize, int maxLives = 1);
	Enemy(TexID sprId, const sf::Vector2f& boxSize, AnimationData animData, int maxLives = 1);
	~Enemy() override = default;

	bool GetActive() const final { return GetIsAlive() && Object::GetActive(); }

	void Update(float deltaTime) override;

	void Reset() override;

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air) { m_airbourne = air; }

	bool GetIsAlive() const { return m_isAlive; }
	bool HasLifes() const { return m_numLives > 0; }
	virtual void DecrementLife();
	virtual void Die() = 0;

	bool GetInvulnerabe() const { return m_invulnerable; }
	void SetInvulnerability(bool val) { m_invulnerable = val; }

	Timer* GetAirTimer() { return &m_airTimer; }
	void SetAirTime(float time);
	void SetTimeLeftActive(float time);

private:

	virtual void Animate(float deltaTime) = 0;

	bool m_isAlive = true;
	bool m_airbourne = false;
	bool m_invulnerable = false;
	bool m_resetAllowed = false;
	int m_numLives = 1;
	int m_maxLives;
	Timer m_airTimer;
	Timer m_resetTimer;
	Timer m_activationTimer;
};
