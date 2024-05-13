#pragma once

#include "../GameObjects/GameObject.h"

enum OStates
{
	beenhit,animRun,reset
};

class Camera;
class Object : public GameObject
{
public:
	Object(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc = 1, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1);
	~Object();
	void Initialise(bool hasAnim, float moveSpd, float jmpSpeed);
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void SetIsAnimated(bool isAnim);
	bool IsAnimated();
	void SetPosition(sf::Vector2f pos);
	void Reset();
	int GetObjectNum();
private:
	virtual void Animate(float deltaTime);

	bool goingUp;

	float animLength;

	bool isAnimating;

	bool m_hasAnim;
	float m_moveSpeed;
	float m_jumpSpeed;

	float m_airtime;
	const float c_maxAirTime = 1.0f;
	int m_objectNum;
};

