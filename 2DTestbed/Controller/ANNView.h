#ifndef ANNViewH
#define AnnViewH

#include <vector>
#include <SFML\Graphics.hpp>
#include "../Utils.h"

class Tile;
class Sensors;
class ANNView
{
public:
	ANNView();
	~ANNView();
	void Update();
	void Render(sf::RenderWindow & window);
	std::vector<Tile*> GetVecView();
	Sensors * GetSensors();
private:
	sf::View m_view;
	sf::RectangleShape back;
	const unsigned int size = 200; 
	std::vector<Tile*> m_vecView;
	Sensors* m_sensors;

	std::vector<sf::Text*> m_text;
	sf::Font m_font;
};

#endif
