#include "../Collisions/Grid.h"
#include "../Game/Camera.h"

#include "../Collisions/Tile.h"
#include <fstream>
#include <sstream>
#include <algorithm>


Grid::Grid()
{
	font.loadFromFile("Fonts/arial.ttf");

	Tile tile(font);
	//create grid for entire level
	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 313; x++)
		{
			m_grid.push_back(new Tile(font));
			m_grid.back()->SetID(x, y);
		}
	}

	SetTilePosition();
	SetTileTypes();
}

Grid::~Grid()
{
	if (m_grid.size() > 0)
	{
		for (size_t i = 0; i < m_grid.size(); i++)
		{
			delete m_grid[i];
			m_grid[i] = nullptr;
		}

		m_grid.clear();
	}

}

void Grid::SetTilePosition()
{
	int x = 0;
	int begin = x;

	//first row
	sf::Vector2f pos(m_grid.front()->GetOrigin().x*sX, m_grid.front()->GetOrigin().y*sY);
	m_grid[x]->SetPosition(pos);

	for (x = x + 1; x < 313; x++)
	{
		pos = sf::Vector2f(pos.x + (m_grid.front()->GetOrigin().x * sX) * 2, pos.y);
		m_grid[x]->SetPosition(pos);
	}

	//remaining rows
	for (int i = 0; i < 14; i++)
	{
		pos = sf::Vector2f(m_grid[begin]->GetPosition().x, m_grid[begin]->GetPosition().y + (m_grid.front()->GetOrigin().y*sY) * 2.f);
		m_grid[x]->SetPosition(pos);
		begin = x;

		int val = 2 + i;

		for (x = x + 1; x < 313 * val; x++)
		{
			pos = sf::Vector2f(pos.x + (m_grid.front()->GetOrigin().x * sX) * 2, pos.y);
			m_grid[x]->SetPosition(pos);
		}
	}
}

void Grid::Render(sf::RenderWindow & window)
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		if (m_grid[i]->GetVisible())
		{
#if _DEBUG
	#ifdef DRender
				m_grid[i]->Render(window);
	#endif //DRender
#endif
		}
	}
}

Tile* Grid::GetTile(int x, int y)
{
	//create id
	std::string id = std::to_string(x) + std::to_string(y);

	//extract tile if tile exists
	return *std::find_if(m_grid.begin(), m_grid.end(), [id](Tile* n) { return n->GetID() == id;});
}

void Grid::SetTileTypes()
{
	std::vector<int> types;
	std::ifstream inFile;
	//extract tile types from textfile
	inFile.open("Resources/TileTypes.txt");
	int type;
	while (inFile >> type) {
		types.push_back(type);
	}

	inFile.close();

	//assign tile types
	for (size_t i = 0; i < types.size(); i++)
	{
		m_grid[i]->SetType(types[i]);
	}
}

std::vector<Tile*> Grid::GetGrid()
{
	return m_grid;
}


