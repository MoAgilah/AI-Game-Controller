#ifndef CameraH
#define CameraH

#include <vector>
#include <SFML\Graphics.hpp>
#include "../Utils.h"


enum CamID
{
	CENTER,TOPLEFT,TOPRIGHT,BOTTOMLEFT,BOTTOMRIGHT
};

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
	~Camera();
	void Reset(sf::RenderWindow & window);
	static Camera* GetCamera();
	sf::View GetView();
	void Update(float deltaTime);
	bool OnScreen(Player* ply);
	bool IsInView(sf::Vector2f pos, sf::Vector2f ori);
	bool IsInView(sf::Sprite spr);
	bool IsinView(sf::RectangleShape rect);
	void RenderGui(sf::RenderWindow& window);
private:
	Camera();
	static Camera* instance;
	sf::Vector2i screenDimensions;
	ScreenLimits curScrBounds;
	sf::View camera;

	sf::Sprite name;
	sf::Texture nameTex;

	sf::Sprite time;
	sf::Texture timeTex;

	std::vector<sf::Text*> m_text;
	sf::Font m_font;

	int displaytime;
	int tileNum;
};

#endif