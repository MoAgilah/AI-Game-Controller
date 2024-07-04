#include "Collisions.h"

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

Collisions* Collisions::instance = nullptr;

Collisions::Collisions()
{
	m_grid = grid.GetGrid();
}

Collisions::~Collisions()
{
	if (m_collidables.empty() == false)
	{
		for (int i = 0; i < m_collidables.size(); ++i)
		{
			delete m_collidables[i];
			m_collidables[i] = nullptr;
		}
	}

	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

Collisions* Collisions::Get()
{
	if (instance == nullptr)
	{
		instance = new Collisions();
	}

	return instance;
}

void Collisions::AddCollidable(Object* bbox)
{
	m_collidables.push_back(bbox);
}

void Collisions::RemoveLastAdded()
{
	m_collidables.pop_back();
}

void Collisions::ReplacePlayer(Player * currPlayer)
{
	m_collidables[0] = currPlayer;
}

Object* Collisions::GetLastAdded()
{
	return m_collidables.back();
}

void Collisions::Render(sf::RenderWindow& window)
{
	const Camera* camera = Game::GetGameMgr()->GetCamera();

	for (int i = 0; i < m_collidables.size(); ++i)
	{
		if (m_collidables[i]->GetActive())
		{
			bool inView = camera->IsInView(m_collidables[i]->GetBBox()->GetSprite());

			if (inView)
			{
	#ifdef DRender
					m_collidables[i]->GetBBox()->Render(window);
	#endif //DRender
			}
		}
	}

	camera = nullptr;

	grid.Render(window);
}

void Collisions::ProcessCollisions(Object* gobj)
{
	int numchecks = 0;
	//check for collision with tilemap
	bool Col = false;
	for (int t = 0; t < m_grid.size(); ++t)
	{
		Tile* tile = m_grid[t];

		if (tile->GetType() == EMPTY || gobj->GetID() == TexID::ChkPnt || gobj->GetID() == TexID::Goal)
		{
			continue;
		}

		if (!tile->GetActive()) continue;

		numchecks++;
		Col = gobj->GetBBox()->Intersects(*tile);

		if (Col)
		{
			++numchecks;
			ColObjectToTile(gobj, tile);
			break;
		}
	}

	if (!Col)
	{
		int id = gobj->GetBBox()->GetID();
		if (id >= PlyBgn && id <= PlyEnd)
		{
			((Player*)gobj)->SetOnGround(false);
		}
		else if (id >= EnmyBgn && id <= EnmyEnd)
		{
			((Enemy*)gobj)->SetOnGround(false);
		}
		else if (id == (int)TexID::ShroomBB)
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

		Col = gobj->GetBBox()->Intersects(m_collidables[g]->GetBBox());

		if (Col)
		{
			ColObjectToColObject(gobj, m_collidables[g]);
			break;
		}
	}
}

Tile Collisions::GetTile(int x, int y)
{
	return *grid.GetTile(x, y);
}

std::vector<Tile*> Collisions::GetGrid()
{
	return grid.GetGrid();
}

std::vector<Object*> Collisions::GetCollidables()
{
	return m_collidables;
}

void Collisions::PlayerToTile(Player* ply, Tile * tile)
{
	int dir = GetDirTravelling(ply);

	if (tile->GetType() == GRND)
	{
		//if falling
		if (dir == DDIR)
		{
			//move to top of tile
			ply->SetPosition(sf::Vector2f(ply->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (ply->GetOrigin().y * sY) +4.f));
			ply->SetOnGround(true);
			return;
		}
	}

	//oway is a one way tile can be fell upon and jumped through
	if (tile->GetType() == OWAY)
	{
		float plyBot = ply->GetBBox()->GetSprite()->getPosition().y + ply->GetOrigin().y;
		float tiletop = tile->GetPosition().y - tile->GetOrigin().y;

		//if above the tile
		if (plyBot < tiletop)
		{
			//if falling
			if (dir == DDIR)
			{
				//move to top of tile
				ply->SetPosition(sf::Vector2f(ply->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (ply->GetOrigin().y * sY) + 4.f));
				ply->SetOnGround(true);
				return;
			}
		}
		else
		{
			if (ply->GetIsSuper())
			{
				//error occured with super mario colliding with tile above him when on the ground
				//this skips collision with that tile as way to alleiviate the problem
				if (tile->GetID() == "1794")
				{
					ply->SetOnGround(false);
				}
			}
		}
	}

	//corner tile or wall tile
	if (tile->GetType() == CRN || tile->GetType() == WALL)
	{
		switch (dir)
		{
		case DDIR:
			//move to tile top
			ply->SetPosition(sf::Vector2f(ply->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (ply->GetOrigin().y * sY) + 4.f));
			ply->SetOnGround(true);
			return;
		case RDIR:
			//move to closest point without a collision to remove jittering
			ply->SetPosition(sf::Vector2f((tile->GetPosition().x - tile->GetOrigin().x * sX) - (ply->GetOrigin().x * sX) + 7.5f, ply->GetPosition().y));
			return;
		case LDIR:
			//move to closest point without a collision to remove jittering
			ply->SetPosition(sf::Vector2f((tile->GetPosition().x + tile->GetOrigin().x * sX) + (ply->GetOrigin().x * sX) - 7.5f, ply->GetPosition().y));
			return;
		}
	}

	//if travelling up or down a slope
	if (tile->GetType() == DIAGU || tile->GetType() == DIAGD)
	{
		//extract slope
		std::vector<sf::RectangleShape> tmpSlope = tile->GetSlopeBBox();

		bool colFound = false;
		for (int i = 0; i < tmpSlope.size(); i++)
		{
			//if collision with slope
			if (ply->GetBBox()->GetSprite()->getGlobalBounds().intersects(tmpSlope[i].getGlobalBounds()))
			{
				switch (dir)
				{
				case DDIR:
					//set to slope top
					ply->SetPosition(sf::Vector2f(ply->GetPosition().x, tmpSlope[i].getPosition().y - tmpSlope[i].getOrigin().y * sY - (ply->GetOrigin().y * sX)));
					ply->SetOnGround(true);
					colFound = true;
					return;
				}
			}
		}
	}

	ply->SetOnGround(false);
}

void Collisions::PlayerToEnemy(Player * ply, Enemy * enmy)
{
	float pBot = ply->GetBBox()->GetSprite()->getPosition().y + ply->GetBBox()->GetSprite()->getOrigin().y;
	float eTop = enmy->GetBBox()->GetSprite()->getPosition().y - enmy->GetBBox()->GetSprite()->getOrigin().y;

	bool col = false;
	if (enmy->GetBBox()->GetID() == (int)TexID::BillBB)
	{
		Bill* bill = (Bill*)enmy;

		//check for collision with inner sphere
		if (CircleToRect(bill->GetBody().front, ply))
		{
			col = true;
		}
		//check for collision with back rect
		else if (ply->GetBBox()->GetSprite()->getGlobalBounds().intersects(bill->GetBody().back.getGlobalBounds()))
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

void Collisions::PlayerToObject(Player * ply, Object * obj)
{
	sf::Vector2f pos;
	switch (obj->GetID())
	{
	case TexID::Shroom://super mushroom
		ply->SetIsSuper(true);
		obj->SetActive(false);
		//ply->UpdateFitness(200);
		break;
	case TexID::YCoin://yoshi coin
		ply->IncreaseCoins(5);
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
		ply->SetSpawnLoc(obj->GetPosition());
		//ply->UpdateFitness(200);
		ply->SetIsSuper(true);
		obj->SetActive(false);
		break;
	case TexID::Goal://end goal
		//ply->UpdateFitness(200);
		ply->GoalHit();
		break;
	default:
		std::cout << "Unknown type!" << std::endl;
		break;
	}
}

void Collisions::ObjectToTile(DynamicObject* obj, Tile * tile)
{
	//ground and one way platforms
	if (tile->GetType() == GRND || tile->GetType() == OWAY)
	{
		if(GetDirTravelling(obj) == DDIR)
		{
			if (obj->GetBBox()->GetID() == (int)TexID::RexBB || tile->GetType() == OWAY)
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
			else
			{
				//set to tile top
				obj->SetPosition(sf::Vector2f(obj->GetPosition().x, (tile->GetPosition().y - tile->GetOrigin().y * sY) - (obj->GetOrigin().y * sY) + 2.5f));
			}

			((Enemy*)obj)->SetOnGround(true);
		}

		return;
	}

	//corner tile or wall tile
	if (tile->GetType() == CRN || tile->GetType() == WALL)
	{
		switch (GetDirTravelling(obj))
		{
		case RDIR:
			if (obj->GetBBox()->GetID() == (int)TexID::RexBB)
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
			else if (obj->GetBBox()->GetID() == (int)TexID::ShroomBB)
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
			if (obj->GetBBox()->GetID() == (int)TexID::RexBB)
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
			else if (obj->GetBBox()->GetID() == (int)TexID::ShroomBB)
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
		std::vector<sf::RectangleShape> tmpSlope = tile->GetSlopeBBox();
		bool colFound = false;
		for (int i = 0; i < tmpSlope.size(); i++)
		{
			if (obj->GetBBox()->GetSprite()->getGlobalBounds().intersects(tmpSlope[i].getGlobalBounds()))
			{
				switch (GetDirTravelling(obj))
				{
				case DDIR:
					obj->SetPosition(sf::Vector2f(obj->GetPosition().x, tmpSlope[i].getPosition().y - tmpSlope[i].getOrigin().y*sY - obj->GetOrigin().y*sX));
					if ((int)obj->GetID() >= (int)EnmyBgn && (int)obj->GetID() <= (int)EnmyEnd)
					{
						((Enemy*)obj)->SetOnGround(true);
					}
					else if (obj->GetID() == TexID::ShroomBB)
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
			else if (obj->GetID() == TexID::ShroomBB)
			{
				((Mushroom*)obj)->SetOnGround(false);
			}
		}

		return;
	}
}

void Collisions::ColObjectToTile(Object * c_obj, Tile * tile)
{
	int id = c_obj->GetBBox()->GetID();
	if (id >= PlyBgn && id <= PlyEnd)
	{
		PlayerToTile((Player*)c_obj, tile);
	}
	else if (id >= EnmyBgn && id <= EnmyEnd)
	{
		ObjectToTile((DynamicObject*)c_obj, tile);
	}
	else if (id == (int)TexID::ShroomBB)
	{
		ObjectToTile((DynamicObject*)c_obj, tile);
	}
}

void Collisions::EnemyToEnemy(Enemy * enmy1, Enemy* enmy2)
{
	bool enmy2Dir = enmy1->GetDirection();
	enmy1->SetDirection(enmy2->GetDirection());
	enmy2->SetDirection(enmy2Dir);

	switch (GetDirTravelling(enmy1))
	{
	case RDIR:
		if (enmy1->GetBBox()->GetID() == (int)TexID::RexBB)
		{
			if (enmy2->GetBBox()->GetID() == (int)TexID::RexBB)//regular
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
		if (enmy1->GetBBox()->GetID() == (int)TexID::RexBB)
		{
			if (enmy2->GetBBox()->GetID() == (int)TexID::RexBB)//regular
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

void Collisions::ColObjectToColObject(Object * colObj1, Object * colObj2)
{
	int col1Typ = colObj1->GetBBox()->GetID();
	int col2Typ = colObj2->GetBBox()->GetID();

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
	}
}

int Collisions::GetDirTravelling(DynamicObject* obj)
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

void Collisions::QBoxHit(Player * ply, QBox* box)
{
	switch (GetDirTravelling(ply))
	{
	case UDIR:
		if (box->GetCanHit())//if not yet been hit
		{
			//ply->UpdateFitness(100);
			//add to the level
			//Game::GetGameMgr()->GetLevel()->AddObject(obj->GetPosition() - sf::Vector2f(0, (obj->GetOrigin().y * sY) * 2.f - 20.f));

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

void Collisions::SBoxHit(Player * ply, SBox* box)
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

bool Collisions::CircleToRect(sf::CircleShape circle, Player* ply)
{
	//convert object into sphere
	sf::Sprite* pspr = ply->GetBBox()->GetSprite();
	sf::Vector2f Obj1Size = sf::Vector2f(pspr->getOrigin().x * 2, pspr->getOrigin().y * 2);

	Obj1Size.x *= sX;
	Obj1Size.y *= sY;

	float Radius1 = (Obj1Size.x + Obj1Size.y) * 0.25f;

	float Radius2 = circle.getRadius();

	//collision check
	sf::Vector2f Distance = pspr->getPosition() -  circle.getPosition();

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}

