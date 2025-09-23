#include "CollisionManager.h"
#include <format>
#include <algorithm>
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

	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>>& collidedWith)
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

	std::array<TexID, 4> dynamicObject =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Rex,
	};

	bool IsDynamicObject(TexID id)
	{
		return std::find(dynamicObject.begin(), dynamicObject.end(), id) != dynamicObject.end();
	}

	float GetYOffSet(float pDistX, float lDistY, float slopeY, float currY, float tileHeight)
	{
		// Guard: avoid divide-by-zero and nonsense ratios near flat/degenerate slopes
		float denom = std::max(1e-4f, std::abs(lDistY));
		float t = std::clamp(pDistX / denom, 0.0f, 1.0f);

		float colHeight = lDistY * t + slopeY;

		// Scale using the real tile height, not a magic constant
		float norm = std::max(1.0f, tileHeight);
		return (currY - colHeight) / norm;
	}

	bool IsMovingTowards(Point p1, Point p2, Point v1, Point v2)
	{
		Point v = v2 - v1;
		Point d = p2 - p1;

		return (d.x * v.x + d.y * v.y) < 0;
	}

	Point GetPreviousOverlap(AABB* box, DynamicObject* obj)
	{
		Point delta = box->GetPosition() - obj->GetPrevPosition();
		delta = Point(std::abs(delta.x), std::abs(delta.y));
		return (obj->GetAABB()->GetExtents() + box->GetExtents()) - delta;
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
}

void CollisionManager::ProcessCollisions(Object* gobj)
{
	if (CanCollideWithTile(gobj->GetID()))
		DynamicObjectToTileCollisions((DynamicObject*)gobj);

	for (size_t i = 0; i < m_collidables.size(); i++)
	{
		if (!m_collidables[i]->GetActive())
			continue;

		if (gobj->GetObjectNum() == m_collidables[i]->GetObjectNum())
			continue;

		ObjectToObjectCollisions(gobj, m_collidables[i]);
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
		Point prevOverlap = GetPreviousOverlap(tile->GetAABB(), obj);

		if (dir == DDIR && prevOverlap.x > 0 && prevOverlap.x > prevOverlap.y)
		{
			if (tileTopEdge.IsPointAboveLine(objBottomPoint))
			{
				ResolveObjectToBoxTop(obj, tile->GetAABB());
				return;
			}
		}

		if (prevOverlap.y > 0)
		{
			ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
		}
		else if (dir == LDIR || dir == RDIR)
		{
			if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				ResolveObjectToBoxTop(obj, tile->GetAABB());
			DynamicObjectToEdgeBounds(obj, tile);
		}
		return;
	}
	case Types::WALL:
		ResolveObjectToBoxHorizontally(obj, tile->GetAABB());
		return;
	case Types::DIAGU:
	{
		bool hit = false;
		switch (dir)
		{
		case DDIR: hit = ResolveObjectToSlopeTop(obj, tile);           break;
		case RDIR: hit = ResolveObjectToSlopeIncline(obj, tile, 0, 1); break;
		case LDIR: hit = ResolveObjectToSlopeDecline(obj, tile, 1, 0); break;
		}

		if (hit && !obj->GetShouldSlideLeft())
			obj->SetShouldSlideLeft(true);
		return;
	}
	case Types::DIAGD:
	{
		bool hit = false;
		switch (dir)
		{
		case DDIR: hit = ResolveObjectToSlopeTop(obj, tile); break;
		case LDIR: hit = ResolveObjectToSlopeIncline(obj, tile, 1, 0); break;
		case RDIR: hit = ResolveObjectToSlopeDecline(obj, tile, 0, 1); break;
		}

		if (hit && !obj->GetShouldSlideRight())
			obj->SetShouldSlideRight(true);
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
				((DynamicCollectable*)obj)->Collect(ply);
			else
				((StaticCollectable*)obj)->Collect(ply);
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

			Point prevOverlap = GetPreviousOverlap(obj2->GetAABB(), dynObj);

			DynamicObjectToBoxResolutions(GetFacingDirection(dynObj), prevOverlap, dynObj, obj2->GetAABB());
		}
	}
	else
	{
		if (IsDynamicObject(obj1->GetID()) && IsDynamicObject(obj2->GetID()))
			DynamicObjectToDynamicObjectCollisions((DynamicObject*)obj1, (DynamicObject*)obj2);
	}
}

void CollisionManager::PlayerToQBoxResolutions(Player* ply, QBox* box)
{
	Direction dir = GetFacingDirection(ply);
	Point prevOverlap = GetPreviousOverlap(box->GetAABB(), ply);

	if (dir == Direction::UDIR)
	{
		// the collision came from a vertical direction
		if (prevOverlap.x > 0)
		{
			// collision came from the bottom
			if (prevOverlap.x > prevOverlap.y)
			{
				if (box->GetCanHit())
				{
					box->SetJustHit(true);
					GameManager::Get()->GetWorld()->AddObject(sf::Vector2f(box->GetPosition().x, (box->GetPosition().y - box->GetAABB()->GetExtents().y) + 4.f));

					if (GameConstants::Automated)
						((AutomatedPlayer*)ply)->UpdateFitness(5);
				}
			}
		}
	}

	DynamicObjectToBoxResolutions(dir, prevOverlap, ply, box->GetAABB());
}

void CollisionManager::PlayerToSBoxResolutions(Player* ply, SBox* box)
{
	Direction dir = GetFacingDirection(ply);
	Point prevOverlap = GetPreviousOverlap(box->GetAABB(), ply);

	if (box->GetCanHit())
	{
		if (dir == Direction::UDIR)
		{
			// the collision came from a vertical direction
			if (prevOverlap.x > 0)
			{
				// the collision came from the bottom
				if (prevOverlap.x > prevOverlap.y)
				{
					if (!box->GetJustHit())
						box->SetJustHit(true);
				}
			}
		}
		else if (dir == Direction::DDIR)
		{
			// the collision came from a vertical direction
			if (prevOverlap.x > 0)
			{
				// the collision came from the top
				if (prevOverlap.x < prevOverlap.y)
				{
					if (ply->GetIsSuper() && ply->GetCantSpinJump())
					{
						box->SetJustSmashed(true);

						if (GameConstants::Automated)
							((AutomatedPlayer*)ply)->UpdateFitness(5);
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

	if (enmy->IsPlayerAbove(ply))
	{
		enmy->DecrementLife();
		ply->Bounce();

		if (GameConstants::Automated)
			((AutomatedPlayer*)ply)->UpdateFitness(10);
	}
	else
	{
		if (!ply->GetIfInvulnerable())
		{
			if (ply->GetIsSuper())
			{
				ply->SetInvulnerability();
				ply->SetIsSuper(false);

				if (GameConstants::Automated)
					((AutomatedPlayer*)ply)->UpdateFitness(-10);
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
	// horizontal-only case
	if (prevOverlap.y > 0 && prevOverlap.x <= 0)
	{
		ResolveObjectToBoxHorizontally(obj, box);
		if (IsPlayerObject(obj->GetID()))
			((Player*)obj)->ForceFall();
		return;
	}

	// vertical cases
	if (prevOverlap.x > 0)
	{
		if (dirOfTravel == Direction::UDIR)
		{
			if (resolveUpDir && prevOverlap.x > prevOverlap.y)
			{
				ResolveObjectToBoxBottom(obj, box);
				return;
			}
		}
		else
		{
			ResolveObjectToBoxTop(obj, box);
		}
	}
}

void CollisionManager::DynamicObjectToDynamicObjectResolution(DynamicObject* obj, float tFirst)
{
	// get amount to move the object out of collision
	Point move = Point(std::lerp(obj->GetPrevPosition().x, obj->GetPosition().y, tFirst),
		std::lerp(obj->GetPrevPosition().y, obj->GetPosition().y, tFirst));

	Point amt = obj->GetPosition() - move;

	// resolve collision and invert direction of travel and associated velocity
	obj->Move(amt * (obj->GetDirection() ? 1.f : -1.f));
	obj->SetDirection(!obj->GetDirection());
	obj->SetVelocity(obj->GetVelocity().x * -1, obj->GetVelocity().x * -1);
}

void CollisionManager::DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2)
{
	if (!IsMovingTowards(obj1->GetAABB()->GetPosition(), obj2->GetAABB()->GetPosition(), obj1->GetVelocity(), obj2->GetVelocity()))
		return;

	float tFirst, tLast = 0;
	if (obj1->GetAABB()->IntersectsMoving(obj2->GetAABB(), obj1->GetVelocity(), obj2->GetVelocity(), tFirst, tLast))
	{
		DynamicObjectToDynamicObjectResolution(obj1, tFirst);
		DynamicObjectToDynamicObjectResolution(obj2, tFirst);
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

bool CollisionManager::ResolveObjectToSlopeTop(DynamicObject* obj, Tile* tile)
{
	Line line = (tile->GetType() == DIAGD)
		? tile->GetSlope(1, 0)   // flip for DIAGD
		: tile->GetSlope(0, 1);  // keep for DIAGU

	Circle circle(obj->GetAABB(), 4);
	if (line.IsPointAboveLine(circle.center))
	{
		Capsule capsule(line, 6);
		if (capsule.IntersectsCircle(circle))
		{
			obj->SetOnSlope(true);
			obj->SetOnGround(true);
			return true;
		}
	}

	return false;
}

bool CollisionManager::ResolveObjectToSlopeIncline(DynamicObject* obj, Tile* tile, int start, int end)
{
	Line line = tile->GetSlope(start, end);
	Circle circle(obj->GetAABB(), 4);
	Capsule capsule(line, 6);
	if (capsule.IntersectsCircle(circle))
	{
		auto yOffset = GetYOffSet(
			start ? GetXDist(circle.center, line.start) : GetXDist(line.start, circle.center),
			line.DistY(),
			line.start.y,
			obj->GetAABB()->GetPosition().y,
			tile->GetTileHeight());

		// Convert back to pixels using the same scale used by GetYOffSet
		float yPixels = yOffset * tile->GetTileHeight();

		// Small threshold avoids micro-jitter; clamp avoids big pops
		if (std::abs(yPixels) > 0.1f) {
			yPixels = std::clamp(yPixels, -6.0f, 6.0f);
			obj->Move(sf::Vector2f(0, yPixels));
			obj->SetOnSlope(true);
			return true;
		}
	}

	return false;
}

bool CollisionManager::ResolveObjectToSlopeDecline(DynamicObject* obj, Tile* tile, int start, int end)
{
	Line line = tile->GetSlope(start, end);
	Circle circle(obj->GetAABB(), 4);
	Capsule capsule(line, 6);
	if (!capsule.IntersectsCircle(circle))
	{
		auto yOffset = GetYOffSet(
			start ? GetXDist(circle.center, line.start) : GetXDist(line.start, circle.center),
			line.DistY(),
			line.start.y,
			obj->GetAABB()->GetPosition().y,
			tile->GetTileHeight());

		float yPixels = -yOffset * tile->GetTileHeight(); // your original negative push on decline

		if (std::abs(yPixels) > 0.1f) {
			yPixels = std::clamp(yPixels, -6.0f, 6.0f);
			obj->Move(sf::Vector2f(0, yPixels));
			obj->SetOnSlope(true);
			return true;
		}
	}

	return false;
}

void CollisionManager::DynamicObjectToEdgeBounds(DynamicObject* obj, Tile* tile)
{
	if (IsPlayerObject(obj->GetID()))
		return;

	Point side;
	if (tile->GetType() == LCRN)
		side = obj->GetAABB()->GetPoint(Side::Right);
	else
		side = obj->GetAABB()->GetPoint(Side::Left);

	Line edge = tile->GetEdge();

	if (IsMovingTowards(edge.start, side, Point(0, 0), obj->GetVelocity()))
	{
		Circle circle(side, 4);
		Capsule capsule(edge, 4);
		if (capsule.IntersectsCircle(circle))
			obj->SetDirection(!obj->GetDirection());
	}
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