#include "TitleState.h"
#include "../Game/Constants.h"

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

	m_titleMessage.Init("Press Any Key To Start", 30, sf::Vector2f(120, 120 * GameConstants::Scale.y));
}

void TitleState::Pause()
{
}

void TitleState::Resume()
{
}

void TitleState::ProcessInputs()
{
}

void TitleState::Update(float deltaTime)
{
	m_titleMessage.Update(deltaTime);
}

void TitleState::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	m_titleMessage.Render(window);
}
