#ifndef ObjectH
#define ObjectH

#include "../GameObjects/GameObject.h"

enum OStates
{
	beenhit,animRun,reset
};

class Camera;
class Object : public GameObject
{
public:
	Object(TexID id, int rows, int cols, int bTyp, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1, const sf::Vector2f& initPos = sf::Vector2f());
	~Object();
	void Initialise(bool hasAnim, float moveSpd, float jmpSpeed);
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void SetIsAnimated(bool isAnim);
	bool IsAnimated();

	void Reset();
private:
	virtual void Animate(float deltaTime);

	bool m_goingUp;

	float animLength;

	bool isAnimating;

	bool m_hasAnim;
	float m_moveSpeed;
	float m_jumpSpeed;

	float m_airtime;
	const float c_maxAirTime = 1.0f;
	int s_objectNum;
};

#endif

