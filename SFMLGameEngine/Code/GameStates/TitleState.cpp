#include "TitleState.h"
#include "../Game/Constants.h"
#include "../GameStates/MainMenuState.h"

TitleState::TitleState(GameManager* gameMgr)
	: GameState("Title")
{
	m_gameMgr = gameMgr;
}

void TitleState::Initialise()
{
	m_backgroundSpr.SetTexture(TexID::Title);
	m_backgroundSpr.SetScale(sf::Vector2f(GameConstants::Scale));
	m_backgroundSpr.SetOrigin(sf::Vector2f(0, 0));

	m_titleMessage.Init("Press Any Key To Start", 30, sf::Vector2f(GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 2.0f));
}

void TitleState::Pause()
{
}

void TitleState::Resume()
{
}

void TitleState::ProcessInputs()
{
	if (m_gameMgr->GetInputManager().IsAnyKeyPressed())
	{
		m_gameMgr->GetGameStateMgr()->ChangeState(new MainMenuState(m_gameMgr));
	}
}

void TitleState::Update(float deltaTime)
{
	ProcessInputs();

	m_titleMessage.Update(deltaTime);
}

void TitleState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_titleMessage.Render(window);
}
