#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

#include "../Collisions/AABB.h"
#include "../Drawables/Sprite.h"
#include "../Game/Constants.h"
#include "../Game/PhysicsController.h"

struct SpawnData
{
	bool initialDir = true;
	sf::Vector2f initialPos;
};

struct AnimationData
{
	int rows;
	int cols;
	bool symmetrical;
	float animationSpeed;
};

class Tile;
class Object
{
public:
	Object(TexID sprId, const sf::Vector2f& boxSize);
	Object(AnimatedSprite* sprite, const sf::Vector2f& boxSize);
	virtual ~Object() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window);

	virtual bool Intersects(Object* obj);

	virtual void Reset();

	int GetObjectNum() const { return m_objectID; }

	AnimatedSprite* GetAnimSpr() { return static_cast<AnimatedSprite*>(GetSprite()); }
	Sprite* GetSprite() { return m_sprite.get(); }
	AABB* GetAABB() { return m_aabb.get(); }

	TexID GetID() const { return m_type; }
	void SetID(TexID id) { m_type = id; }

	virtual bool GetActive() const { return m_active; }
	void SetActive(bool act) { m_active = act; }

	bool GetDirection() const { return m_direction; }
	void SetDirection(bool dir);

	const sf::Vector2f& GetPosition() const { return m_sprite->GetPosition(); };
	void SetPosition(const sf::Vector2f& pos) { m_sprite->SetPosition(pos); }
	void SetPosition(float x, float y) { m_sprite->SetPosition(sf::Vector2f(x, y)); }

	sf::Vector2f GetOrigin() const { return m_sprite->GetOrigin(); }

	bool GetInitialDirection() const { return m_spawnData.initialDir; };
	void SetInitialDirection(bool dir) { m_spawnData.initialDir = dir; }

	const sf::Vector2f& GetInitialPosition() const { return m_spawnData.initialPos; }
	void SetInitialPosition(const sf::Vector2f& pos) { m_spawnData.initialPos = pos; }

private:

	TexID m_type = TexID::None;
	bool m_active = false;
	bool m_direction = true;
	int m_objectID = 0;
	static int s_objectNum;
	SpawnData m_spawnData;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<AABB> m_aabb;
};

class DynamicObject : public Object
{
public:
	DynamicObject(TexID sprId, const sf::Vector2f& boxSize);
	DynamicObject(AnimatedSprite* sprite, const sf::Vector2f& boxSize);
	~DynamicObject() override = default;

	void Reset() override;

	void SetPrevPosition(sf::Vector2f pos) { m_previousPos = pos; }
	void SetPrevPosition(float x, float y) { m_previousPos = sf::Vector2f(x, y); }
	sf::Vector2f GetPrevPosition() const { return m_previousPos; }

	sf::Vector2f GetVelocity() const { return m_velocity; }
	void SetVelocity(sf::Vector2f vel) { m_velocity = vel; }
	void SetVelocity(float x, float y) { m_velocity = sf::Vector2f(x, y); }

	float GetXVelocity() const { return m_velocity.x; }
	void SetXVelocity(float x) { m_velocity.x = x; }
	void IncrementXVelocity(float x);
	void DecrementXVelocity(float x);

	float GetYVelocity() const { return m_velocity.y; }
	void SetYVelocity(float y) { m_velocity.y = y; }
	void IncrementYVelocity(float y);
	void DecrementYVelocity(float y);

	bool GetOnGround() const { return m_onGround; }
	void SetOnGround(bool grnd) { m_onGround = grnd; }

	void Move(float x, float y);
	void Move(const sf::Vector2f& pos);

	void CheckForHorizontalBounds(float deltaTime);

	PhysicsController* GetPhysicsController() { return m_physicsCtrl.get(); }

private:

	bool m_onGround = false;
	sf::Vector2f m_velocity;
	sf::Vector2f m_previousPos;
	std::shared_ptr<PhysicsController> m_physicsCtrl;
};