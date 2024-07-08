#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"
#include "../Collisions/BoundingBox.h"
#include "../Game/Constants.h"

struct SpawnData
{
	bool initialDir;
	sf::Vector2f initialPos;
};

struct AnimationData
{
	int rows;
	int cols;
	bool symmetrical;
	float animationSpeed;
};

enum GOTYPE
{
	PlyBgn = (int)TexID::Mario, PlyEnd = (int)TexID::SuperBB,
	EnmyBgn = (int)TexID::Koopa, EnmyEnd = (int)TexID::ChuckBB,
	ColBgn = (int)TexID::Coin, ColEnd = (int)TexID::GoalBB,
	ObjBgn = (int)TexID::QBox, ObjEnd = (int)TexID::BoxBB
};

class Tile;
class Object
{
public:
	Object(TexID sprId, TexID boxId);
	Object(AnimatedSprite* sprite, TexID boxId);
	virtual ~Object() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window);

	virtual void Reset();

	int GetObjectNum() const { return m_objectID; }

	Sprite* GetSprite() { return m_sprite.get(); }
	BoundingBox* GetBBox() { return m_bbox.get(); }

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

	virtual void ResolveCollisions(Object* other) = 0;
	virtual void ResolveCollisions(Tile* tile) = 0;
private:

	TexID m_type = TexID::None;
	bool m_active = false;
	bool m_direction = true;
	int m_objectID = 0;
	static int s_objectNum;
	SpawnData m_spawnData;
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<BoundingBox> m_bbox;
};

class DynamicObject : public Object
{
public:
	DynamicObject(TexID sprId, TexID boxId);
	DynamicObject(AnimatedSprite* sprite, TexID boxId);
	~DynamicObject() override = default;

	void SetPrevPosition(sf::Vector2f pos) { m_previousPos = pos; }
	void SetPrevPosition(float x, float y) { m_previousPos = sf::Vector2f(x, y); }
	sf::Vector2f GetPrevPostion() const { return m_previousPos; }

	sf::Vector2f GetVelocity() const { return m_velocity; }
	void SetVelocity(sf::Vector2f vel) { m_velocity = vel; }
	void SetVelocity(float x, float y) { m_velocity = sf::Vector2f(x, y); }

	float GetXVelocity() const { return m_velocity.x; }
	void SetXVelocity(float x) { m_velocity.x = x; }
	void IncrementXVelocity(float x) { m_velocity.x += x; }
	void DecrementXVelocity(float x) { m_velocity.x -= x; }

	float GetYVelocity() const { return m_velocity.y; }
	void SetYVelocity(float y) { m_velocity.y = y; }
	void IncrementYVelocity(float y) { m_velocity.y += y; }
	void DecrementYVelocity(float y) { m_velocity.y -= y; }

	void Move(float x, float y);
	void Move(const sf::Vector2f& pos);

	virtual void UpdateBoundingBox() {}
private:

	sf::Vector2f m_velocity;
	sf::Vector2f m_previousPos;
};