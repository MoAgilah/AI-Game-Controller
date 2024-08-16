#include "../Controller/ANNView.h"
#include "../Game/Camera.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"
#include "../Collisions/CollisionManager.h"
#include "../Controller/CtrlMgr.h"
#include "../Game/Constants.h"
#include "../Utilities/Utilities.h"

#include <format>

ANNView::ANNView()
{
	m_font.loadFromFile("Resources/Fonts/arial.ttf");

	m_text.push_back(new sf::Text());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize(60);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);
	m_text.back()->setString("Player: 0");

	m_text.push_back(new sf::Text());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize(60);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);
	m_text.back()->setString("Generation: 0");

	m_text.push_back(new sf::Text());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize(60);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);
	m_text.back()->setString("Highest fitness: 0");

	//background of second screen
	back.setSize(screenDim);
	back.setOrigin(screenDim * 0.5f);
	back.setScale(scale);
	//black half transparency
	back.setFillColor(sf::Color(0, 0, 0, 125));


	//create the grid inputs
	for (int i = 0; i < 255; i++)
		m_vecView.push_back(new Tile());
}

void ANNView::Update()
{
	const Camera* camera = GameManager::GetGameMgr()->GetCamera();

	//get main view
	sf::View standard = camera->GetView();

	//reset mini view with standard view
	m_view.reset(sf::FloatRect(standard.getCenter().x-100, standard.getCenter().y, static_cast<float>(size), static_cast<float>(screenDim.y * size / screenDim.x)));
	m_view.setViewport(sf::FloatRect(1.f - static_cast<float>(m_view.getSize().x) / screenDim.x - 0.6f, 1.f - static_cast<float>(m_view.getSize().y) / screenDim.y - 0.6f, static_cast<float>(m_view.getSize().x) / screenDim.x, static_cast<float>(m_view.getSize().y) / screenDim.y));
	m_view.zoom(4.f);

	//update bkg image with standard view center position
	back.setPosition(camera->GetView().getCenter());

	Controller* ctrl = CtrlMgr::GetCtrlMgr()->GetController();

	m_text[0]->setString(std::format("Player: {} / {}", ctrl->GetCurrentPlayerNum(), CParams::iNumPlayers));
	m_text[0]->setPosition(m_view.getCenter() - sf::Vector2f(400, -200));

	m_text[1]->setString(std::format("Generation: {}" ,ctrl->GetCurrentGeneration()));
	m_text[1]->setPosition(m_view.getCenter() - sf::Vector2f(400, -260));

	m_text[2]->setString(std::format("Highest fitness: {}", ctrl->BestFitness()));
	m_text[2]->setPosition(m_view.getCenter() - sf::Vector2f(400, -330));

	//extract m_visible tiles
	int cnt = 0;
	for (auto& tile : GameManager::GetGameMgr()->GetCollisionMgr()->GetGrid())
	{
		if (cnt == 255) break;

		if (!tile->GetActive())
			continue;

		m_vecView[cnt] = tile.get();
		m_vecView[cnt]->SetFillColour(sf::Color::Transparent);

		cnt++;
	}

	//colour m_visible tiles
	for (auto& tile : m_vecView)
	{
		if (tile->GetType() != EMPTY)
		{
			tile->SetFillColour(sf::Color::White);
			continue;
		}

		sf::RectangleShape tmp;
		for (auto& gobj : GameManager::GetGameMgr()->GetCollisionMgr()->GetCollidables())
		{
			TexID type = gobj->GetID();
			if (type == TexID::Bill)
			{
				tmp.setSize(sf::Vector2f(96, 108));
				tmp.setOrigin(48, 54);
			}
			else
			{
				tmp.setSize(sf::Vector2f(16, 16));
				tmp.setOrigin(8, 8);
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
	window.draw(back);

	for (auto& tile : m_vecView)
		window.draw(tile->GetRect());

	for (auto& text : m_text)
		window.draw(*text);
}

std::vector<Tile*> ANNView::GetVecView()
{
	return m_vecView;
}
