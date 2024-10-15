#include "../Controller/ANNView.h"
#include <format>
#include "../Controller/AIController.h"
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"
#include "../Utilities/Utilities.h"

ANNView::ANNView()
{
	for (int i = 0; i < 255; i++)
		m_vecView[i] = std::make_shared<Tile>();

	auto& font = GameManager::Get()->GetFontMgr().GetStandardFont();

	for (size_t i = 0; i < 3; i++)
	{
		m_text[i] = std::make_unique<sf::Text>();
		m_text[i]->setFont(font);
		m_text[i]->setCharacterSize(60);
		m_text[i]->setOutlineColor(sf::Color::Black);
		m_text[i]->setOutlineThickness(1.f);
		m_text[i]->setFillColor(sf::Color::Yellow);
	}

	m_text[0]->setString("Player: 0");
	m_text[1]->setString("Generation: 0");
	m_text[2]->setString("Highest fitness: 0");

	m_background.setSize(GameConstants::ScreenDim);
	m_background.setOrigin(GameConstants::ScreenDim * 0.5f);
	m_background.setScale(GameConstants::Scale);
	m_background.setFillColor(sf::Color(0, 0, 0, 125));
}

void ANNView::Update()
{
	auto& camera = GameManager::Get()->GetCamera();
	sf::View standard = camera.GetView();

	//reset mini view with standard view
	m_view.reset(sf::FloatRect(standard.getCenter().x - 100, standard.getCenter().y, size, size));
	m_view.setViewport(sf::FloatRect(0.025f, 0.07f, 0.33f, 0.33f));
	m_view.zoom(4.f);

	//update bkg image with standard view center position
	m_background.setPosition(camera.GetView().getCenter());

	AIController* ctrl = GameManager::Get()->GetAIController();

	m_text[0]->setString(std::format("Player: {} / {}", ctrl->GetCurrentPlayerNum(), CParams::iNumPlayers));
	m_text[0]->setPosition(m_view.getCenter() - sf::Vector2f(400, -200));

	m_text[1]->setString(std::format("Generation: {}", ctrl->GetCurrentGeneration()));
	m_text[1]->setPosition(m_view.getCenter() - sf::Vector2f(400, -260));

	m_text[2]->setString(std::format("Highest fitness: {}", ctrl->BestFitness()));
	m_text[2]->setPosition(m_view.getCenter() - sf::Vector2f(400, -320));

	//extract m_visible tiles
	int cnt = 0;
	for (auto& tile : GameManager::Get()->GetCollisionMgr()->GetGrid())
	{
		if (cnt == 255) break;

		if (!tile->GetActive())
			continue;

		m_vecView[cnt] = tile;
		m_vecView[cnt]->SetFillColour(sf::Color::Transparent);

		cnt++;
	}

	// colour m_visible tiles
	for (auto& tile : m_vecView)
	{
		if (tile->GetType() != EMPTY)
		{
			tile->SetFillColour(sf::Color::White);
			continue;
		}

		for (auto& gobj : GameManager::Get()->GetCollisionMgr()->GetCollidables())
		{
			sf::RectangleShape tmp(sf::Vector2f(16, 16));
			TexID type = gobj->GetID();
				if (type == TexID::Bill)
			{
				tmp.setSize(sf::Vector2f(96, 108));
				tmp.setOrigin(48, 54);
			}

			if (gobj->GetActive())
			{
				tmp.setPosition(gobj->GetPosition());

				if (tile->GetRect().getGlobalBounds().intersects(tmp.getGlobalBounds()))
				{
					if (IsPlayerObject(type))
					{
						tile->SetFillColour(sf::Color::Red);
					}
					else if (IsEnemyObject(type))
					{
						tile->SetFillColour(sf::Color::Black);
					}
					else if (IsCollectableObject(type))
					{
						tile->SetFillColour(sf::Color::Yellow);
					}
					else if (IsBoxObject(type))
					{
						tile->SetFillColour(sf::Color::Green);
					}
				}
			}
		}
	}
}

void ANNView::Render(sf::RenderWindow & window)
{
	window.setView(m_view);
	window.draw(m_background);

	for (auto& tile : m_vecView)
		window.draw(tile->GetRect());

	for (auto& text : m_text)
		window.draw(*text);
}