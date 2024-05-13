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
		m_player.reset(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player.reset(new Player("mario.png", 8, 2, FPS, PLAYER, 1, true, false, 0, .5f));

	m_level = std::make_unique<Level>();
	m_logger = std::make_unique<Logger>();
}

Game::~Game()
{
	if (m_instance)
	{

		m_instance = nullptr;
	}
}

Game * Game::GetGameMgr()
{
	if (m_instance == nullptr)
		m_instance = new Game();

	return m_instance;
}

void Game::ChangePlayer(Player * ply)
{
	m_player.reset(ply);
	Collisions::Get()->ReplacePlayer(m_player.get());
}

Player * Game::GetPlayer()
{
	return m_player.get();
}

Level * Game::GetLevel()
{
	return m_level.get();
}

Logger * Game::GetLogger()
{
	return m_logger.get();
}

void Game::CheckInView()
{
	m_player->SetVisible(Camera::GetCamera()->IsInView(*m_player->GetBBox()->GetSprite()));

	for (Tile* grid : Collisions::Get()->GetGrid())
		grid->SetVisible(Camera::GetCamera()->IsinView(grid->GetRect()));

	for (Enemy* enemy : m_level->GetEnemies())
		enemy->SetVisible(Camera::GetCamera()->IsInView(*enemy->GetBBox()->GetSprite()));

	for (Object* object : m_level->GetObjects())
		object->SetVisible(Camera::GetCamera()->IsInView(*object->GetBBox()->GetSprite()));
}

void Game::Update(float deltaTime)
{
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
		CtrlMgr::GetCtrlMgr()->GetController()->Update();

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

