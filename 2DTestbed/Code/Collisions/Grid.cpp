#include "../Collisions/Grid.h"
#include <format>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../Collisions/Tile.h"

Grid::Grid()
{
	auto& font = GameManager::Get()->GetFontMgr().GetStandardFont();

	//create grid for entire level
	for (int y = 0; y < 15; y++)
	{
		for (int x = 0; x < 313; x++)
			m_grid.push_back(std::make_shared<Tile>(x, y, font));
	}

	SetTileTypes();
	SetTilePosition();
}

void Grid::SetTilePosition()
{
	int x = 0;
	int begin = x;

	//first row
	sf::Vector2f pos(m_grid.front()->GetOrigin().x * GameConstants::Scale.x, m_grid.front()->GetOrigin().y * GameConstants::Scale.y);
	m_grid[x]->SetPosition(pos);

	for (x = x + 1; x < 313; x++)
	{
		pos = sf::Vector2f(pos.x + (m_grid.front()->GetOrigin().x * GameConstants::Scale.x) * 2, pos.y);
		m_grid[x]->SetPosition(pos);
	}

	//remaining rows
	for (int i = 0; i < 14; i++)
	{
		pos = sf::Vector2f(m_grid[begin]->GetPosition().x, m_grid[begin]->GetPosition().y + (m_grid.front()->GetOrigin().y * GameConstants::Scale.y) * 2.f);
		m_grid[x]->SetPosition(pos);
		begin = x;

		int val = 2 + i;

		for (x = x + 1; x < 313 * val; x++)
		{
			pos = sf::Vector2f(pos.x + (m_grid.front()->GetOrigin().x * GameConstants::Scale.x) * 2, pos.y);
			m_grid[x]->SetPosition(pos);
		}
	}
}

void Grid::Render(sf::RenderWindow & window)
{
	for (auto& tile : m_grid)
	{
		if (tile->GetActive())
		{
#if _DEBUG
			if (GameConstants::DRender)
				tile->Render(window);
#endif
		}
	}
}

Tile* Grid::GetTile(int x, int y)
{
	//create id
	std::string id = std::format("{},{}", x, y);

	auto it = std::find_if(m_grid.begin(), m_grid.end(), [id](auto n) { return n->GetID() == id; });
	if (it != m_grid.end())
		return it->get();

	//extract tile if tile exists
	return nullptr;
}

void Grid::SetTileTypes()
{
	std::ifstream inFile;
	std::vector<int> types;

	//extract tile types from text file
	inFile.open("Resources/TileTypes.txt");

	int type;
	while (inFile >> type)
		types.push_back(type);

	inFile.close();

	//assign tile types
	for (size_t i = 0; i < types.size(); i++)
		m_grid[i]->SetType(types[i]);
}