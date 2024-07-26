#include "../Game/Game.h"
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

std::shared_ptr<Game> Game::m_instance = nullptr;

Game::Game()
{
	m_collectableIds =
	{
		TexID::Shroom, TexID::Goal, TexID::Coin, TexID::YCoin, TexID::ChkPnt
	};

	m_enemyObjIds =
	{
		TexID::Koopa, TexID::Bill, TexID::Rex, TexID::PPlant, TexID::Chuck
	};

	m_dynamicObjIds =
	{
		TexID::Mario, TexID::Super, TexID::Koopa, TexID::Rex, TexID::Chuck, TexID::Shroom, TexID::Goal
	};

	m_instance.reset(this);
	m_texureManager = std::make_unique<TextureManager>();
	m_collisionManager = std::make_unique<CollisionManager>();
	m_camera = std::make_unique<Camera>();

	if (Automated)
		m_player.reset(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentPlayer());
	else
		m_player = std::make_unique<Player>(m_collisionManager->GetTile(300, 11).GetPosition());
	//302, 11
	m_world = std::make_unique<World>();
	m_logger = std::make_unique<Logger>();

	m_stateMgr.ChangeState(new DebugState(this));
}

void Game::ChangePlayer(Player * ply)
{
	m_player.reset(ply);
	m_collisionManager->ReplacePlayer(m_player.get());
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
	m_player->SetActive(m_camera->IsInView(m_player->GetAnimSpr()->GetSprite()));

	for (auto& tile : m_collisionManager->GetGrid())
		tile->SetActive(m_camera->IsinView(tile->GetRect()));

	m_world->CheckIsInView();
}

bool Game::IsBoxObject(TexID id)
{
	return id == TexID::QBox || id == TexID::SBox;
}

bool Game::IsEnemyObject(TexID id)
{
	return std::find(m_enemyObjIds.begin(), m_enemyObjIds.end(), id) != m_enemyObjIds.end();
}

bool Game::IsPlayerObject(TexID id)
{
	return id == TexID::Mario || id == TexID::Super;
}

bool Game::IsCollectableObject(TexID id)
{
	return std::find(m_collectableIds.begin(), m_collectableIds.end(), id) != m_collectableIds.end();
}

bool Game::IsDynamicObject(TexID id, int first, int last)
{
	auto start = m_dynamicObjIds.begin() + first;
	auto end = m_dynamicObjIds.begin() + last;

	return std::find(start, end, id) != end;
}
