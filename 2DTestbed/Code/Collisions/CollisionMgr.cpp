#include "CollisionMgr.h"
#include "Collisions.h"
#include "../GameObjects/Player.h"
#include <iostream>
#include <format>

namespace
{
	int GetDirTravelling(DynamicObject* obj)
	{
		//direction travelling
		sf::Vector2f dirV = obj->GetPosition() - obj->GetPrevPosition();
		int dir = DDIR;

		//if movement in x
		if (dirV.x != 0)
		{
			if (dirV.x > 0)	dir = RDIR;
			else dir = LDIR;
		}

		//if movement in y
		if (dirV.y != 0)
		{
			if (dirV.y > 0)	dir = DDIR;
			else dir = UDIR;
		}

		return dir;
	}
}

CollisionMgr::CollisionMgr()
{
	for (auto& gridTile : Collisions::Get()->GetGrid())
	{
		if (gridTile->GetType() == EMPTY)
			continue;
		std::shared_ptr<Tile> tile;
		tile.reset(gridTile);
		m_tiles.push_back(tile);
	}
}

void CollisionMgr::AddCollidable(Object* go)
{
	std::shared_ptr<Object> obj;
	obj.reset(go);
	m_collidables.push_back(obj);
}

void CollisionMgr::Render(sf::RenderWindow& window)
{
	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (!tile->GetBox()->GetHit())
			continue;

		tile->GetBox()->SetFillColour(sf::Color::Transparent);
		tile->GetBox()->SetOutlineColour(sf::Color::Red);
		tile->GetBox()->Render(window);
		tile->GetBox()->SetFillColour(sf::Color::White);
		tile->GetBox()->SetOutlineColour(sf::Color::Black);
	}
}

void CollisionMgr::ProcessCollisions()
{
	for (auto& obj : m_collidables)
	{
		if (!obj->GetActive())
			continue;

		for (auto& tile : m_tiles)
		{
			if (!tile->GetActive())
			{
				tile->GetBox()->SetHit(false);
				continue;
			}

			if (obj->GetAABB()->Intersects(tile->GetBox()))
			{
				std::cout << std::format("Collision with tile({})\n", tile->GetID());
				tile->GetBox()->SetHit(true);

				sf::Vector2f delta = sf::Vector2f(std::abs(tile->GetPosition().x - obj->GetPosition().x), std::abs(tile->GetPosition().y - obj->GetPosition().y));
				sf::Vector2f overlap = sf::Vector2f((obj->GetAABB()->GetExtents().x + tile->GetBox()->GetExtents().x) - delta.x, (obj->GetAABB()->GetExtents().y + tile->GetBox()->GetExtents().y) - delta.y);

				auto dir = GetDirTravelling((DynamicObject*)obj.get());

				if (dir == DDIR)
				{
					obj->SetPosition(obj->GetPosition() - sf::Vector2f(0, overlap.y));
					obj->GetAABB()->Update(obj->GetPosition());
					((Player*)obj.get())->SetOnGround(true);
				}
			}
			else
			{
				tile->GetBox()->SetHit(false);
			}
		}
	}

	/*for (size_t i = 0; i < m_collidables.size(); i++)
	{
		if (!m_collidables[i]->GetActive())
			continue;

		for (size_t j = 1; j < m_collidables.size(); j++)
		{
			if (!m_collidables[j]->GetActive())
				continue;
		}
	}*/
}
