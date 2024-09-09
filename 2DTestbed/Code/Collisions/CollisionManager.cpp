#include "CollisionManager.h"
#include <format>
#include <algorithm>
#include "../Controller/ControllerManager.h"
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Bill.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Collectables.h"
#include "../Utilities/Utilities.h"

namespace
{
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

	bool IsDynamicCollectable(TexID id)
	{
		return id == TexID::Shroom || id == TexID::Goal;
	}

	std::array<TexID, 9> dynamicObject =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck, TexID::Shroom
	};

	bool IsDynamicObject(TexID id)
	{
		return std::find(dynamicObject.begin(), dynamicObject.end(), id) != dynamicObject.end();
	}

	float GetYOffSet(float pDistX, float lDistY, float slopeY, float currY, float tileHeight)
	{
		auto percent = pDistX / lDistY;
		auto colHeight = lDistY * percent + slopeY;
		return ((currY - colHeight) / 16);
	}
}

CollisionManager::CollisionManager()
{
	for (auto& tile : m_grid.GetGrid())
	{
		if (tile->GetType() == EMPTY)
			continue;

		m_tiles.push_back(tile);
	}
}

void CollisionManager::AddCollidable(Object* go)
{
	m_collidables.push_back(go);
}

void CollisionManager::RemoveLastAdded()
{
	m_collidables.pop_back();
}

void CollisionManager::ReplacePlayer(Player * currPlayer)
{
	m_collidables[0] = currPlayer;
}

Object* CollisionManager::GetLastAdded()
{
	return m_collidables.back();
}

void CollisionManager::Render(sf::RenderWindow& window)
{
	m_grid.Render(window);

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (!tile->GetAABB()->hit)
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
		if (!collidable->GetActive())
			continue;

		if (gobj->GetObjectNum() == collidable->GetObjectNum())
			continue;

		ObjectToObjectCollisions(gobj, collidable);
	}
}

Tile* CollisionManager::GetTile(int x, int y)
{
	return m_grid.GetTile(x, y);
}

std::vector<std::shared_ptr<Tile>> CollisionManager::GetGrid()
{
	return m_grid.GetGrid();
}

std::vector<Object*> CollisionManager::GetCollidables()
{
	return m_collidables;
}

void CollisionManager::DynamicObjectToTileCollisions(DynamicObject* obj)
{
	for (auto& tile : m_tiles)
		tile->GetAABB()->hit = false;

	bool collided = false;
	std::vector<std::shared_ptr<Tile>> collidedWith;

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetAABB()->Intersects(obj->GetAABB()))
		{
			tile->GetAABB()->hit = true;
			collidedWith.push_back(tile);
		}
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
	{
		if (obj->GetID() != TexID::Goal)
		{
			obj->SetOnGround(false);
			obj->SetOnSlope(false);
		}
	}
}

void CollisionManager::DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile)
{
	Direction dir = GetFacingDirection(obj);
	Line tileTopEdge = tile->GetAABB()->GetSide(Side::Top);
	Point objBottomPoint = obj->GetAABB()->GetPoint(Side::Bottom);

	switch (tile->GetType())
	{
	case Types::OWAY:
	{
		if (dir == DDIR || dir == LDIR || dir == RDIR)
		{
			Capsule capsule(tileTopEdge, 6);
			Circle circle(obj->GetAABB(), 4);
			if (capsule.IntersectsCircle(circle))
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
					ResolveObjectToBoxTop(obj, tile->GetAABB());
			}
		}
		return;
	}
	case Types::GRND:
		if (dir == DDIR || dir == LDIR || dir == RDIR)
		{
			if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				ResolveObjectToBoxTop(obj, tile->GetAABB());
		}
		return;
	case Types::LCRN:
		[[fallthrough]];
	case Types::RCRN:
	{
		Point delta = tile->GetAABB()->GetPosition() - obj->GetPrevPosition();
		delta = Point(std::abs(delta.x), std::abs(delta.y));
		Point prevOverlap = (obj->GetAABB()->GetExtents() + tile->GetAABB()->GetExtents()) - delta;

		if (dir == DDIR)
		{
			if (prevOverlap.x > 0)
			{
				if (prevOverlap.x > prevOverlap.y)
				{
					if (tileTopEdge.IsPointAboveLine(objBottomPoint))
					{
						ResolveObjectToBoxTop(obj, tile->GetAABB());
					}
				}
			}
		}

		if (prevOverlap.y > 0)
		{
			ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
		}
		else
		{
			if (dir == LDIR || dir == RDIR)
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				{
					ResolveObjectToBoxTop(obj, tile->GetAABB());
					if (!IsPlayerObject(obj->GetID()))
					{
						Point cnt = tile->GetType() == LCRN ?
							obj->GetAABB()->GetPoint(Side::Right) :
							obj->GetAABB()->GetPoint(Side::Left);

						Circle circle(cnt, 4);
						Capsule capsule(tile->GetEdge(), 4);
						if (capsule.IntersectsCircle(circle))
							obj->SetDirection(!obj->GetDirection());
					}
				}
			}
		}
	}
		return;
	case Types::WALL:
		ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
		return;
	case Types::DIAGU:
	{
		if (dir == DDIR)
		{
			Line line = tile->GetSlope(0, 1);
			Circle circle(obj->GetAABB(), 4);
			if (line.IsPointAboveLine(circle.center))
			{
				Capsule capsule(line, 4);
				if (capsule.IntersectsCircle(circle))
				{
					obj->SetOnSlope(true);
					if (!obj->GetShouldSlideLeft())
						obj->SetShouldSlideLeft(true);
				}
			}
		}

		if (dir == RDIR)
		{
			Line line = tile->GetSlope(0, 1);
			Circle circle(obj->GetAABB(), 4);
			Capsule capsule(line, 4);
			if (capsule.IntersectsCircle(circle))
			{
				auto yOffset = GetYOffSet(GetXDist(line.start, circle.center),
					line.DistY(),
					line.start.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, yOffset));
				obj->SetOnSlope(true);
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
		}

		if (dir == LDIR)
		{
			Line line = tile->GetSlope(1, 0);
			Circle circle(obj->GetAABB(), 4);
			Capsule capsule(line, 4);
			if (!capsule.IntersectsCircle(circle))
			{
				auto yOffset = GetYOffSet(GetXDist(circle.center, line.start),
					line.DistY(),
					line.start.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, -yOffset));
				obj->SetOnSlope(true);
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
		}
		return;
	}
	case Types::DIAGD:
	{
		if (dir == DDIR)
		{
			Line line = tile->GetSlope(0, 1);
			Circle circle(obj->GetAABB(), 4);
			if (line.IsPointAboveLine(circle.center))
			{
				Capsule capsule(line, 4);
				if (capsule.IntersectsCircle(circle))
				{
					obj->SetOnSlope(true);
					if (!obj->GetShouldSlideRight())
						obj->SetShouldSlideRight(true);
				}
			}
		}

		if (dir == LDIR)
		{
			Line line = tile->GetSlope(1, 0);
			Circle circle(obj->GetAABB(), 4);
			Capsule capsule(line, 4);
			if (capsule.IntersectsCircle(circle))
			{
				auto yOffset = GetYOffSet(GetXDist(circle.center, line.start),
					line.DistY(),
					line.start.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, yOffset));
				obj->SetOnSlope(true);
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
		}

		if (dir == RDIR)
		{
			Line line = tile->GetSlope(0, 1);
			Circle circle(obj->GetAABB(), 4);
			Capsule capsule(line, 4);
			if (!capsule.IntersectsCircle(circle))
			{
				auto yOffset = GetYOffSet(GetXDist(line.start, circle.center),
					line.DistY(),
					line.start.y,
					obj->GetAABB()->GetPosition().y,
					tile->GetTileHeight());

				obj->Move(sf::Vector2f(0, -yOffset));
				obj->SetOnSlope(true);
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
		}
		return;
	}
	}
}

void CollisionManager::PlayerToObjectCollisions(Player* ply, Object* obj)
{
	if (IsBoxObject(obj->GetID()))
	{
		if (obj->Intersects(ply))
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
		if (obj->Intersects(ply))
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
		if (!enemy->HasLifes())
			return;

		if (obj->Intersects(ply))
			PlayerToEnemyResolutions(ply, (Enemy*)obj);
	}
}

void CollisionManager::ObjectToObjectCollisions(Object* obj1, Object* obj2)
{
	if (IsPlayerObject(obj1->GetID()))
	{
		PlayerToObjectCollisions((Player*)obj1, obj2);
	}
	else if (IsPlayerObject(obj2->GetID()))
	{
		PlayerToObjectCollisions((Player*)obj2, obj1);
	}
	else if (IsBoxObject(obj2->GetID()))
	{
		if (obj2->Intersects(obj1))
		{
			DynamicObject* dynObj = (DynamicObject*)obj1;

			Point delta = obj2->GetAABB()->GetPosition() - dynObj->GetPrevPosition();
			delta = Point(std::abs(delta.x), std::abs(delta.y));
			Point prevOverlap = (dynObj->GetAABB()->GetExtents() + obj2->GetAABB()->GetExtents()) - delta;

			DynamicObjectToBoxResolutions(GetFacingDirection(dynObj), prevOverlap, dynObj, obj2->GetAABB());
		}
	}
	else
	{
		DynamicObjectToDynamicObject((DynamicObject*)obj1, (DynamicObject*)obj2);
	}
}

void CollisionManager::PlayerToQBoxResolutions(Player* ply, QBox* box)
{
	Direction dir = GetFacingDirection(ply);
	// calculate the overlap
	Point delta = box->GetAABB()->GetPosition() - ply->GetPrevPosition();
	delta = Point(std::abs(delta.x), std::abs(delta.y));
	Point prevOverlap = (ply->GetAABB()->GetExtents() + box->GetAABB()->GetExtents()) - delta;

	if (dir == Direction::UDIR)
	{
		if (prevOverlap.x > 0)
		{
			if (prevOverlap.x > prevOverlap.y)
			{
				if (box->GetCanHit())//if not yet been hit
				{
					box->SetJustHit(true);
					GameManager::GetGameMgr()->GetWorld()->AddObject(sf::Vector2f(box->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * scale.y) - (box->GetOrigin().y * scale.y) + 4.f));
					//ply->UpdateFitness(100);
				}
			}
		}
	}

	DynamicObjectToBoxResolutions(dir, prevOverlap, ply, box->GetAABB());
}

void CollisionManager::PlayerToSBoxResolutions(Player* ply, SBox* box)
{
	Direction dir = GetFacingDirection(ply);
	// calculate the overlap
	Point delta = box->GetAABB()->GetPosition() - ply->GetPrevPosition();
	delta = Point(std::abs(delta.x), std::abs(delta.y));
	Point prevOverlap = (ply->GetAABB()->GetExtents() + box->GetAABB()->GetExtents()) - delta;

	if (box->GetCanHit())//if not yet been hit
	{
		if (dir == Direction::UDIR)
		{
			if (prevOverlap.x > 0)
			{
				if (prevOverlap.x > prevOverlap.y)
				{
					if (!box->GetJustHit())
						box->SetJustHit(true);
				}
			}
		}
		else if (dir == Direction::DDIR)
		{
			if (prevOverlap.x > 0)
			{
				if (prevOverlap.x < prevOverlap.y)
				{
					if (ply->GetIsSuper() && ply->GetCantSpinJump())
					{
						box->SetJustSmashed(true);
						return;
					}
				}
			}
		}

		DynamicObjectToBoxResolutions(dir, prevOverlap, ply, box->GetAABB(), false);
	}
}

void CollisionManager::PlayerToEnemyResolutions(Player* ply, Enemy* enmy)
{
	if (!ply->GetIsAlive())
		return;

	Circle circle(ply->GetAABB(), 4);
	Capsule capsule(enmy->GetAABB()->GetSide(Side::Top), 6);
	if (capsule.IntersectsCircle(circle) && enmy->GetID() != TexID::PPlant)
	{
		enmy->DecrementLife();
		ply->Bounce();
		//	ptmp->UpdateFitness(-100);
	}
	else
	{
		if (!ply->GetIfInvulnerable())
		{
			if (ply->GetIsSuper())
			{
				ply->SetInvulnerability();
				ply->SetIsSuper(false);
				//ptmp->UpdateFitness(-100);
			}
			else
			{
				ply->SetIsAlive(false);
			}
		}
	}
}

void CollisionManager::DynamicObjectToBoxResolutions(Direction dirOfTravel, const Point& prevOverlap, DynamicObject* obj, AABB* box, bool resolveUpDir)
{
	if (prevOverlap.y > 0)
	{
		if (prevOverlap.x <=0)
		{
			ResolveObjectToBoxHorizontally(obj, box);
			if (IsPlayerObject(obj->GetID()))
				((Player*)obj)->ForceFall();
			return;
		}
	}

	if (prevOverlap.x > 0)
	{
		if (dirOfTravel == Direction::UDIR)
		{
			if (resolveUpDir)
			{
				if (prevOverlap.x > prevOverlap.y)
				{
					ResolveObjectToBoxBottom(obj, box);
					return;
				}
			}
		}
		else
		{
			ResolveObjectToBoxTop(obj, box);
		}
	}
}

void CollisionManager::DynamicObjectToDynamicObject(DynamicObject* obj1, DynamicObject* obj2)
{
	float tFirst, tLast = 0;
	if (obj1->GetAABB()->IntersectsMoving(obj2->GetAABB(), obj1->GetVelocity(), obj2->GetVelocity(), tFirst, tLast))
	{
		Point move = Point(std::lerp(obj1->GetPrevPosition().x, obj1->GetPosition().y, tFirst),
			std::lerp(obj1->GetPrevPosition().y, obj1->GetPosition().y, tFirst));

		Point amt = obj1->GetPosition() - move;

		obj1->Move(amt * (obj1->GetDirection() ? 1.f : -1.f));
		obj1->SetDirection(!obj1->GetDirection());
		obj1->SetVelocity(obj1->GetVelocity().x * -1, obj1->GetVelocity().x * -1);

		move = Point(std::lerp(obj2->GetPrevPosition().x, obj2->GetPosition().y, tFirst),
			std::lerp(obj2->GetPrevPosition().y, obj2->GetPosition().y, tFirst));

		amt = obj2->GetPosition() - move;

		obj2->Move(amt * (obj2->GetDirection() ? 1.f : -1.f));
		obj2->SetDirection(!obj2->GetDirection());
		obj2->SetVelocity(obj2->GetVelocity().x * -1, obj2->GetVelocity().x * -1);
	}
}

void CollisionManager::ResolveObjectToBoxTop(DynamicObject* obj, AABB* box)
{
	obj->Move(0, -box->GetOverlap().y);
	obj->SetOnGround(true);
	obj->SetOnSlope(false);
}

void CollisionManager::ResolveObjectToBoxBottom(DynamicObject* obj, AABB* box)
{
	obj->Move(0, box->GetOverlap().y);
	if (IsPlayerObject(obj->GetID()))
		((Player*)obj)->ForceFall();
}

void CollisionManager::ResolveObjectToBoxHorizontally(DynamicObject* obj, AABB* box)
{
	obj->Move((obj->GetDirection() ? -1 : 1) * box->GetOverlap().x, 0);
	if (!IsPlayerObject(obj->GetID()))
		obj->SetDirection(!obj->GetDirection());
}

Direction CollisionManager::GetFacingDirection(DynamicObject* obj)
{
	const sf::Vector2f& currentVel = obj->GetVelocity();

	Direction currentDir = DDIR;

	if (currentVel.x != 0.f || currentVel.y != 0.f)
	{
		float vxa = std::abs(currentVel.x);
		float vya = std::abs(currentVel.y);

		if (vxa > vya)
		{
			currentDir = (currentVel.x < 0) ?
				Direction::LDIR : Direction::RDIR;
		}
		else
		{
			currentDir = (currentVel.y < 0) ?
				Direction::UDIR : Direction::DDIR;
		}
	}

	return currentDir;
}