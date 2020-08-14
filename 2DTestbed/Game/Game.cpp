#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"

Game* Game::m_instance = nullptr;

Game::Game()
{
	if (Automated)
	{
		m_player = CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer();
	}
	else
	{
		m_player = new Player("mario.png", 8, 2, FPS, PLAYER, 1, true, false, 0, .5f);
	}
	
	m_level = new Level();
	m_logger = new Logger();
}

Game::~Game()
{
	if (m_instance)
	{
		if (m_level)
		{
			delete m_level;
			m_level = nullptr;
		}

		if (m_player)
		{
			delete m_player;
			m_player = nullptr;
		}

		m_instance = nullptr;
	}
}

Game * Game::GetGameMgr()
{
	if (m_instance == nullptr)
	{
		m_instance = new Game();
	}

	return m_instance;
}

void Game::ChangePlayer(Player * ply)
{
	m_player = ply;
	Collisions::Get()->ReplacePlayer(m_player);
}

Player * Game::GetPlayer()
{
	return m_player;
}

Level * Game::GetLevel()
{
	return m_level;
}

Logger * Game::GetLogger()
{
	return m_logger;
}

void Game::CheckInView()
{
	m_player->SetVisible(
		Camera::GetCamera()->IsInView(*m_player->GetBBox()->GetSprite())
	);
	
	std::vector<Tile*> grid = Collisions::Get()->GetGrid();
	for (int i = 0; i < grid.size(); i++)
	{
		grid[i]->SetVisible(
			Camera::GetCamera()->IsinView(grid[i]->GetRect())
		);
	}

	std::vector<Enemy*> enemy = m_level->GetEnemies();
	for (int i = 0; i < enemy.size(); i++)
	{
		enemy[i]->SetVisible(
			Camera::GetCamera()->IsInView(*enemy[i]->GetBBox()->GetSprite())
		);
	}

	std::vector<Object*> object = m_level->GetObjects();
	for (int i = 0; i < object.size(); i++)
	{
		object[i]->SetVisible(
			Camera::GetCamera()->IsInView(*object[i]->GetBBox()->GetSprite())
		);
	}
}

void Game::Update(float deltaTime)
{
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
	{
		CtrlMgr::GetCtrlMgr()->GetController()->Update();
	}

	m_player->Update(deltaTime);
	m_level->Update(deltaTime);

	Camera::GetCamera()->Update(deltaTime);
}

void Game::Render(sf::RenderWindow & window)
{
	CheckInView();
	//do render
	m_level->Render(window);
	Camera::GetCamera()->RenderGui(window);
	m_player->Render(window);

	Collisions::Get()->Render(window);
}

