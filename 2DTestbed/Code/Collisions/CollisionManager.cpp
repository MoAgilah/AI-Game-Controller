#include "CollisionManager.h"

#include "../Game/Camera.h"
#include "../Game/GameManager.h"

#include "../GameObjects/Object.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Collectables.h"

#include "../GameObjects/Chuck.h"
#include "../GameObjects/Rex.h"
#include "../GameObjects/Bill.h"

#include "../Controller/CtrlMgr.h"

#include "../Game/Constants.h"

#include <algorithm>

namespace
{
	bool IsPlayerObject(TexID id)
	{
		return id == TexID::Mario || id == TexID::Super;
	}

	std::array<TexID, 7> canCollideWithTile =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Rex, TexID::Chuck, TexID::Shroom, TexID::Goal
	};

	bool CanCollideWithTile(TexID id)
	{
		return std::find(canCollideWithTile.begin(), canCollideWithTile.end(), id) != canCollideWithTile.end();
	}

	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith)
	{
		std::ranges::sort(collidedWith, [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b)
			{
				if (a->GetColNum() == b->GetColNum())
					return a->GetRowNum() < b->GetRowNum();
				return a->GetColNum() > b->GetColNum();
			});
	}

	std::array<TexID, 5> collectableObject =
	{
		TexID::Coin, TexID::YCoin, TexID::ChkPnt, TexID::Shroom, TexID::Goal
	};

	bool IsCollectableObject(TexID id)
	{
		return std::find(collectableObject.begin(), collectableObject.end(), id) != collectableObject.end();
	}

	bool IsDynamicCollectable(TexID id)
	{
		return id == TexID::Shroom || id == TexID::Goal;
	}

	std::array<TexID, 9> dynamicObject =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck, TexID::Shroom, TexID::Goal
	};

	bool IsDynamicObject(TexID id)
	{
		return std::find(dynamicObject.begin(), dynamicObject.end(), id) != dynamicObject.end();
	}

	std::array<TexID, 5> enemyObject =
	{
		TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck
	};

	bool IsEnemyObject(TexID id)
	{
		return std::find(enemyObject.begin(), enemyObject.end(), id) != enemyObject.end();
	}

	bool IsBoxObject(TexID id)
	{
		return id == TexID::QBox || id == TexID::SBox;
	}
}

CollisionManager::CollisionManager()
{
	for (auto& gridTile : m_grid.GetGrid())
	{
		if (gridTile->GetType() == EMPTY)
			continue;
		std::shared_ptr<Tile> tile;
		tile.reset(gridTile);
		m_tiles.push_back(tile);
	}
}

void CollisionManager::AddCollidable(Object* go)
{
	std::shared_ptr<Object> obj;
	obj.reset(go);
	m_collidables.push_back(obj);
}

void CollisionManager::RemoveLastAdded()
{
	m_collidables.pop_back();
}

void CollisionManager::ReplacePlayer(Player * currPlayer)
{
	m_collidables[0].reset(currPlayer);
}

Object* CollisionManager::GetLastAdded()
{
	return m_collidables.back().get();
}

void CollisionManager::Render(sf::RenderWindow& window)
{
	m_grid.Render(window);

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (!tile->GetAABB()->GetHit())
			continue;

		tile->GetAABB()->SetFillColour(sf::Color::Transparent);
		tile->GetAABB()->SetOutlineColour(sf::Color::Red);
		tile->Render(window);
		tile->GetAABB()->SetFillColour(sf::Color::White);
		tile->GetAABB()->SetOutlineColour(sf::Color::Black);
	}
}

void CollisionManager::ProcessCollisions(Object* gobj)
{
	if (CanCollideWithTile(gobj->GetID()))
		DynamicObjectToTileCollisions((DynamicObject*)gobj);

	for (auto& collidable : m_collidables)
	{
		if (gobj->GetObjectNum() == collidable->GetObjectNum())
			continue;

		if (!collidable->GetActive())
			continue;

		if (IsPlayerObject(gobj->GetID()))
		{
			PlayerToObjectCollisions((Player*)gobj, collidable.get());
		}
		else
		{
			ObjectToObjectCollisions(gobj, collidable.get());
		}
	}
}

Tile CollisionManager::GetTile(int x, int y)
{
	return *m_grid.GetTile(x, y);
}

std::vector<Tile*> CollisionManager::GetGrid()
{
	return m_grid.GetGrid();
}

std::vector<std::shared_ptr<Object>> CollisionManager::GetCollidables()
{
	return m_collidables;
}

void CollisionManager::DynamicObjectToTileCollisions(DynamicObject* obj)
{
	for (auto& tile : m_tiles)
		tile->GetAABB()->SetHit(false);

	bool collided = false;
	std::vector<std::shared_ptr<Tile>> collidedWith;

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetAABB()->Intersects(obj->GetAABB()))
			collidedWith.push_back(tile);
	}

	if (collided = !collidedWith.empty())
	{
		if (obj->GetDirection())
			SortCollidedTiles(collidedWith);

		for (auto& tile : collidedWith)
			if (tile->GetAABB()->Intersects(obj->GetAABB()))
				DynamicObjectToTileResolution(obj, tile.get());
	}

	if (!collided)
		if (obj->GetID() != TexID::Goal)
			obj->SetOnGround(false);
}

void CollisionManager::DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile)
{
	Direction dir = GetDirTravelling(obj);

	float objBottom = obj->GetAABB()->GetPosition().y + obj->GetOrigin().y;
	float tileTop = tile->GetPosition().y - tile->GetOrigin().y;

	if (tile->GetType() != DIAGU && tile->GetType() != DIAGD)
	{
		switch (dir)
		{
		case DDIR:
		{
			switch (tile->GetType())
			{
			case GRND:
			case OWAY:
			case CRN:
			{
				if (objBottom < tileTop)
					ResolveObjectToBoxTop(obj, tile->GetAABB());
				return;
			}
			default:
				return;
			}
		}
		case RDIR:
		case LDIR:
		{
			bool shouldResolve = true;
			switch (tile->GetType())
			{
			case CRN:
				shouldResolve = objBottom > tileTop;
				[[fallthrough]];
			case WALL:
			{
				if (shouldResolve)
					ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
				return;
			}
			default:
				return;
			}
		}
		}
	}
	else
	{
		for (auto& box : tile->GetSlopeBBox())
		{
			if (dir == DDIR)
			{
				if (box.Intersects(obj->GetAABB()))
				{
					ResolveObjectToBoxTop(obj, &box);
					return;
				}
			}
		}
	}

	obj->SetOnGround(false);
}

void CollisionManager::PlayerToObjectCollisions(Player* ply, Object* obj)
{
	if (IsBoxObject(obj->GetID()))
	{
		if (obj->GetAABB()->Intersects(ply->GetAABB()))
		{
			switch (obj->GetID())
			{
			case TexID::QBox:
				PlayerToQBoxResolutions(ply, (QBox*)obj);
				break;
			case TexID::SBox:
				PlayerToSBoxResolutions(ply, (SBox*)obj);
				break;
			}
		}
	}
	else if (IsCollectableObject(obj->GetID()))
	{
		if (obj->GetAABB()->Intersects(ply->GetAABB()))
		{
			if (IsDynamicCollectable(obj->GetID()))
			{
				((DynamicCollectable*)obj)->Collect(ply);
			}
			else
			{
				((StaticCollectable*)obj)->Collect(ply);
			}
		}
	}
	else if (IsEnemyObject(obj->GetID()))
	{
		Enemy* enemy = (Enemy*)obj;
		if (!enemy->GetIsAlive())
			return;

		bool shouldResolve = false;
		if (TexID::Bill == obj->GetID())
		{
			shouldResolve = ((Bill*)obj)->GetBody().Intersects(ply->GetAABB());
		}
		else
		{
			shouldResolve = obj->GetAABB()->Intersects(ply->GetAABB());
		}

		if (shouldResolve)
			PlayerToEnemyResolutions(ply, (Enemy*)obj);
	}
}

void CollisionManager::ObjectToObjectCollisions(Object* obj1, Object* obj2)
{
	if (IsPlayerObject(obj2->GetID()))
	{
		PlayerToObjectCollisions((Player*)obj2, obj1);
	}
	else if (IsBoxObject(obj2->GetID()))
	{
		if (obj2->GetAABB()->Intersects(obj1->GetAABB()))
		{
			DynamicObject* dynObj = (DynamicObject*)obj1;
			DynamicObjectToBoxResolutions((Direction)GetDirTravelling(dynObj), dynObj, obj2->GetAABB());
		}
	}
	else
	{
		DynamicObjectToDynamicObject((DynamicObject*)obj1, (DynamicObject*)obj2);
	}
}

void CollisionManager::PlayerToQBoxResolutions(Player* ply, QBox* box)
{
	auto dir = (Direction)GetDirTravelling(ply);
	if (dir == Direction::UDIR)
	{
		if (box->GetCanHit())//if not yet been hit
		{
			box->SetJustHit(true);
			GameManager::GetGameMgr()->GetLevel()->AddObject(sf::Vector2f(box->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * sY) - (box->GetOrigin().y * sY) + 4.f));
			//ply->UpdateFitness(100);
		}
	}

	DynamicObjectToBoxResolutions(dir, ply, box->GetAABB());
}

void CollisionManager::PlayerToSBoxResolutions(Player* ply, SBox* box)
{
	if (box->GetCanHit())//if not yet been hit
	{
		Direction dir = GetDirTravelling(ply);
		if (dir == Direction::UDIR)
		{
			if (!box->GetJustHit())
				box->SetJustHit(true);
		}
		else if (dir == Direction::DDIR)
		{
			if (ply->GetIsSuper() && ply->GetCantSpinJump())
			{
				box->SetJustSmashed(true);
				return;
			}
		}

		DynamicObjectToBoxResolutions(dir, ply, box->GetAABB(), false);
	}
}

void CollisionManager::PlayerToEnemyResolutions(Player* ply, Enemy* enmy)
{
	float pBot = ply->GetAABB()->GetPosition().y + ply->GetAABB()->GetOrigin().y;
	float eTop = enmy->GetAABB()->GetPosition().y - enmy->GetAABB()->GetOrigin().y;

	if (ply->GetIsAlive())
		return;

	if (pBot > eTop || enmy->GetID() == TexID::PPlant)
	{
		if (!ply->GetIfInvulnerable())
		{
			if (ply->GetIsSuper())
			{
				ply->JustBeenHit(true);
				ply->SetIsSuper(false);
				//ptmp->UpdateFitness(-100);
			}
			else
			{
				ply->SetIsAlive(false);
			}
		}
	}
	else
	{
		//set hover time
		ply->JusyHitEnemy(0.1f);
		enmy->DecrementLife();
		//ptmp->UpdateFitness(-100);
	}
}

void CollisionManager::DynamicObjectToBoxResolutions(Direction dirOfTravel, DynamicObject* obj, AABB* box, bool resolveUpDir)
{
	switch (dirOfTravel)
	{
	case UDIR:
		if (resolveUpDir)
			ResolveObjectToBoxBottom(obj, box);
		break;
	case DDIR:
		ResolveObjectToBoxTop(obj, box);
		break;
	case LDIR:
	case RDIR:
		ResolveObjectToBoxHorizontally(obj, box);
		break;
	}
}

void CollisionManager::DynamicObjectToDynamicObject(DynamicObject* obj1, DynamicObject* obj2)
{
	float tFirst, tLast = 0;
	if (obj1->GetAABB()->IntersectsMoving(obj2->GetAABB(), obj1->GetVelocity(), obj2->GetVelocity(), tFirst, tLast))
	{
		obj1->SetPosition(
		std::lerp(obj1->GetPrevPostion().x, obj1->GetPosition().y, tFirst),
		std::lerp(obj1->GetPrevPostion().x, obj1->GetPosition().y, tFirst));
		obj1->GetAABB()->Update(obj1->GetPosition());
		obj1->SetDirection(!obj1->GetDirection());

		obj2->SetPosition(
			std::lerp(obj2->GetPrevPostion().x, obj2->GetPosition().y, tFirst),
			std::lerp(obj2->GetPrevPostion().x, obj2->GetPosition().y, tFirst));
		obj2->GetAABB()->Update(obj2->GetPosition());
		obj2->SetDirection(!obj2->GetDirection());
	}
}

void CollisionManager::ResolveObjectToBoxTop(DynamicObject* obj, AABB* box)
{
	obj->Move(0, -box->GetOverlap().y);
	obj->SetOnGround(true);
}

void CollisionManager::ResolveObjectToBoxBottom(DynamicObject* obj, AABB* box)
{
	obj->Move(0, box->GetOverlap().y);
	if (IsPlayerObject(obj->GetID()))
		((Player*)obj)->SetAirbourne(false);
}

void CollisionManager::ResolveObjectToBoxHorizontally(DynamicObject* obj, AABB* box)
{
	obj->Move((obj->GetDirection() ? -1 : 1) * box->GetOverlap().x, 0);
	if (!IsPlayerObject(obj->GetID()))
		obj->SetDirection(!obj->GetDirection());
}

Direction CollisionManager::GetDirTravelling(DynamicObject* obj)
{
	//direction travelling
	sf::Vector2f dirV = obj->GetPosition() - obj->GetPrevPostion();
	Direction dir = DDIR;

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

