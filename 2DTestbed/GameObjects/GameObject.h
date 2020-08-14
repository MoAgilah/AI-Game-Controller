#ifndef  GameObjectH
#define  GameObjectH

#include <string>
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
	~GameObject();

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;

	sf::Sprite GetSprite();
	virtual BoundingBox* GetBBox();
	AnimatedSprite* GetAnimSpr() { return m_spr; }

	virtual void SetInitialPos(sf::Vector2f pos);
	virtual sf::Vector2f GetPosition();
	virtual void SetPosition(sf::Vector2f);
	virtual void SetPosition(float x, float y);

	virtual sf::Vector2f GetVelocity();
	virtual void SetVelocity(sf::Vector2f);
	virtual void SetVelocity(float x, float y);

	virtual sf::Vector2f GetOrigin();

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
	int objectID;
	int initialAnim;
	int m_type;
	int m_state;
	static int m_objectNum;
	static int s_numOfEnemies;
	static int s_numOfObjects;

	bool m_active;
	bool prevVisibility;
	bool visible;
	bool m_direction;
	bool m_onGround;
	bool m_falling;
	bool m_airbourne;
	bool initialDir;

	const float gravity = 0.981f;

	AnimatedSprite* m_spr;
	BoundingBox* m_bbox;

	sf::Vector2f m_position;
	sf::Vector2f m_prevPos;
	sf::Vector2f m_velocity;
	sf::Vector2f initialPos;
};

#endif
