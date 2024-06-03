#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"

std::unique_ptr<Game> Game::m_instance = nullptr;

Game::Game()
{
	m_instance.reset(this);
	m_texureManager = std::make_unique<TextureManager>();
	m_camera = std::make_unique<Camera>();

	if (Automated)
		m_player.reset(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player = std::make_unique<Player>(8, 2, false, 0, .5f);

	m_world = std::make_unique<World>();
	m_logger = std::make_unique<Logger>();
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
	m_player->SetVisible(m_camera->IsInView(m_player->GetBBox()->GetSprite()));

	for (auto& tile : Collisions::Get()->GetGrid())
		tile->SetVisible(m_camera->IsinView(tile->GetRect()));

	for (auto& enemy : m_world->GetEnemies())
		enemy->SetVisible(m_camera->IsInView(enemy->GetSprite()));

	for (auto& object : m_world->GetObjects())
		object->SetVisible(m_camera->IsInView(object->GetSprite()));
}
