#ifndef  GameObjectH
#define  GameObjectH

#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../GameObjects/AnimatedSprite.h"

enum GOTYPE
{
	PLAYER,
	KOOPA, REX, BILL, PPLANT,CHUCK,
	YCOIN, SHROOM, QBOX, SMBOX, SPBOX,
	CHKPOINT, GOAL,

	EnmyBgn = KOOPA, EnmyEnd = CHUCK,
	ObjBgn = YCOIN, ObjEnd = GOAL
};

class Camera;
class BoundingBox;
class GameObject
{
public:
	GameObject(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd);
	virtual ~GameObject() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;

	virtual BoundingBox* GetBBox() { return m_bbox.get(); };
	virtual AnimatedSprite* GetAnimSpr() { return m_spr.get(); }

	virtual void SetInitialPos(sf::Vector2f pos) { m_initialPos = pos; }
	virtual sf::Vector2f GetPosition() const { return m_spr->GetSpr()->getPosition(); }
	virtual void SetPosition(sf::Vector2f pos) { m_spr->GetSpr()->setPosition(pos); }
	virtual void SetPosition(float x, float y) { m_spr->GetSpr()->setPosition((sf::Vector2f(x, y))); }

	sf::Vector2f GetVelocity() const { return m_velocity; }
	void SetVelocity(sf::Vector2f vec) { m_velocity = vec; }
	void SetVelocity(float x, float y) { m_velocity = sf::Vector2f(x, y); }

	sf::Vector2f GetOrigin() const { return m_spr->GetSpr()->getOrigin(); }

	sf::Vector2f GetPrevPostion() const { return m_prevPos; }
	void SetPrevPosition(sf::Vector2f pos) { m_prevPos = pos; }
	void SetPrevPosition(float x, float y) { m_prevPos = sf::Vector2f(x, y); }

	bool GetOnGround() const { return m_onGround; }
	virtual void SetOnGround(bool grnd);

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air) { m_airbourne = air; }

	bool GetFalling() const { return m_falling; }
	void SetFalling(bool fall) { m_falling = fall; }

	bool GetDirection() const { return m_direction; }
	void SetDirection(bool dir) { m_direction = dir; }

	bool GetVisible() const { return m_visible; }
	void SetVisible(bool vis)
	{
		m_prevVisibility = m_visible;
		m_visible = vis;
	}

	int GetObjectNum() const { return m_objectID; };

	bool GetActive() const { return m_active; }
	void SetActive(bool act) { m_active = act; }

protected:
	int m_objectID;
	int m_initialAnim;
	int m_type;
	static int s_objectNum;
	static int s_numOfEnemies;
	static int s_numOfObjects;

	bool m_active = true;
	bool m_prevVisibility = false;
	bool m_visible = false;
	bool m_direction;
	bool m_onGround = false;
	bool m_falling = true;
	bool m_airbourne = false;
	bool m_initialDir;

	const float m_gravity = 0.981f;

	int m_state = 0;
	BoundingBox* m_curBbox;
	AnimatedSprite* m_currSpr;

	std::unique_ptr<AnimatedSprite> m_spr;
	std::unique_ptr<BoundingBox> m_bbox;

	sf::Vector2f m_position;
	sf::Vector2f m_prevPos;
	sf::Vector2f m_velocity;
	sf::Vector2f m_initialPos;
};

#endif
