#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include "../GameObjects/Player.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../Collisions/Grid.h"
#include "../Controller/ControllerManager.h"
#include "../GameStates/MainState.h"
#include "../GameStates/DebugState.h"

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager()
	: m_timer(300.f)
{
	m_instance = this;
	m_collisionManager = std::make_unique<CollisionManager>();


	if (Automated)
		m_player.reset(ControllerManager::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player = std::make_unique<Player>(m_collisionManager->GetTile(2, 11)->GetPosition());

	m_world = std::make_unique<World>();

	m_stateManager.ChangeState(new DebugState(this));
}

GameManager::~GameManager()
{
	if (m_instance)
		m_instance = nullptr;
}

void GameManager::ChangePlayer(Player * ply)
{
	m_player.reset(ply);
	m_collisionManager->ReplacePlayer(m_player.get());
}

void GameManager::Update(float deltaTime)
{
	m_stateManager.Update(deltaTime);
}

void GameManager::Render(sf::RenderWindow & window)
{
	m_stateManager.Render(window);
}

void GameManager::CheckInView()
{
	m_player->SetActive(m_camera.IsInView(m_player->GetAABB()));

	for (auto& tile : m_collisionManager->GetGrid())
		tile->SetActive(m_camera.IsInView(tile->GetAABB()));

	m_world->CheckIsInView();
}
