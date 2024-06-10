#ifndef  GameObjectH
#define  GameObjectH

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"

enum GOTYPE
{
	PLAYER,
	KOOPA, REX, BILL, PPLANT,CHUCK,
	YCOIN, SHROOM, QBOX, SMBOX, SPBOX,
	CHKPOINT, GOAL,

	EnmyBgn = KOOPA, EnmyEnd = CHUCK,
	ObjBgn = YCOIN, ObjEnd = GOAL
};

struct SpawnData
{
	int m_initialAnim = 0;
	bool m_initialDir = true;
	sf::Vector2f m_initialPos;
};

class Camera;
class BoundingBox;
class GameObject
{
public:
	explicit GameObject(TexID boxId);
	GameObject(TexID sprId, TexID boxId);
	virtual ~GameObject() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window);

	virtual void Reset();

	sf::Sprite* GetSprite() { return m_spr->GetSprite(); }
	BoundingBox* GetBBox() { return m_bbox.get(); }

	sf::Vector2f GetPosition() const { return m_spr->GetPosition(); };
	void SetPosition(sf::Vector2f pos);
	void SetPosition(float x, float y);

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

	sf::Vector2f GetOrigin() const { return m_spr->GetOrigin(); }

	void SetPrevPosition(sf::Vector2f pos) { m_prevPos = pos; }
	void SetPrevPosition(float x, float y) { m_prevPos = sf::Vector2f(x, y); }
	sf::Vector2f GetPrevPostion() const { return m_prevPos; }

	bool GetOnGround() const { return m_onGround; }
	virtual void SetOnGround(bool grnd) { m_onGround = grnd; }

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air) { m_airbourne = air; }

	bool GetDirection() const { return m_direction; }
	void SetDirection(bool dir);

	int GetObjectNum() const { return m_objectID; }

	virtual bool GetActive() const { return m_visible; }
	void SetVisible(bool act) { m_visible = act; }

	TexID GetID() const { return (TexID)m_type; }

protected:
	int m_type = -1;
	int m_objectID;
	static int s_objectNum;

	SpawnData m_spawnData;

	bool m_visible = false;

	bool m_direction = true;
	bool m_onGround = false;
	bool m_airbourne = false;

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

	sf::Vector2f m_prevPos;

	std::shared_ptr<Sprite> m_spr;
	std::shared_ptr<BoundingBox> m_bbox;
};

class AnimatedGameObject : public GameObject
{
public:
	AnimatedGameObject(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd);
	AnimatedGameObject(TexID id, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd);
	~AnimatedGameObject() override = default;

	void Reset() override;

	AnimatedSprite* GetAnimSpr() { return static_cast<AnimatedSprite*>(m_spr.get()); }
};

#endif
