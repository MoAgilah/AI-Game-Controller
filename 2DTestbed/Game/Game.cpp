#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"

std::unique_ptr<Game> Game::m_instance = nullptr;

Game::Game()
{
	m_camera = std::make_unique<Camera>();

	if (Automated)
		m_player.reset(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player = std::make_unique<Player>("mario.png", 8, 2, false, 0, .5f);

	m_world = std::make_unique<World>();
	m_logger = std::make_unique<Logger>();
	m_instance.reset(this);
}

Game::~Game()
{
	if (m_instance)
		m_instance = nullptr;
}

void Game::ChangePlayer(Player * ply)
{
	m_player.reset(ply);
	Collisions::Get()->ReplacePlayer(m_player.get());
}

void Game::Update(float deltaTime)
{
	Timer::Get()->UpdateTime(deltaTime);

	if (Automated)
		CtrlMgr::GetCtrlMgr()->GetController()->Update();

	m_player->Update(deltaTime);
	m_world->Update(deltaTime);
}

void Game::Render(sf::RenderWindow & window)
{
	m_camera->Reset(window);
	CheckInView();

	m_world->Render(window);
	m_player->Render(window);

	Collisions::Get()->Render(window);
}

void Game::CheckInView()
{
	m_player->SetActive(m_camera->IsInView(m_player->GetBBox()->GetSprite()));

	for (auto& tile : Collisions::Get()->GetGrid())
		tile->SetActive(m_camera->IsinView(tile->GetRect()));

	/*for (auto& enemy : m_world->GetEnemies())
		enemy->SetActive(m_camera->IsInView(enemy->GetBBox()->GetSprite()));

	for (auto& object : m_level->GetObjects())
		object->SetActive(m_camera->IsInView(object->GetBBox()->GetSprite()));*/
}
