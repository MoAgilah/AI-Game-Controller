#include "../Game/Level.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"

#include "../../Collisions/BoundingBox.h"
#include "../../Collisions/Collisions.h"
#include "../GameObjects/Object.h"
#include "../GameObjects/Koopa.h"
#include "../GameObjects/Bill.h"
#include "../GameObjects/Rex.h"
#include "../GameObjects/PPlant.h"
#include "../GameObjects/Chuck.h"
#include <assert.h>

Level::Level()
{
	std::vector<std::string> filenames{ "Sprites/background.png", "Sprites/pipe1.png", "Sprites/pipe2.png", "Sprites/pipe3.png" };

	for (int i = 0; i < (int)SPRID::MAX; i++)
	{
		if (!m_textures[i].loadFromFile(filenames[i]))
		{
			std::cout << "failed to load texture file" << filenames[i] << std::endl;
			continue;
		}

		m_textures[i].setSmooth(true);
		m_sprites[i].setTexture(m_textures[i]);
		m_sprites[i].setScale(sX, sY);
	}

	m_sprites[(int)SPRID::BKG].setPosition(0, -480);

	AddEnemies();
	AddObjects();
}

void Level::Update(float deltaTime)
{
	for (auto& enemy : m_enemy)
		enemy->Update(deltaTime);

	for (auto& object : m_objects)
		object->Update(deltaTime);
}

void Level::Render(sf::RenderWindow & window)
{
	window.draw(m_sprites[(int)SPRID::BKG]);

	for (auto& enemy : m_enemy)
	{
		if (!enemy->GetActive())
			continue;

		if (!enemy->GetVisible())
			continue;

		enemy->Render(window);
	}

	for (auto& object : m_objects)
	{
		if (!object->GetActive())
			continue;

		if (!object->GetVisible())
			continue;

		object->Render(window);
	}

	const Camera* camera = Game::GetGameMgr()->GetCamera();

	for (int i = (int)SPRID::PIPE1; i < (int)SPRID::MAX; i++)
	{
		if (!camera->IsInView(&m_sprites[i]))
			continue;

		window.draw(m_sprites[i]);
	}

	camera = nullptr;
}

void Level::ResetLevel()
{
	for (auto& enemy : m_enemy)
		enemy->Revive();

	for (int i = 14; i < m_objects.size(); i++)
		m_objects.pop_back();

	for (auto& object : m_objects)
		object->Reset();
}

std::vector<Enemy*> Level::GetEnemies()
{
	return m_enemy;
}

std::vector<Object*> Level::GetObjects()
{
	return m_objects;
}

Level::~Level()
{
	if (m_enemy.size() > 0)
	{
		for (int i = 0; i < m_enemy.size(); i++)
		{
			delete m_enemy[i];
			m_enemy[i] = nullptr;
		}
	}

	if (m_objects.size() > 0)
	{
		for (int i = 0; i < m_objects.size(); i++)
		{
			delete m_objects[i];
			m_objects[i] = nullptr;
		}
	}
}

void Level::AddEnemies()
{
	/*Tile tmp = Collisions::Get()->GetTile(32, 5);
	m_enemy.push_back(new Bill("bill.png",1,1,FPS,BILL,1,false,true,0,1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX-10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(33, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(47, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(55, 8);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(72, 7);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(82, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(89, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(112, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(113, 5);
	m_enemy.push_back(new PPlant("pplant.png",2,2,FPS,PPLANT,2,false,false,0,1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	m_sprites[(int)SPRID::PIPE1].setOrigin(m_textures[(int)SPRID::PIPE1].getSize().x / 2.f, m_textures[(int)SPRID::PIPE1].getSize().x / 2.f);
	m_sprites[(int)SPRID::PIPE1].setPosition(m_enemy.back()->GetPosition() + sf::Vector2f(0.f, 40.f));

	tmp = Collisions::Get()->GetTile(138, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(139, 6);
	m_enemy.push_back(new PPlant("pplant.png",2,2,FPS,PPLANT,2,false, false,0,1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	m_sprites[(int)SPRID::PIPE2].setOrigin(m_textures[(int)SPRID::PIPE2].getSize().x / 2.f, m_textures[(int)SPRID::PIPE2].getSize().x / 2.f);
	m_sprites[(int)SPRID::PIPE2].setPosition(m_enemy.back()->GetPosition() + sf::Vector2f(0.f, 40.f));

	tmp = Collisions::Get()->GetTile(172, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(182, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(184, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(186, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(189, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(203, 4);
	m_enemy.push_back(new Bill("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY - 2.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(207, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(227, 4);
	m_enemy.push_back(new Bill("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY - 2.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(242, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(257, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(263, 3);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(280, 4);
	m_enemy.push_back(new Bill("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY - 2.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(284, 6);
	m_enemy.push_back(new PPlant("pplant.png",2,2,FPS,PPLANT,2,false,false,0,1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	m_sprites[(int)SPRID::PIPE3].setOrigin(m_textures[(int)SPRID::PIPE3].getSize().x / 2.f, m_textures[(int)SPRID::PIPE3].getSize().x / 2.f);
	m_sprites[(int)SPRID::PIPE3].setPosition(m_enemy.back()->GetPosition() + sf::Vector2f(0.f, 40.f));

	tmp = Collisions::Get()->GetTile(290, 6);
	m_enemy.push_back(new Rex("rex.png",3,2,FPS,REX,1,false,false,0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(298, 3);
	m_enemy.push_back(new Chuck("chuck.png", 6, 2, FPS, CHUCK, 1, false, false, 3, 0.5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y *sY + 6.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());*/
}

void Level::AddObjects()
{
	/*Tile tmp = Collisions::Get()->GetTile(17, 9);
	m_objects.push_back(new Object("ycoin.png",1,1,FPS,YCOIN,2,true,true,0,1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x,tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(37, 6);
	std::vector<int> frames{ 1, 1 };
	m_objects.push_back(new Object("bopwr.png", 2, 1, FPS, QBOX, 2, true, false, 0, 1.f));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(88, 9);
	m_objects.push_back(new Object("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(102, 3);
	m_objects.push_back(new Object("shroom.png", 1, 1, FPS, SHROOM, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(120, 6);
	m_objects.push_back(new Object("boassort.png", 1, 1, FPS, SMBOX, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(121, 6);
	m_objects.push_back(new Object("boassort.png", 1, 1, FPS, SMBOX, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(151, 5);
	m_objects.push_back(new Object("chpnt.png", 1, 1, FPS, CHKPOINT, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY +10.f));

	tmp = Collisions::Get()->GetTile(179, 11);
	m_objects.push_back(new Object("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(239, 6);
	frames = std::vector<int>{ 1, 4 };
	m_objects.push_back(new Object("bospin.png", 2, 4, FPS, SPBOX, 2, true, false, 0, 0.25f));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(243, 5);
	m_objects.push_back(new Object("bospin.png", 2, 4, FPS, SPBOX, 2, true, false, 0, 0.25f));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(247, 6);
	m_objects.push_back(new Object("bospin.png", 2, 4, FPS, SPBOX, 2, true, false, 0, 0.25f));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(243, 9);
	frames = std::vector<int>{ 1, 1 };
	m_objects.push_back(new Object("bopwr.png", 2, 1, FPS, QBOX, 2, true, false, 0, 1.f));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(288, 9);
	m_objects.push_back(new Object("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(302, 3);
	m_objects.push_back(new Object("goal.png", 1, 1, FPS, GOAL,1, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x-6.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY +30.f));*/
}

void Level::AddObject(Object * obj)
{
	m_objects.push_back(obj);
}

