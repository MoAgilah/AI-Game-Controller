#include "../Game/Game.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/Collisions.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"
#include "../GameStates/MainState.h"
#include "../GameStates/DebugState.h"

std::shared_ptr<Game> Game::m_instance = nullptr;

Game::Game()
{
	m_instance.reset(this);
	m_texureManager = std::make_unique<TextureManager>();
	m_camera = std::make_unique<Camera>();

	if (Automated)
		m_player.reset(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player = std::make_unique<Player>(Collisions::Get()->GetTile(295, 11).GetPosition());

	m_world = std::make_unique<World>();
	m_logger = std::make_unique<Logger>();

	m_stateMgr.ChangeState(new DebugState(this));
}

void Game::ChangePlayer(Player * ply)
{
	m_player.reset(ply);
	Collisions::Get()->ReplacePlayer(m_player.get());
}

void Game::Update(float deltaTime)
{
	m_stateMgr.Update(deltaTime);
}

void Game::Render(sf::RenderWindow & window)
{
	m_stateMgr.Render(window);
}

void Game::CheckInView()
{
	m_player->SetActive(m_camera->IsInView(m_player->GetBBox()->GetSprite()));

	for (auto& tile : Collisions::Get()->GetGrid())
		tile->SetActive(m_camera->IsinView(tile->GetRect()));

	m_world->CheckIsInView();
}
