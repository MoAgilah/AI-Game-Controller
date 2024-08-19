#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>

class Tile;
class ANNView
{
public:
	ANNView();
	~ANNView() = default;

	void Update();
	void Render(sf::RenderWindow & window);

	const std::array<std::shared_ptr<Tile>, 255>& GetVecView() { return m_vecView; }

private:

	const float size = 200.f;
	sf::View m_view;
	sf::RectangleShape m_background;
	std::array<std::unique_ptr<sf::Text>, 3> m_text;
	std::array<std::shared_ptr<Tile>, 255> m_vecView;
};
