#ifndef CameraH
#define CameraH

#include "../Utils.h"
#include <memory>
#include <SFML\Graphics.hpp>
#include <array>

enum class GUI { NAME, TIME, MAX };

class Player;

class Camera
{
public:
	Camera();
	~Camera() = default;

	sf::View GetView() const { return m_camera; }

	bool OnScreen(const Player* ply) const;
	bool IsInView(const sf::Vector2f& pos, const sf::Vector2f& ori) const;
	bool IsInView(const sf::Sprite* spr) const;
	bool IsinView(const sf::RectangleShape& rect) const;

	void Reset(sf::RenderWindow& window);
	void UpdateGUI(float deltaTime);
	void RenderGUI(sf::RenderWindow& window) const;

private:

	sf::View m_camera;
	sf::Vector2i m_screenDimensions;
	sf::Rect<float> m_curScrBounds;

	sf::Font m_font;
	std::array<sf::Sprite, (int)GUI::MAX> m_sprites;
	std::array<sf::Texture, (int)GUI::MAX> m_textures;
	std::array<sf::Text, (int)GUI::MAX> m_text;
};

#endif