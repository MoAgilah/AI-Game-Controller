#include "World.h"
#include "Game.h"
#include "Camera.h"
#include "Timer.h"
#include "../Game/Constants.h"


World::World()
{
	m_backgroundSprite.Init("background.png");
	m_backgroundSprite.SetScale(sf::Vector2f(sX, sY));
	m_backgroundSprite.SetOrigin(sf::Vector2f(0, 0));
	m_backgroundSprite.SetPosition(sf::Vector2f(0, -480));
	AddGUI();
}


void World::Update(float deltaTime)
{
	UpdateGUI();
}

void World::Render(sf::RenderWindow& window)
{
	m_backgroundSprite.Render(window);

	// render GUI
	for (auto& sprite : m_sprites)
		window.draw(*sprite.GetSprite());

	for (const auto& text : m_text)
		window.draw(text);
}

void World::ResetLevel()
{
}

void World::AddEnemies()
{
}

void World::AddObject()
{
}

void World::AddGUI()
{
	m_font.loadFromFile("Resources/Fonts/arial.ttf");

	std::vector<std::string> filenames{ "name.png", "time.png" };

	for (int i = 0; i < (int)GUI::MAX; i++)
	{
		m_sprites[i].Init(filenames[i]);
		m_text[i].setFont(m_font);
		m_text[i].setCharacterSize(15);
		m_text[i].setOutlineColor(sf::Color::Black);
		m_text[i].setOutlineThickness(1.f);
		m_text[i].setFillColor(sf::Color::Yellow);
	}

	m_text[(int)GUI::NAME].setString("x 00");
	m_text[(int)GUI::TIME].setString(std::to_string((int)Timer::Get()->CurrentTime()));
}

void World::UpdateGUI()
{
	auto curScrBounds = Game::GetGameMgr()->GetCamera()->GetCurrentScreenBounds();
	auto view = Game::GetGameMgr()->GetCamera()->GetView();

	m_sprites[(int)GUI::NAME].SetPosition(sf::Vector2f(curScrBounds.left + m_sprites[(int)GUI::NAME].GetOrigin().x + 20,
		curScrBounds.top + m_sprites[(int)GUI::NAME].GetOrigin().y + 20));

	m_sprites[(int)GUI::TIME].SetPosition(sf::Vector2f(view.getCenter().x,
		curScrBounds.top + m_sprites[(int)GUI::TIME].GetOrigin().y + 20));

	for (int i = 0; i < (int)GUI::MAX; i++)
		m_text[i].setPosition(m_sprites[i].GetPosition() + sf::Vector2f(m_sprites[i].GetOrigin().x + 10, -9));

	m_text[(int)GUI::TIME].setString(std::to_string((int)Timer::Get()->CurrentTime()));
}
