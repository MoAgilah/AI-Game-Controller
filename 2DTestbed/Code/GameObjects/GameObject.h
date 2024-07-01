#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"
#include "../Collisions/BoundingBox.h"
#include "../Game/Constants.h"

enum GOTYPE
{
	PlyBgn = (int)TexID::Mario, PlyEnd = (int)TexID::SuperBB,
	EnmyBgn = (int)TexID::Koopa, EnmyEnd = (int)TexID::ChuckBB,
	ColBgn = (int)TexID::YCoin, ColEnd = (int)TexID::GoalBB,
	ObjBgn = (int)TexID::QBox, ObjEnd = (int)TexID::BoxBB
};

class GameObject
{
public:
	GameObject(TexID sprId, TexID boxId);
	GameObject(AnimatedSprite* sprite, TexID boxId);
	virtual ~GameObject() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window);

	virtual void Reset();

	int GetObjectNum() const { return m_objectID; }

	Sprite* GetSprite() { return m_spr.get(); }
	BoundingBox* GetBBox() { return m_bbox.get(); }

	TexID GetID() const { return (TexID)m_type; }
	void SetID(TexID id) { m_type = (int)id; }

	virtual bool GetActive() const { return m_visible; }
	void SetActive(bool act) { m_visible = act; }

	bool GetDirection() const { return m_direction; }
	void SetDirection(bool dir);

	bool GetInitialDirection() const { return m_initialDir; };
	void SetInitialDirection(bool dir) { m_initialDir = dir; }

	bool GetOnGround() const { return m_onGround; }
	void SetOnGround(bool grnd) { m_onGround = grnd; }

	void Move(float x, float y);
	void Move(const sf::Vector2f& pos);

	const sf::Vector2f& GetPosition() const { return m_spr->GetPosition(); };
	void SetPosition(const sf::Vector2f& pos);
	void SetPosition(float x, float y);

	void SetPrevPosition(sf::Vector2f pos) { m_previousPos = pos; }
	void SetPrevPosition(float x, float y) { m_previousPos = sf::Vector2f(x, y); }
	sf::Vector2f GetPrevPostion() const { return m_previousPos; }

	const sf::Vector2f& GetInitialPosition() const { return m_initialPos; }
	void SetInitialPosition(const sf::Vector2f& pos) { m_initialPos = pos; }

	sf::Vector2f GetOrigin() const { return m_spr->GetOrigin(); }

private:

	int m_type = -1;
	int m_objectID;
	static int s_objectNum;
	bool m_visible = false;
	bool m_onGround = false;
	bool m_direction = true;
	bool m_initialDir = m_direction;
	sf::Vector2f m_initialPos;
	sf::Vector2f m_previousPos;
	std::shared_ptr<Sprite> m_spr;
	std::shared_ptr<BoundingBox> m_bbox;
};


class StaticObject : public GameObject
{
public:
	StaticObject(TexID id, int bTyp, bool dir, const sf::Vector2f& pos);
	~StaticObject() override = default;

	void Reset() override;
};

struct Cells
{
	int m_rows;
	int m_cols;

	Cells(int rows, int cols)
		: m_rows(rows), m_cols(cols) {}
};

class AnimatedObject : public GameObject
{
public:
	AnimatedObject(TexID id, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd);
	AnimatedObject(TexID id, int bTyp, bool dir, Cells cell, bool symmetrical, int initAnim, float animSpd);
	~AnimatedObject() override = default;

	void Reset() override;

	AnimatedSprite* GetAnimSpr() { return static_cast<AnimatedSprite*>(GetSprite()); }

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air) { m_airbourne = air; }

	int GetInitialAnim() const { return m_initialAnim; }
	void SetInitialAnim(int initAnim) { m_initialAnim = initAnim; }

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
private:

	bool m_airbourne = false;
	int m_initialAnim;
	sf::Vector2f m_prevPos;
	sf::Vector2f m_velocity;
};
