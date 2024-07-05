#pragma once
#include "../GameObjects/Object.h"

class StaticCollectable : public Object
{
public:
	StaticCollectable(TexID sprID, TexID boxID, const sf::Vector2f& initPos);
	StaticCollectable(AnimatedSprite* sprite, TexID boxId, const sf::Vector2f& initPos);
	~StaticCollectable() override = default;
};

class Coin : public StaticCollectable
{
public:
	explicit Coin(const sf::Vector2f& initPos);
	~Coin() final = default;

	void Update(float deltaTime) final;

	int Collect();

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
};

class YCoin : public StaticCollectable
{
public:
	explicit YCoin(const sf::Vector2f& initPos);
	~YCoin() final = default;

	void Update(float deltaTime) final;

	void Reset() final;

	int Collect();

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
private:

	static int s_collected;
};

class CheckPoint : public StaticCollectable
{
public:
	explicit CheckPoint(const sf::Vector2f& initPos);
	~CheckPoint() final = default;

	void Update(float deltaTime) final;

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
};

class DynamicCollectable : public DynamicObject
{
public:
	DynamicCollectable(TexID sprID, TexID boxID, const sf::Vector2f& initPos);
	~DynamicCollectable() override = default;

	bool GetOnGround() const { return m_onGround; }
	void SetOnGround(bool grnd) { m_onGround = grnd; }

private:

	bool m_onGround = false;
};

class Mushroom : public DynamicCollectable
{
public:
	explicit Mushroom(const sf::Vector2f& initPos);
	~Mushroom() final = default;

	void Update(float deltaTime) final;

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
};

class Goal : public DynamicCollectable
{
public:
	explicit Goal(const sf::Vector2f& initPos);
	~Goal() final = default;

	void Update(float deltaTime) final;

	void ResolveCollisions(Object* other);
	void ResolveCollisions(Tile* tile);
};