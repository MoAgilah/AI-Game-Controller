#ifndef CameraH
#define CameraH

#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>
#include "../Utils.h"


struct ScreenLimits
{
	float left;
	float right;
	float bottom;
	float top;
};

class Player;
class Camera
{
public:
	~Camera() = default;
	void Reset(sf::RenderWindow & window);

	static Camera* GetCamera();
	sf::View GetView() const { return m_camera; };

	bool OnScreen(const Player* ply) const;
	bool IsInView(sf::Vector2f pos, sf::Vector2f ori);
	bool IsInView(const sf::Sprite* spr) const;
	bool IsinView(const sf::RectangleShape& rect) const;

	void Update();
	void RenderGui(sf::RenderWindow& window) const;
private:
	Camera();

	static Camera* m_instance;
	sf::Vector2i m_screenDimensions;
	ScreenLimits m_curScrBounds;
	sf::View m_camera;

	sf::Sprite m_nameSpr;
	sf::Texture m_nameTex;

	sf::Sprite m_timeSpr;
	sf::Texture m_timeTex;

	std::vector<std::unique_ptr<sf::Text>> m_text;
	sf::Font m_font;

	int m_displaytime;
	int m_tileNum;
};

#endif