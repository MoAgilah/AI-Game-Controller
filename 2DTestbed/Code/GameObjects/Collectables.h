#pragma once
#include "../GameObjects/Object.h"

class StaticCollectable : public Object
{
public:
	StaticCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	StaticCollectable(AnimatedSprite* sprite, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	~StaticCollectable() override = default;

	bool GetActive() const final { return !m_collected && Object::GetActive(); }

	void SetCollected() { m_collected = true; }
private:
	bool m_collected = false;
};

class Coin : public StaticCollectable
{
public:
	explicit Coin(const sf::Vector2f& initPos);
	~Coin() final = default;

	void Update(float deltaTime) final;

	int Collect();
};

class YCoin : public StaticCollectable
{
public:
	explicit YCoin(const sf::Vector2f& initPos);
	~YCoin() final = default;

	void Update(float deltaTime) final;

	void Reset() final;

	int Collect();

private:

	static int s_collected;
};

class CheckPoint : public StaticCollectable
{
public:
	explicit CheckPoint(const sf::Vector2f& initPos);
	~CheckPoint() final = default;

	void Update(float deltaTime) final;

	AnimatedSprite* GetAnimSpr() = delete;

};

class DynamicCollectable : public DynamicObject
{
public:
	DynamicCollectable(TexID sprID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	~DynamicCollectable() override = default;

	bool GetActive() const final { return !m_collected && Object::GetActive(); }

	void SetCollected() { m_collected = true; }
private:

	bool m_collected = false;
};

class Mushroom : public DynamicCollectable
{
public:
	explicit Mushroom(const sf::Vector2f& initPos);
	~Mushroom() final = default;

	void Update(float deltaTime) final;

	AnimatedSprite* GetAnimSpr() = delete;

};

class Goal : public DynamicCollectable
{
public:
	explicit Goal(const sf::Vector2f& initPos);
	~Goal() final = default;

	void Update(float deltaTime) final;

	AnimatedSprite* GetAnimSpr() = delete;

	float GetAirTime() const { return m_travelTime; }
	void SetAirTime(float val) { m_travelTime = val; }
	void IncAirTime(float val) { m_travelTime += val; }

private:

	float m_travelTime = 0;
	const float c_maxTravelTime = 2.15f;
};