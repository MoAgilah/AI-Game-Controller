#pragma once

#include <SFML/Graphics.hpp>
#include "../Collisions/AABB.h"

class Player;

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Update();

	sf::View GetView() const { return m_camera; }

	bool IsInView(AABB* box);
	bool CheckVerticalBounds(AABB* box);
	void Reset(sf::RenderWindow& window);
	void RenderViewBox(sf::RenderWindow& window);

private:

	AABB m_viewBox;
	sf::View m_camera;
};