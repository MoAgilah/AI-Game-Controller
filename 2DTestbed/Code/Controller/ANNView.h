#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class Tile;
class Sensors;
class ANNView
{
public:
	ANNView();
	~ANNView() = default;

	void Update();
	void Render(sf::RenderWindow & window);

	std::vector<Tile*> GetVecView();

private:

	sf::View m_view;
	sf::RectangleShape back;
	const unsigned int size = 200;
	std::vector<Tile*> m_vecView;

	std::vector<sf::Text*> m_text;
	sf::Font m_font;
};
