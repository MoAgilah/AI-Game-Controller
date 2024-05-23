#ifndef  GameObjectH
#define  GameObjectH

#include <string>
#include <SFML/Graphics.hpp>
#include "Sprite.h"

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
	GameObject(std::string_view filepath, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd);
	~GameObject();

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;

	sf::Sprite* GetSprite();
	virtual BoundingBox* GetBBox();
	AnimatedSprite* GetAnimSpr() { return m_spr; }

	virtual void Setm_initialPos(sf::Vector2f pos);
	virtual sf::Vector2f GetPosition() const;
	virtual void SetPosition(sf::Vector2f);
	virtual void SetPosition(float x, float y);

	virtual sf::Vector2f GetVelocity();
	virtual void SetVelocity(sf::Vector2f);
	virtual void SetVelocity(float x, float y);

	virtual sf::Vector2f GetOrigin() const;

	virtual void SetPrevPosition(sf::Vector2f);
	virtual void SetPrevPosition(float x, float y);
	virtual sf::Vector2f GetPrevPostion();

	bool GetOnGround();
	virtual void SetOnGround(bool grnd);

	bool GetAirbourne();
	void SetAirbourne(bool air);

	bool GetFalling();
	void SetFalling(bool fall);

	bool GetDirection();
	void SetDirection(bool dir);

	bool GetVisible();
	void SetVisible(bool vis);

	int GetObjectNum();

	void SetActive(bool act);
	bool GetActive();

protected:
	int m_type = -1;
	int m_objectID;
	static int s_objectNum;
	static int s_numOfEnemies;
	static int s_numOfObjects;

	int m_initialAnim = 0;
	bool m_initialDir = true;
	sf::Vector2f m_initialPos;

	bool m_active = true;

	bool m_visible = false;
	bool m_direction = true;
	bool m_onGround = false;
	bool m_falling = false;
	bool m_airbourne = false;

	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

	bool m_prevVisibility = false;
	sf::Vector2f m_prevPos;

	const float gravity = 0.981f;

	AnimatedSprite* m_spr;
	BoundingBox* m_bbox;


};

#endif
