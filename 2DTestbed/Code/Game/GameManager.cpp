#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include "../GameStates/MainState.h"
#include "../GameStates/DebugState.h"

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager()
	: m_timer(300.f)
{
	m_instance = this;
	m_collisionManager = std::make_unique<CollisionManager>();
	m_aiController = std::make_unique<AIController>();

	if (GameConstants::Automated)
		m_player = m_aiController->GetCurrentPlayer();
	else
		m_player =  new Player(m_collisionManager->GetTile(2, 11)->GetPosition());

	m_world = std::make_unique<World>();

	m_stateManager.ChangeState(new DebugState(this));
}

GameManager::~GameManager()
{
	if (m_instance)
		m_instance = nullptr;

	if (!GameConstants::Automated)
	{
		if (m_player)
			delete m_player;
		m_player = nullptr;
	}
}

void GameManager::CheckInView()
{
	m_player->SetActive(m_camera.IsInView(m_player->GetAABB()));

	for (auto& tile : m_collisionManager->GetGrid())
		tile->SetActive(m_camera.IsInView(tile->GetAABB()));

	m_world->CheckIsInView();
}

void GameManager::Update(float deltaTime)
{
	m_stateManager.Update(deltaTime);
}

void GameManager::Render(sf::RenderWindow& window)
{
	m_stateManager.Render(window);
}

void GameManager::ChangePlayer(Player * ply)
{
	m_player = ply;
	m_collisionManager->ReplacePlayer(m_player);
}

void GameManager::ChangeWorld(World* world)
{
	m_world.reset(world);
}