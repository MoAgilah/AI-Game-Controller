#include "LoadingState.h"
#include <thread>
#include "../Game/Constants.h"
#include "../GameStates/MenuState.h"

void LoadResources()
{
	auto gameMgr = GameManager::Get();
	gameMgr->InitPlayer();

	auto world = gameMgr->GetWorld();

	world->AddEnemies();
	world->AddObjects();
	world->AddForeGroundSprites();

	GameConstants::GameIsReady = true;
}

LoadingState::LoadingState(GameManager* gameMgr)
	: GameState("Loading")
{
	m_gameMgr = gameMgr;
}

void LoadingState::Initialise()
{
	m_backgroundSpr.SetTexture(TexID::Title);
	m_backgroundSpr.SetScale(sf::Vector2f(GameConstants::Scale));
	m_backgroundSpr.SetOrigin(sf::Vector2f(0, 0));

	m_titleMessage.Init("Loading", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f));

	std::thread t(LoadResources);
	t.detach();
}

void LoadingState::Pause()
{
}

void LoadingState::Resume()
{
}

void LoadingState::ProcessInputs()
{
}

void LoadingState::Update(float deltaTime)
{
	ProcessInputs();

	m_titleMessage.Update(deltaTime);

	if (GameConstants::GameIsReady)
	{
		m_gameMgr->GetGameStateMgr()->PopState();
	}
}

void LoadingState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_titleMessage.Render(window);
}
