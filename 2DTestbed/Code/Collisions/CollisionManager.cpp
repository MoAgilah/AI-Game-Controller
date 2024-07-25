#include "CollisionManager.h"

#include "../Game/Camera.h"
#include "../Game/Game.h"

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
	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith)
	{
		std::sort(collidedWith.begin(), collidedWith.end(), [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b)
			{
				if (a->GetColNum() == b->GetColNum())
					return a->GetRowNum() < b->GetRowNum();

				return a->GetColNum() > b->GetColNum();
			});
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
	for (auto tile : m_tiles)
		tile->GetAABB()->SetHit(false);

	int id = (int)gobj->GetID();
	if (id >= PlyBgn && id <= (int)TexID::Goal)
	{
		bool collided = false;
		std::vector<std::shared_ptr<Tile>> collidedWith;

		for (auto tile : m_tiles)
		{
			if (!tile->GetActive())
				continue;

			if (tile->GetAABB()->Intersects(gobj->GetAABB()))
				collidedWith.push_back(tile);
		}

		if (collided = !collidedWith.empty())
		{
			if (gobj->GetDirection())
				SortCollidedTiles(collidedWith);

			for (auto tile : collidedWith)
				if (tile->GetAABB()->Intersects(gobj->GetAABB()))
					DynamicObjectToTile((DynamicObject*)gobj, tile.get());
		}

		if (!collided)
			((DynamicObject*)gobj)->SetOnGround(false);
	}

	for (int g = 0; g < m_collidables.size(); ++g)
	{
		if (gobj->GetObjectNum() == m_collidables[g]->GetObjectNum())
			continue;

		if (!m_collidables[g]->GetActive())
			continue;

		if (gobj->GetAABB()->Intersects(m_collidables[g]->GetAABB()))
		{
			ColObjectToColObject(gobj, m_collidables[g].get());
			break;
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

void CollisionManager::DynamicObjectToTile(DynamicObject* obj, Tile* tile)
{
	int dir = GetDirTravelling(obj);

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
				{
					obj->Move(0, -tile->GetAABB()->GetOverlap().y);
					obj->SetOnGround(true);
					return;
				}
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
					obj->Move((obj->GetDirection() ? -1 : 1) * tile->GetAABB()->GetOverlap().x, 0);
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
					obj->Move(0, -box.GetOverlap().y);
					obj->SetOnGround(true);
					return;
				}
			}
		}
	}

	obj->SetOnGround(false);
}

void CollisionManager::PlayerToEnemy(Player * ply, Enemy * enmy)
{
	float pBot = ply->GetAABB()->GetPosition().y + ply->GetAABB()->GetOrigin().y;
	float eTop = enmy->GetAABB()->GetPosition().y - enmy->GetAABB()->GetOrigin().y;

	bool col = false;
	if (enmy->GetID() == TexID::Bill)
	{
		Bill* bill = (Bill*)enmy;

		//check for collision with inner sphere
		if (CircleToRect(bill->GetBody().front, ply))
		{
			col = true;
		}
		//check for collision with back rect
		else if (ply->GetAABB()->GetRect().getGlobalBounds().intersects(bill->GetBody().back.getGlobalBounds()))
		{
			col = true;
		}
	}
	else
	{
		col = true;
	}

	if (col)
	{
		if (enmy->GetIsAlive())
		{
			//if player is not above enemy and enemy is piranha plant
			if (pBot > eTop || enmy->GetID() == TexID::PPlant)
			{
				//if not vulnerable
				if (!ply->GetIfInvulnerable())
				{
					if (ply->GetIsAlive())
					{
						if (ply->GetIsSuper())
						{
							//ptmp->UpdateFitness(-100);
							ply->JustBeenHit(true);
							ply->SetIsSuper(false);
						}
						else
						{
							ply->SetIsAlive(false);
						}
					}
				}
			}
			else
			{
				if (ply->GetIsAlive())
				{
					//set hover time
					ply->JusyHitEnemy(0.1f);
					enmy->DecrementLife();
				}
			}
		}
	}
}

void CollisionManager::PlayerToObject(Player * ply, Object * obj)
{
	sf::Vector2f pos;
	switch (obj->GetID())
	{
	case TexID::Shroom://super mushroom
		ply->SetIsSuper(true);
		obj->SetActive(false);
		((Mushroom*)obj)->SetCollected();
		//ply->UpdateFitness(200);
		break;
	case TexID::Coin:
		((Coin*)obj)->SetCollected();
		ply->IncreaseCoins(((Coin*)obj)->Collect());
		//ply->UpdateFitness(10);
		obj->SetActive(false);
		break;
	case TexID::YCoin://yoshi coin
		((YCoin*)obj)->SetCollected();
		ply->IncreaseCoins(((YCoin*)obj)->Collect());
		//ply->UpdateFitness(100);
		obj->SetActive(false);
		break;
	case TexID::QBox://question mark box
		QBoxHit(ply, (QBox*)obj);
		break;
	case TexID::SBox://spin box
		SBoxHit(ply, (SBox*)obj);
		break;
	case TexID::ChkPnt://check point
		((CheckPoint*)obj)->SetCollected();
		ply->SetSpawnLoc(obj->GetPosition());
		//ply->UpdateFitness(200);
		ply->SetIsSuper(true);
		obj->SetActive(false);
		break;
	case TexID::Goal://end goal
		((Goal*)obj)->SetCollected();
		//ply->UpdateFitness(200);
		ply->GoalHit();
		break;
	default:
		std::cout << "Unknown type!" << std::endl;
		break;
	}
}

void CollisionManager::EnemyToEnemy(Enemy * enmy1, Enemy* enmy2)
{
	bool enmy2Dir = enmy1->GetDirection();
	enmy1->SetDirection(enmy2->GetDirection());
	enmy2->SetDirection(enmy2Dir);

	switch (GetDirTravelling(enmy1))
	{
	case RDIR:
		if (enmy1->GetID() == TexID::Rex)
		{
			if (enmy2->GetID() == TexID::Rex)//regular
			{
				//resolve collision
				enmy1->SetPosition(sf::Vector2f((enmy2->GetPosition().x - enmy2->GetOrigin().x * sX) - (enmy1->GetOrigin().x * sX) + 8, enmy1->GetPosition().y));
			}
			else//squished
			{
				//resolve collision
				enmy1->SetPosition(sf::Vector2f((enmy2->GetPosition().x - enmy2->GetOrigin().x * sX) - (enmy1->GetOrigin().x * sX) + 7.f, enmy1->GetPosition().y));
			}
		}
		else
		{
			//resolve collision
			enmy1->SetPosition(sf::Vector2f((enmy2->GetPosition().x - enmy2->GetOrigin().x * sX) - (enmy1->GetOrigin().x * sX) + 7.5f, enmy1->GetPosition().y));
		}
		break;
	case LDIR:
		if (enmy1->GetID() == TexID::Rex)
		{
			if (enmy2->GetID() == TexID::Rex)//regular
			{
				//resolve collision
				enmy1->SetPosition(sf::Vector2f((enmy2->GetPosition().x - enmy2->GetOrigin().x * sX) - (enmy1->GetOrigin().x * sX) - 8.f, enmy1->GetPosition().y));
			}
			else//suqished
			{
				//resolve collision
				enmy1->SetPosition(sf::Vector2f((enmy2->GetPosition().x + enmy2->GetOrigin().x * sX) + (enmy1->GetOrigin().x * sX) - 7.f, enmy1->GetPosition().y));
			}
		}
		else
		{
			//resolve collision
			enmy1->SetPosition(sf::Vector2f((enmy2->GetPosition().x + enmy2->GetOrigin().x * sX) + (enmy1->GetOrigin().x * sX) - 7.5f, enmy1->GetPosition().y));
		}
		break;
	};
}

void CollisionManager::ColObjectToColObject(Object * colObj1, Object * colObj2)
{
	int col1Typ = (int)colObj1->GetID();
	int col2Typ = (int)colObj2->GetID();

	int isPlayer = -1;

	//if either is a player assign id num
	if (col1Typ >= PlyBgn && col1Typ <= PlyEnd) isPlayer = 1;
	else if (col2Typ >= PlyBgn && col2Typ <= PlyEnd) isPlayer = 2;

	if (isPlayer == 1)//if player is obj 1
	{
		if (col1Typ >= EnmyBgn && col1Typ <= EnmyEnd)
		{
			PlayerToEnemy((Player*)colObj1, (Enemy*)colObj2);
		}
		else if (col2Typ >= ColBgn && col2Typ <= ObjEnd)
		{
			PlayerToObject((Player*)colObj1, colObj2);
		}
	}
	else if (isPlayer == 2)//if player is obj 2
	{
		if (col1Typ >= EnmyBgn && col1Typ <= EnmyEnd)
		{
			PlayerToEnemy((Player*)colObj2, (Enemy*)colObj1);
		}
		else if (col1Typ >= ColBgn && col1Typ <= ObjEnd)
		{
			PlayerToObject((Player*)colObj2, colObj1);
		}
	}
	else // if neither are the player
	{
		//if both are enemies
		if ((col1Typ >= EnmyBgn && col1Typ <= EnmyEnd) && (col2Typ >= EnmyBgn && col2Typ <= EnmyEnd))
		{
			if (colObj1->GetActive() && colObj2->GetActive())
			{
				EnemyToEnemy((Enemy*)colObj1, (Enemy*)colObj2);
			}
		}
		else if ((col1Typ == (int)TexID::Shroom) && (col2Typ >= ObjBgn && col2Typ <= ObjEnd))
		{
			if (colObj1->GetActive() && colObj2->GetActive())
			{
				QBoxHit((Mushroom*)colObj1, (QBox*)colObj2);
			}
		}
	}
}

int CollisionManager::GetDirTravelling(DynamicObject* obj)
{
	//direction travelling
	sf::Vector2f dirV = obj->GetPosition() - obj->GetPrevPostion();
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

void CollisionManager::QBoxHit(Player * ply, QBox* box)
{
	switch (GetDirTravelling(ply))
	{
	case UDIR:
		if (box->GetCanHit())//if not yet been hit
		{
			//ply->UpdateFitness(100);
			//add to the level
			Game::GetGameMgr()->GetLevel()->AddObject(sf::Vector2f(box->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * sY) - (box->GetOrigin().y * sY) + 4.f));

			box->SetJustHit(true);
		}
		//resove collision
		ply->SetPosition(sf::Vector2f(ply->GetPosition().x, (box->GetPosition().y + box->GetOrigin().y * sY) + (ply->GetOrigin().y * sY) + 4.f));
		ply->ForceFall();//make mario fall
		break;
	case DDIR:
		//set to top of qbox
		ply->SetPosition(sf::Vector2f(ply->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * sY) - (ply->GetOrigin().y * sY) + 4.f));
		ply->SetOnGround(true);
		break;
	case RDIR:
		//set to right of qbox
		ply->SetPosition(sf::Vector2f((box->GetPosition().x - box->GetOrigin().x * sX) - (ply->GetOrigin().x * sX) + 7.5f, ply->GetPosition().y));
		break;
	case LDIR:
		//set to left of qbox
		ply->SetPosition(sf::Vector2f((box->GetPosition().x + box->GetOrigin().x * sX) + (ply->GetOrigin().x * sX) - 7.5f, ply->GetPosition().y));
		break;
	}
}

void CollisionManager::QBoxHit(Mushroom* shm, QBox* box)
{
	switch (GetDirTravelling(shm))
	{
	case DDIR:
		//set to top of qbox
		shm->SetPosition(sf::Vector2f(shm->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * sY) - (shm->GetOrigin().y * sY) + 4.f));
		shm->SetOnGround(true);
		break;
	}
}

void CollisionManager::SBoxHit(Player * ply, SBox* box)
{
	if (box->GetCanHit())//if not yet been hit
	{
		switch (GetDirTravelling(ply))
		{
		case UDIR://if going up
			if (!box->GetJustHit())
				box->SetJustHit(true);
			break;
		case DDIR://if falling
			//land on object
			if (ply->GetIsSuper() && ply->GetCantSpinJump())
			{
				box->SetJustSmashed(true);
			}
			else
			{
				ply->SetPosition(sf::Vector2f(ply->GetPosition().x, (box->GetPosition().y - box->GetOrigin().y * sY) - (ply->GetOrigin().y * sY) + 4.f));
				ply->SetOnGround(true);
			}
			break;
		}
	}
}

bool CollisionManager::CircleToRect(sf::CircleShape circle, Player* ply)
{
	//convert object into sphere
	sf::Vector2f Obj1Size = sf::Vector2f(ply->GetAABB()->GetOrigin().x * 2, ply->GetAABB()->GetOrigin().y * 2);

	Obj1Size.x *= sX;
	Obj1Size.y *= sY;

	float Radius1 = (Obj1Size.x + Obj1Size.y) * 0.25f;

	float Radius2 = circle.getRadius();

	//collision check
	sf::Vector2f Distance = ply->GetAABB()->GetPosition() -  circle.getPosition();

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}

