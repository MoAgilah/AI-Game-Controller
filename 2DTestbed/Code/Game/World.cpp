#include "World.h"
#include "Game.h"
#include "Camera.h"
#include "Timer.h"
#include "../Game/Constants.h"
#include "../Game/Camera.h"

#include "../Collisions/BoundingBox.h"
#include "../Collisions/Collisions.h"
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

	AddEnemies();
	AddObjects();
	AddForeGroundSprites();
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
	m_objects.push_back(std::make_unique<Object>(TexID::Shroom, 1, 1, (int)TexID::ShroomBB, true, true, 0, 1.f, pos));
}

void World::AddEnemies()
{
	Tile tmp = Collisions::Get()->GetTile(32, 9);
	m_enemies.push_back(std::make_unique<Bill>(false, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	/*tmp = Collisions::Get()->GetTile(33, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(47, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(55, 6);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(72, 7);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(82, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(89, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(112, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(113, 9);
	m_enemies.push_back(std::make_unique<PPlant>(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(138, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(139, 8);
	m_enemies.push_back(std::make_unique<PPlant>(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(172, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(182, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(184, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(186, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(189, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(203, 10);
	m_enemies.push_back(std::make_unique<Bill>(true, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(207, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(227, 10);
	m_enemies.push_back(std::make_unique<Bill>(true, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(242, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(257, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(263, 11);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(280, 10);
	m_enemies.push_back(std::make_unique<Bill>(true, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(284, 8);
	m_enemies.push_back(std::make_unique<PPlant>(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(290, 8);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(298, 11);
	m_enemies.push_back(std::make_unique<Chuck>(false, 3, 0.5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 6.f)));*/
}

void World::AddObjects()
{
	/*Tile tmp = Collisions::Get()->GetTile(17, 5);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	tmp = Collisions::Get()->GetTile(37, 8);
	m_objects.push_back(std::make_unique<Object>(TexID::QBox, 2, 1, (int)TexID::BoxBB, true, false, 0, 1.f, tmp.GetPosition()));
	std::vector<int> frames{ 1, 1 };
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(88, 5);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	tmp = Collisions::Get()->GetTile(102, 11);
	m_objects.push_back(std::make_unique<Object>(TexID::Shroom, 1, 1, (int)TexID::ShroomBB, true, true, 0, 1.f, tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(120, 8);
	m_objects.push_back(std::make_unique<Object>(TexID::Box, 1, 1, (int)TexID::BoxBB, true, true, 0, 1.f, tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(121, 8);
	m_objects.push_back(std::make_unique<Object>(TexID::Box, 1, 1, (int)TexID::BoxBB, true, true, 0, 1.f, tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(151, 9);
	m_objects.push_back(std::make_unique<Object>(TexID::ChkPnt, 1, 1, (int)TexID::ChkPntBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 10.f)));

	tmp = Collisions::Get()->GetTile(179, 3);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	frames = { 1, 4 };

	tmp = Collisions::Get()->GetTile(239, 8);
	m_objects.push_back(std::make_unique<Object>(TexID::SBox, 2, 4, (int)TexID::BoxBB, true, false, 0, 0.25f, tmp.GetPosition()));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(243, 9);
	m_objects.push_back(std::make_unique<Object>(TexID::SBox, 2, 4, (int)TexID::BoxBB, true, false, 0, 0.25f, tmp.GetPosition()));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(247, 8);
	m_objects.push_back(std::make_unique<Object>(TexID::SBox, 2, 4, (int)TexID::BoxBB, true, false, 0, 0.25f, tmp.GetPosition()));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(243, 5);
	m_objects.push_back(std::make_unique<Object>(TexID::QBox, 2, 1, (int)TexID::BoxBB, true, false, 0, 1.f, tmp.GetPosition()));
	frames = { 1, 1 };
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(288, 5);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	tmp = Collisions::Get()->GetTile(302, 11);
	m_objects.push_back(std::make_unique<Object>(TexID::Goal, 1, 1, (int)TexID::GoalBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x - 6.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 30.f)));*/
}

void World::AddForeGroundSprites()
{
	auto tmp = Collisions::Get()->GetTile(113, 9);
	m_sprites[(int)Sprites::Pipe1].SetTexture(TexID::Pipe1);
	m_sprites[(int)Sprites::Pipe1].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::Pipe1].SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY) + sf::Vector2f(-37.f, 0.f));

	tmp = Collisions::Get()->GetTile(139, 8);
	m_sprites[(int)Sprites::Pipe2].SetTexture(TexID::Pipe2);
	m_sprites[(int)Sprites::Pipe2].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::Pipe2].SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY) + sf::Vector2f(-37.f, 0.f));

	tmp = Collisions::Get()->GetTile(284, 8);
	m_sprites[(int)Sprites::Pipe3].SetTexture(TexID::Pipe3);
	m_sprites[(int)Sprites::Pipe3].SetScale(sf::Vector2f(sX, sY));
	m_sprites[(int)Sprites::Pipe3].SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY) + sf::Vector2f(-37.f, 0.f));
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

	m_sprites[(int)Sprites::Name].SetPosition(sf::Vector2f(curScrBounds.left + 20, 20));

	m_texts[(int)Texts::Name].setPosition(m_sprites[(int)Sprites::Name].GetPosition() + sf::Vector2f((float)m_sprites[(int)Sprites::Name].GetTextureSize().x * 0.5f + 25, 0));

	m_sprites[(int)Sprites::Time].SetPosition(sf::Vector2f(view.getCenter().x + 20, 20));

	m_texts[(int)Texts::Time].setPosition(m_sprites[(int)Sprites::Time].GetPosition() + sf::Vector2f((float)m_sprites[(int)Sprites::Time].GetTextureSize().x * 0.5f + 25, 0));

	m_texts[(int)Texts::Time].setString(std::to_string((int)Timer::Get()->CurrentTime()));
}
