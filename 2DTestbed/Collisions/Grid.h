#ifndef GridH
#define GridH

#include <iostream>
#include <string>
#include <vector>

#include <sfml/Graphics.hpp>

class Tile;
class Camera;
class Grid
{
public:
	Grid();
	~Grid();
	void Render(sf::RenderWindow & window);
	Tile* GetTile(int x, int y);
	std::vector<Tile*> GetGrid();
private:
	void SetTileTypes();
	void SetTilePosition();

	std::vector<Tile*> m_grid;
	sf::Font font;
};

#endif