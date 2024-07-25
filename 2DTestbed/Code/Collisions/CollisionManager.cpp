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

	std::vector<std::shared_ptr<Tile>> collidedWith;

	//check for collision with tilemap
	bool Col = false;
	for (auto tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetAABB()->Intersects(gobj->GetAABB()))
			collidedWith.push_back(tile);
	}

	if (!collidedWith.empty())
	{
		Col = true;
		for (auto tile : collidedWith)
			ColObjectToTile(gobj, tile.get());
	}


	if (!Col)
	{
		int id = (int)gobj->GetID();
		if (id >= PlyBgn && id <= PlyEnd)
		{
			((Player*)gobj)->SetOnGround(false);
		}
		else if (id >= EnmyBgn && id <= EnmyEnd)
		{
			((Enemy*)gobj)->SetOnGround(false);
		}
		else if (id == (int)TexID::Shroom)
		{
			((Mushroom*)gobj)->SetOnGround(false);
		}
	}

	for (int g = 0; g < m_collidables.size(); ++g)
	{
		if (gobj->GetObjectNum() == m_collidables[g]->GetObjectNum())
			continue;

		if (!m_collidables[g]->GetActive())
			continue;

		Col = gobj->GetAABB()->Intersects(m_collidables[g]->GetAABB());

		if (Col)
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

void CollisionManager::PlayerToTile(Player* ply, Tile * tile)
{
	int dir = GetDirTravelling(ply);

	if (tile->GetType() == GRND)
	{
		//if falling
		if (dir == DDIR)
		{
			//move to top of tile
			ply->Move(0, -tile->GetAABB()->GetOverlap().y);
			ply->SetOnGround(true);
			return;
		}
	}

	//oway is a one way tile can be fell upon and jumped through
	if (tile->GetType() == OWAY)
	{
		float plyBot = ply->GetAABB()->GetPosition().y + ply->GetOrigin().y;
		float tiletop = tile->GetPosition().y - tile->GetOrigin().y;

		//if above the tile
		if (plyBot < tiletop)
		{
			//if falling
			if (dir == DDIR)
			{
				//move to top of tile
				ply->Move(0, -tile->GetAABB()->GetOverlap().y);
				ply->SetOnGround(true);
				return;
			}
		}
	}

	//corner tile or wall tile
	if (tile->GetType() == CRN)
	{
		switch (dir)
		{
		case DDIR:
		{
			float plyBot = ply->GetAABB()->GetPosition().y + ply->GetOrigin().y;
			float tiletop = tile->GetPosition().y - tile->GetOrigin().y;

			//if above the tile
			if (plyBot < tiletop)
			{
				//move to top of tile
				ply->Move(0, -tile->GetAABB()->GetOverlap().y);
				ply->SetOnGround(true);
				return;
			}
		}
		case RDIR:
			if (ply->GetAirbourne())
				ply->Move(-tile->GetAABB()->GetOverlap().x, 0);
			return;
		case LDIR:
			if (ply->GetAirbourne())
				ply->Move(tile->GetAABB()->GetOverlap().x, 0);
			return;
		}
	}

	//corner tile or wall tile
	if (tile->GetType() == WALL)
	{
		switch (dir)
		{
		case RDIR:
			ply->Move(-tile->GetAABB()->GetOverlap().x, 0);
			return;
		case LDIR:
			ply->Move(tile->GetAABB()->GetOverlap().x, 0);
			return;
		}
	}

	//if travelling up or down a slope
	if (tile->GetType() == DIAGU || tile->GetType() == DIAGD)
	{
		//extract slope
		std::vector<AABB> tmpSlope = tile->GetSlopeBBox();

		bool colFound = false;
		for (int i = 0; i < tmpSlope.size(); i++)
		{
			//if collision with slope
			if (ply->GetAABB()->Intersects(&tmpSlope[i]))
			{
				switch (dir)
				{
				case DDIR:
					//set to slope top
					//move to top of tile
					ply->Move(0, -tile->GetAABB()->GetOverlap().y);
					ply->SetOnGround(true);
					colFound = true;
					return;
				}
			}
		}
	}

	ply->SetOnGround(false);
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

void CollisionManager::ObjectToTile(DynamicObject* obj, Tile * tile)
{
	//ground and one way platforms
	if (tile->GetType() == GRND || tile->GetType() == OWAY)
	{
		if(GetDirTravelling(obj) == DDIR)
		{
			if (obj->GetID() == TexID::Rex || tile->GetType() == OWAY)
			{
				Rex* rtmp = (Rex*)obj;
				if (rtmp->Tall())//if regular
				{
					//set to tile top
					rtmp->SetPosition(sf::Vector2f(obj->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (obj->GetOrigin().y * sY) + 3.5f));
				}
				else//if squished
				{
					//set to tile top
					rtmp->SetPosition(sf::Vector2f(obj->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (obj->GetOrigin().y * sY) + 4.f));
				}
			}
			else if (obj->GetID() == TexID::Shroom || obj->GetID() == TexID::Goal)
			{
				((DynamicCollectable*)obj)->SetOnGround(true);
				obj->SetPosition(sf::Vector2f(obj->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (obj->GetOrigin().y * sY)));
			}
			else
			{
				//set to tile top
				obj->SetPosition(sf::Vector2f(obj->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (obj->GetOrigin().y * sY) + 2.5f));
			}

			if ((int)obj->GetID() >= (int)EnmyBgn && (int)obj->GetID() <= (int)EnmyEnd)
			{
				((Enemy*)obj)->SetOnGround(true);
			}
		}

		return;
	}

	//corner tile or wall tile
	if (tile->GetType() == CRN || tile->GetType() == WALL)
	{
		switch (GetDirTravelling(obj))
		{
		case RDIR:
			if (obj->GetID() == TexID::Rex)
			{
				Rex* rtmp = (Rex*)obj;
				if (rtmp->Tall())//regular
				{
					//set to minimum closest dist
					rtmp->SetPosition(sf::Vector2f((tile->GetPosition().x - tile->GetOrigin().x * sX) - (obj->GetOrigin().x * sX) + 7, obj->GetPosition().y));
				}
				else//squished
				{
					//set to minimum closest dist
					rtmp->SetPosition(sf::Vector2f((tile->GetPosition().x - tile->GetOrigin().x * sX) - (obj->GetOrigin().x * sX) + 3, obj->GetPosition().y));
				}
			}
			else if (obj->GetID() == TexID::Shroom)
			{
				//set to minimum closest dist
				obj->SetPosition(sf::Vector2f((tile->GetPosition().x - tile->GetOrigin().x * sX) - (obj->GetOrigin().x * sX) -4.f, obj->GetPosition().y));
			}
			else
			{
				//set to minimum closest dist
				obj->SetPosition(sf::Vector2f((tile->GetPosition().x - tile->GetOrigin().x * sX) - (obj->GetOrigin().x * sX) + 7.5f, obj->GetPosition().y));
			}

			//flip direction
			if (obj->GetDirection())
			{
				obj->SetDirection(false);
			}
			else
			{
				obj->SetDirection(true);
			}

			break;
		case LDIR:
			if (obj->GetID() == TexID::Rex)
			{
				Rex* rtmp = (Rex*)obj;
				if (rtmp->Tall())//regular
				{
					//set to minimum closest dist
					rtmp->SetPosition(sf::Vector2f((tile->GetPosition().x + tile->GetOrigin().x * sX) + (obj->GetOrigin().x * sX) - 7.f, obj->GetPosition().y));
				}
				else//squished
				{
					//set to minimum closest dist
					rtmp->SetPosition(sf::Vector2f((tile->GetPosition().x + tile->GetOrigin().x * sX) + (obj->GetOrigin().x * sX) - 3.f, obj->GetPosition().y));
				}
			}
			else if (obj->GetID() == TexID::Shroom)
			{
				//set to minimum closest dist
				obj->SetPosition(sf::Vector2f((tile->GetPosition().x + tile->GetOrigin().x * sX) + (obj->GetOrigin().x * sX) + 4.f, obj->GetPosition().y));
			}
			else
			{
				//set to minimum closest dist
				obj->SetPosition(sf::Vector2f((tile->GetPosition().x + tile->GetOrigin().x * sX) + (obj->GetOrigin().x * sX) - 7.5f, obj->GetPosition().y));
			}

			//flip direction
			if (obj->GetDirection())
			{
				obj->SetDirection(false);
			}
			else
			{
				obj->SetDirection(true);
			}

			break;
		}

		return;
	}

	//if travelling up or down a slope
	if (tile->GetType() == DIAGU || tile->GetType() == DIAGD)
	{
		//extract slope
		std::vector<AABB> tmpSlope = tile->GetSlopeBBox();
		bool colFound = false;
		for (int i = 0; i < tmpSlope.size(); i++)
		{
			if (obj->GetAABB()->Intersects(&tmpSlope[i]))
			{
				switch (GetDirTravelling(obj))
				{
				case DDIR:
					obj->SetPosition(sf::Vector2f(obj->GetPosition().x, tmpSlope[i].GetPosition().y - tmpSlope[i].GetOrigin().y * sY - obj->GetOrigin().y * sX));
					if ((int)obj->GetID() >= (int)EnmyBgn && (int)obj->GetID() <= (int)EnmyEnd)
					{
						((Enemy*)obj)->SetOnGround(true);
					}
					else if (obj->GetID() == TexID::Shroom)
					{
						((Mushroom*)obj)->SetOnGround(true);
					}
					colFound = true;
					break;
				case UDIR:
					break;
				}
			}
		}

		if (!colFound)
		{
			if ((int)obj->GetID() >= (int)EnmyBgn && (int)obj->GetID() <= (int)EnmyEnd)
			{
				((Enemy*)obj)->SetOnGround(false);
			}
			else if (obj->GetID() == TexID::Shroom)
			{
				((Mushroom*)obj)->SetOnGround(false);
			}
		}

		return;
	}
}

void CollisionManager::ColObjectToTile(Object * c_obj, Tile * tile)
{
	int id = (int)c_obj->GetID();
	if (id >= PlyBgn && id <= PlyEnd)
	{
		PlayerToTile((Player*)c_obj, tile);
	}
	else if (id >= EnmyBgn && id <= EnmyEnd)
	{
		ObjectToTile((DynamicObject*)c_obj, tile);
	}
	else if (id >= (int)TexID::Shroom && id <= (int)TexID::Goal)
	{
		ObjectToTile((DynamicObject*)c_obj, tile);
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

