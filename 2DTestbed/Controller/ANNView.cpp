#include "../Controller/ANNView.h"
#include "../Game/Camera.h"
#include "../Game/Game.h"
#include "../GameObjects/Player.h"
#include "../Collisions/Collisions.h"
#include "../Controller/CtrlMgr.h"

#include <format>

ANNView::ANNView()
{
	m_font.loadFromFile("Fonts/arial.ttf");

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
	back.setSize(sf::Vector2f(600.f, 600.f));
	back.setOrigin(scrX * 0.5f, scrY * 0.5f);
	back.setScale(sX, sY);
	//black half transparency
	back.setFillColor(sf::Color(0, 0, 0, 125));


	//create the grid inputs
	int cnt = 0;
	for (int i = 0; i < 240; i++)
	{
		m_vecView.push_back(new Tile());
	}
}

void ANNView::Update()
{
	const Camera* camera = Game::GetGameMgr()->GetCamera();

	//get main view
	sf::View standard = camera->GetView();

	//reset mini view with standard view
	m_view.reset(sf::FloatRect(standard.getCenter().x, standard.getCenter().y, static_cast<float>(size), static_cast<float>(scrY * size / scrX)));
	m_view.setViewport(sf::FloatRect(1.f - static_cast<float>(m_view.getSize().x) / scrX - 0.6f, 1.f - static_cast<float>(m_view.getSize().y) / scrY - 0.6f, static_cast<float>(m_view.getSize().x) / scrX, static_cast<float>(m_view.getSize().y) / scrY));
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
	int num(0),cnt(0);
	std::vector<Tile*> grid = Collisions::Get()->GetGrid();
	for (int i = 0; i < grid.size(); i++)
	{
		if (cnt == 240) break;

		if (grid[i]->GetActive())
		{
			if (cnt == 0) num = grid[i]->GetCowNum();

			if (grid[i]->GetCowNum() <= num + 15)
			{
				m_vecView[cnt] = grid[i];
				cnt++;
			}
		}
	}

	//colour m_visible tiles
	sf::RectangleShape tmp;
	std::vector<GameObject*> gobj = Collisions::Get()->GetCollidables();
	for (int i = 0; i < m_vecView.size(); i++)
	{
		if (m_vecView[i]->GetType() == EMPTY)
		{
			sf::RectangleShape tmp;
			for (size_t j = 0; j < gobj.size(); j++)
			{
				int type = gobj[j]->GetBBox()->GetID();
				if (type == BILL)
				{
					tmp.setSize(sf::Vector2f(96, 108));
					tmp.setOrigin(48, 54);
				}
				else
				{
					tmp.setSize(sf::Vector2f(16, 16));
					tmp.setOrigin(8, 8);
				}

				if (gobj[j]->GetActive())
				{
					tmp.setPosition(gobj[j]->GetPosition());

					if (m_vecView[i]->GetRect().getGlobalBounds().intersects(tmp.getGlobalBounds()))
					{
						if (type == PLAYER)
						{
							m_vecView[i]->SetFillColour(sf::Color::Red);
						}
						else if (type > PLAYER && type <= EnmyEnd)
						{
							m_vecView[i]->SetFillColour(sf::Color::Black);
						}
						else if (type > EnmyEnd && type <= SHROOM)
						{
							m_vecView[i]->SetFillColour(sf::Color::Yellow);
						}
						else if (type > SHROOM && type <= ObjEnd)
						{
							m_vecView[i]->SetFillColour(sf::Color::Green);
						}

						break;
					}
					else
					{
						m_vecView[i]->SetFillColour(sf::Color::Transparent);
					}
				}
				else
				{
					m_vecView[i]->SetFillColour(sf::Color::Transparent);
				}
			}
		}
		else
		{
			m_vecView[i]->SetFillColour(sf::Color::White);
		}
	}
}

void ANNView::Render(sf::RenderWindow & window)
{
	window.setView(m_view);
	window.draw(back);

	for (int i = 0; i < m_vecView.size(); i++)
	{
		window.draw(m_vecView[i]->GetRect());
	}

	for (int i = 0; i < m_text.size(); i++)
	{
		window.draw(*m_text[i]);
	}
}

std::vector<Tile*> ANNView::GetVecView()
{
	return m_vecView;
}

ANNView::~ANNView()
{
}
