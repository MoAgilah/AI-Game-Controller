#pragma once

#include <vector>
#include <memory>
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

	std::vector<std::shared_ptr<Tile>> GetVecView() { return m_vecView; }

private:

	const unsigned int size = 200;
	sf::View m_view;
	sf::RectangleShape m_background;
	std::vector<std::unique_ptr<sf::Text>> m_text;
	std::vector<std::shared_ptr<Tile>> m_vecView;
};
