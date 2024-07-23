#pragma once

#include "../GameObjects/Object.h"
#include <SFML/Graphics.hpp>

class Enemy : public DynamicObject
{
public:
	Enemy(TexID sprId, const sf::Vector2f& boxSize, int maxLives = 1);
	Enemy(TexID sprId, const sf::Vector2f& boxSize, AnimationData animData, int maxLives = 1);
	~Enemy() override = default;

	void Update(float deltaTime) override;

	void Reset() override;

	bool GetOnGround() const { return m_onGround; }
	void SetOnGround(bool grnd) { m_onGround = grnd; }

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air) { m_airbourne = air; }

	float GetAirTime() const { return m_airtime; }
	void SetAirTime(float val) { m_airtime = val; }
	void IncAirTime(float val) { m_airtime += val; }

	bool GetIsAlive() const { return m_numLives > 0; }
	virtual void DecrementLife();
	virtual void Die() = 0;

	bool GetInvulnerabe() const { return m_invulnerable; }
	void SetInvulnerability(bool val) { m_invulnerable = val; }

	void SetTimeLeftActive(float time) { m_timeLeftActive = time; }

	void UpdateBoundingBox() override { GetAABB()->Update(sf::Vector2f(GetPosition().x, GetPosition().y + 3.5f)); };
private:
	virtual void Animate(float deltaTime) = 0;

	bool m_invulnerable = false;
	bool m_resetAllowed = false;
	bool m_onGround = false;
	bool m_airbourne = false;
	int m_numLives = 1;
	int m_maxLives;
	float m_airtime = 0;
	float m_tillReset = 0;
	float m_timeLeftActive = 0;
};
