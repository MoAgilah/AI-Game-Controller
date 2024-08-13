#include "../Game/GameManager.h"
#include "../Game/Camera.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/CollisionManager.h"
#include "../Collisions/Grid.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"
#include "../GameStates/MainState.h"
#include "../GameStates/DebugState.h"

std::shared_ptr<GameManager> GameManager::m_instance = nullptr;

GameManager::GameManager()
{
	m_instance.reset(this);
	m_texureManager = std::make_unique<TextureManager>();
	m_collisionManager = std::make_unique<CollisionManager>();
	m_camera = std::make_unique<Camera>();

	if (Automated)
		m_player.reset(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player = std::make_unique<Player>(m_collisionManager->GetTile(1, 11).GetPosition());

	m_world = std::make_unique<World>();
	m_logger = std::make_unique<Logger>();

	m_stateMgr.ChangeState(new DebugState(this));
}

void GameManager::ChangePlayer(Player * ply)
{
	m_player.reset(ply);
	m_collisionManager->ReplacePlayer(m_player.get());
}

void GameManager::Update(float deltaTime)
{
	m_stateMgr.Update(deltaTime);
}

void GameManager::Render(sf::RenderWindow & window)
{
	m_stateMgr.Render(window);
}

void GameManager::CheckInView()
{
	m_player->SetActive(m_camera->IsInView(m_player->GetAABB()));

	for (auto& tile : m_collisionManager->GetGrid())
		tile->SetActive(m_camera->IsInView(tile->GetAABB()));

	m_world->CheckIsInView();
}
