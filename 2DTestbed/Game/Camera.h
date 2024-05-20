#pragma once

#include "../Utils.h"
#include <SFML\Graphics.hpp>

class Player;

class Camera
{
public:
	Camera();
	~Camera() = default;

	sf::View GetView() const { return m_camera; }
	sf::Rect<float> GetCurrentScreenBounds() const { return m_curScrBounds; }

	bool OnScreen(const Player* ply) const;
	bool IsInView(const sf::Vector2f& pos, const sf::Vector2f& ori) const;
	bool IsInView(const sf::Sprite* spr) const;
	bool IsinView(const sf::RectangleShape& rect) const;
	void Reset(sf::RenderWindow& window);

private:

	sf::View m_camera;
	sf::Vector2i m_screenDimensions;
	sf::Rect<float> m_curScrBounds;
};