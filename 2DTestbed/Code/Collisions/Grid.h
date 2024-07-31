#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>

class Tile;
class Camera;
class Grid
{
public:
	Grid();
	~Grid() = default;

	void Render(sf::RenderWindow & window);

	Tile* GetTile(int x, int y);
	std::vector<std::shared_ptr<Tile>> GetGrid() { return m_grid; }

private:

	void SetTileTypes();
	void SetTilePosition();

	sf::Font font;
	std::vector<std::shared_ptr<Tile>> m_grid;
};