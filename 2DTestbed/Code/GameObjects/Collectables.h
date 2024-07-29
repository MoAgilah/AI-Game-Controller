#pragma once
#include "../GameObjects/Object.h"

class Player;

class StaticCollectable : public Object
{
public:
	StaticCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	StaticCollectable(AnimatedSprite* sprite, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	~StaticCollectable() override = default;

	bool GetActive() const final { return !GetCollected() && Object::GetActive(); }

	void SetCollected() { m_collected = true; }
	bool GetCollected() const { return m_collected; }

	virtual void Collect(Player* player) = 0;

private:

	bool m_collected = false;
};

class Coin : public StaticCollectable
{
public:
	explicit Coin(const sf::Vector2f& initPos);
	~Coin() final = default;

	void Update(float deltaTime) final;

	void Collect(Player* player) final;
};

class YCoin : public StaticCollectable
{
public:
	explicit YCoin(const sf::Vector2f& initPos);
	~YCoin() final = default;

	void Update(float deltaTime) final;

	void Reset() final;

	void Collect(Player* player) final;

private:

	static int s_collected;
};

class CheckPoint : public StaticCollectable
{
public:
	explicit CheckPoint(const sf::Vector2f& initPos);
	~CheckPoint() final = default;
	AnimatedSprite* GetAnimSpr() = delete;

	void Update(float deltaTime) final;

	void Collect(Player* player) final;
};

class DynamicCollectable : public DynamicObject
{
public:
	DynamicCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	~DynamicCollectable() override = default;

	bool GetActive() const final { return !GetCollected() && Object::GetActive(); }

	void SetCollected() { m_collected = true; }
	bool GetCollected() const { return m_collected; }

	virtual void Collect(Player* player) = 0;

private:

	bool m_collected = false;
};

class Mushroom : public DynamicCollectable
{
public:
	explicit Mushroom(const sf::Vector2f& initPos);
	~Mushroom() final = default;
	AnimatedSprite* GetAnimSpr() = delete;

	void Update(float deltaTime) final;

	void Collect(Player* player) final;
};

class Goal : public DynamicCollectable
{
public:
	explicit Goal(const sf::Vector2f& initPos);
	~Goal() final = default;
	AnimatedSprite* GetAnimSpr() = delete;

	void Update(float deltaTime) final;

	void Collect(Player* player) final;

	float GetAirTime() const { return m_travelTime; }
	void SetAirTime(float val) { m_travelTime = val; }
	void IncAirTime(float val) { m_travelTime += val; }

private:

	float m_travelTime = 0;
	const float c_maxTravelTime = 2.15f;
};