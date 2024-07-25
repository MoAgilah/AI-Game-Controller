#include "World.h"
#include "Game.h"
#include "Camera.h"
#include "Timer.h"
#include "../Game/Constants.h"
#include "../Game/Camera.h"

#include "../Collisions/AABB.h"
#include "../Collisions/CollisionManager.h"
#include "../GameObjects/Collectables.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Koopa.h"
#include "../GameObjects/Bill.h"
#include "../GameObjects/Rex.h"
#include "../GameObjects/PPlant.h"
#include "../GameObjects/Chuck.h"

World::World()
{
	m_sprites[(int)Sprites::BackGround].SetTexture(TexID::Background);
	m_sprites[(int)Sprites::BackGround].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::BackGround].SetOrigin(sf::Vector2f(0, 0));
	m_sprites[(int)Sprites::BackGround].SetPosition(sf::Vector2f(0, -480));

	AddGUI();
}


void World::Update(float deltaTime)
{
	for (const auto& enemy : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		enemy->Update(deltaTime);
	}

	for (const auto& object : m_objects)
	{
		if (!object->GetActive())
			continue;

		object->Update(deltaTime);
	}

	UpdateGUI();
}

void World::Render(sf::RenderWindow& window)
{
	m_sprites[(int)Sprites::BackGround].Render(window);

	for (const auto& enemy : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		enemy->Render(window);
	}

	for (const auto& object : m_objects)
	{
		if (!object->GetActive())
			continue;

		object->Render(window);
	}

	auto camera = Game::GetGameMgr()->GetCamera();
	for (auto i = (int)Sprites::Pipe1; i <= (int)Sprites::Pipe3; i++)
	{
		if (!camera->IsInView(m_sprites[i].GetSprite()))
			continue;

		m_sprites[i].Render(window);
	}

	for (auto i = (int)Sprites::Name; i < (int)Sprites::Max; i++)
		m_sprites[i].Render(window);

	for (const auto& text : m_texts)
		window.draw(text);
}

void World::ResetLevel()
{
	for (const auto& enemy : m_enemies)
		enemy->Reset();

	if (m_objects.size() > 14)
		m_objects.resize(14);

	for (const auto& object : m_objects)
		object->Reset();
}

void World::CheckIsInView()
{
	auto camera = Game::GetGameMgr()->GetCamera();

	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->SetActive(camera->IsInView(m_enemies[i]->GetSprite()->GetSprite()));
	}

	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i]->SetActive(camera->IsInView(m_objects[i]->GetSprite()->GetSprite()));
	}
}

void World::AddObject(const sf::Vector2f& pos)
{
	m_objects.push_back(std::make_unique<Mushroom>(pos));
}

void World::AddEnemies()
{
	Tile tmp;
	/*tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(13, 5);
	m_enemies.push_back(std::make_unique<Koopa>(false, tmp.GetPosition()));*/

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(32, 8);
	m_enemies.push_back(std::make_unique<Bill>(false, tmp.GetPosition() - sf::Vector2f(0, -16)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(35, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(47, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(55, 6);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(72, 6);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(82, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(89, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(112, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(113, 9);
	m_enemies.push_back(std::make_unique<PPlant>(tmp.GetPosition() + sf::Vector2f(18, 16)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(138, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(139, 9);
	m_enemies.push_back(std::make_unique<PPlant>(tmp.GetPosition() + sf::Vector2f(18, 16)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(172, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(182, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(184, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(186, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(189, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(203, 9);
	m_enemies.push_back(std::make_unique<Bill>(false, tmp.GetPosition() + sf::Vector2f(0, 20)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(207, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(227, 9);
	m_enemies.push_back(std::make_unique<Bill>(false, tmp.GetPosition() + sf::Vector2f(0, 20)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(242, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(257, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(263, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(280, 9);
	m_enemies.push_back(std::make_unique<Bill>(false, tmp.GetPosition() + sf::Vector2f(0, 20)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(284, 8);
	m_enemies.push_back(std::make_unique<PPlant>(tmp.GetPosition() + sf::Vector2f(18, 16)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(290, 8);
	m_enemies.push_back(std::make_unique<Rex>(false, tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(298, 10);
	m_enemies.push_back(std::make_unique<Chuck>(false, tmp.GetPosition() + sf::Vector2f(0,24)));
}

void World::AddObjects()
{
	Tile tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(17, 5);
	m_objects.push_back(std::make_unique<YCoin>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(37, 8);
	m_objects.push_back(std::make_unique<QBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(89, 5);
	m_objects.push_back(std::make_unique<YCoin>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(102, 11);
	m_objects.push_back(std::make_unique<Mushroom>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(120, 8);
	m_objects.push_back(std::make_unique<SBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(121, 8);
	m_objects.push_back(std::make_unique<SBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(151, 9);
	m_objects.push_back(std::make_unique<CheckPoint>(tmp.GetPosition() - sf::Vector2f(3, 10.5f)));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(179, 2);
	m_objects.push_back(std::make_unique<YCoin>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(239, 8);
	m_objects.push_back(std::make_unique<SBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(243, 9);
	m_objects.push_back(std::make_unique<SBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(247, 8);
	m_objects.push_back(std::make_unique<SBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(243, 5);
	m_objects.push_back(std::make_unique<QBox>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(288, 4);
	m_objects.push_back(std::make_unique<YCoin>(tmp.GetPosition()));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(302, 11);
	m_objects.push_back(std::make_unique<Goal>(sf::Vector2f(tmp.GetPosition().x - 7.f, tmp.GetPosition().y)));
}

void World::AddForeGroundSprites()
{
	auto tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(113, 10);
	m_sprites[(int)Sprites::Pipe1].SetTexture(TexID::Pipe1);
	m_sprites[(int)Sprites::Pipe1].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::Pipe1].SetPosition(tmp.GetPosition() + sf::Vector2f(18.5f, 0.f));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(140, 9);
	m_sprites[(int)Sprites::Pipe2].SetTexture(TexID::Pipe2);
	m_sprites[(int)Sprites::Pipe2].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::Pipe2].SetPosition(tmp.GetPosition() - sf::Vector2f(18.5f, 0.f));

	tmp = Game::GetGameMgr()->GetCollisionMgr()->GetTile(284, 9);
	m_sprites[(int)Sprites::Pipe3].SetTexture(TexID::Pipe3);
	m_sprites[(int)Sprites::Pipe3].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::Pipe3].SetPosition(tmp.GetPosition() + sf::Vector2f(18.5f, 0.f));
}

void World::AddGUI()
{
	m_font.loadFromFile("Resources/Fonts/arial.ttf");

	m_sprites[(int)Sprites::Name].SetTexture(TexID::Name);
	m_sprites[(int)Sprites::Time].SetTexture(TexID::Time);

	for (int i = 0; i < (int)Texts::Max; i++)
	{
		m_texts[i].setFont(m_font);
		m_texts[i].setCharacterSize(15);
		m_texts[i].setOutlineColor(sf::Color::Black);
		m_texts[i].setOutlineThickness(1.f);
		m_texts[i].setFillColor(sf::Color::Yellow);
	}

	m_texts[(int)Texts::Name].setString("x 00");
	m_texts[(int)Texts::Time].setString(std::to_string((int)Timer::Get()->CurrentTime()));
}

void World::UpdateGUI()
{
	auto curScrBounds = Game::GetGameMgr()->GetCamera()->GetCurrentScreenBounds();
	auto view = Game::GetGameMgr()->GetCamera()->GetView();

	m_sprites[(int)Sprites::Name].SetPosition(sf::Vector2f(curScrBounds.left + 30, 20));

	m_texts[(int)Texts::Name].setPosition(m_sprites[(int)Sprites::Name].GetPosition() + sf::Vector2f((float)m_sprites[(int)Sprites::Name].GetTextureSize().x * 0.5f + 20, -10));

	m_sprites[(int)Sprites::Time].SetPosition(sf::Vector2f(view.getCenter().x, 20));

	m_texts[(int)Texts::Time].setPosition(m_sprites[(int)Sprites::Time].GetPosition() + sf::Vector2f((float)m_sprites[(int)Sprites::Time].GetTextureSize().x * 0.5f + 20, -10));

	m_texts[(int)Texts::Time].setString(std::to_string((int)Timer::Get()->CurrentTime()));
}
