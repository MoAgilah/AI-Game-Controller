#pragma once

#include "../GameObjects/GameObject.h"

enum OStates
{
	beenhit,animRun,reset
};

class Camera;
class Object : public AnimatedGameObject
{
public:
	Object(TexID id, int rows, int cols, int bTyp, bool dir = true, bool symmetrical = true, int initAnim = 0, float animSpd = 1, const sf::Vector2f& initPos = sf::Vector2f());
	~Object() final = default;

	void Update(float deltaTime) final;
	void Render(sf::RenderWindow& window) final;
	void Reset() final;

	void SetIsAnimated(bool isAnim) { m_isAnimating = isAnim; }
	bool IsAnimated() const { return m_isAnimating; }

private:
	virtual void Animate(float deltaTime);

	bool m_goingUp = false;
	bool m_isAnimating = false;
	int s_objectNum;
	float m_airtime;
	float m_animLength;
};

