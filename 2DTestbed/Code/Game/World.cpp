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
	m_backgroundSprite.Init(TexID::Background);
	m_backgroundSprite.SetScale(sf::Vector2f(sX, sY));
	m_backgroundSprite.SetOrigin(sf::Vector2f(0, 0));
	m_backgroundSprite.SetPosition(sf::Vector2f(0, -480));

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
	m_backgroundSprite.Render(window);

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
	for (auto& sprite : m_foregroundSprites)
	{
		if (!camera->IsInView(sprite.second.GetSprite()))
			continue;

		window.draw(*sprite.second.GetSprite());
	}

	// render GUI
	for (auto& sprite : m_sprites)
		window.draw(*sprite.GetSprite());

	for (const auto& text : m_text)
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

void World::AddObject(const sf::Vector2f& pos)
{
	m_objects.push_back(std::make_unique<Object>(TexID::Shroom, 1, 1, (int)TexID::ShroomBB, true, true, 0, 1.f, pos));
}

void World::AddEnemies()
{
	Tile tmp = Collisions::Get()->GetTile(32, 5);
	m_enemies.push_back(std::make_unique<Bill>(false, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(33, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(47, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(55, 8);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(72, 7);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(82, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(89, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(112, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(113, 5);
	m_enemies.push_back(std::make_unique<PPlant>(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	m_foregroundSprites.emplace_back(m_enemies.size() - 1, Sprite());

	tmp = Collisions::Get()->GetTile(138, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(139, 6);
	m_enemies.push_back(std::make_unique<PPlant>(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	m_foregroundSprites.emplace_back(m_enemies.size() - 1, Sprite());

	tmp = Collisions::Get()->GetTile(172, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(182, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(184, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(186, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(189, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(203, 4);
	m_enemies.push_back(std::make_unique<Bill>(true, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(207, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(227, 4);
	m_enemies.push_back(std::make_unique<Bill>(true, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(242, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(257, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(263, 3);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(280, 4);
	m_enemies.push_back(std::make_unique<Bill>(true, sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f)));

	tmp = Collisions::Get()->GetTile(284, 6);
	m_enemies.push_back(std::make_unique<PPlant>(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	m_foregroundSprites.emplace_back(m_enemies.size() - 1, Sprite());

	tmp = Collisions::Get()->GetTile(290, 6);
	m_enemies.push_back(std::make_unique<Rex>(false, 0, .5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY)));

	tmp = Collisions::Get()->GetTile(298, 3);
	m_enemies.push_back(std::make_unique<Chuck>(false, 3, 0.5f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 6.f)));
}

void World::AddObjects()
{
	/*Tile tmp = Collisions::Get()->GetTile(17, 9);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	tmp = Collisions::Get()->GetTile(37, 6);
	m_objects.push_back(std::make_unique<Object>(TexID::QBox, 2, 1, (int)TexID::BoxBB, true, false, 0, 1.f, tmp.GetPosition()));
	std::vector<int> frames{ 1, 1 };
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(88, 9);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	tmp = Collisions::Get()->GetTile(102, 3);
	m_objects.push_back(std::make_unique<Object>(TexID::Shroom, 1, 1, (int)TexID::ShroomBB, true, true, 0, 1.f, tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(120, 6);
	m_objects.push_back(std::make_unique<Object>(TexID::Box, 1, 1, (int)TexID::BoxBB, true, true, 0, 1.f, tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(121, 6);
	m_objects.push_back(std::make_unique<Object>(TexID::Box, 1, 1, (int)TexID::BoxBB, true, true, 0, 1.f, tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(151, 5);
	m_objects.push_back(std::make_unique<Object>(TexID::ChkPnt, 1, 1, (int)TexID::ChkPntBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 10.f)));

	tmp = Collisions::Get()->GetTile(179, 11);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	frames = { 1, 4 };

	tmp = Collisions::Get()->GetTile(239, 6);
	m_objects.push_back(std::make_unique<Object>(TexID::SBox, 2, 4, (int)TexID::BoxBB, true, false, 0, 0.25f, tmp.GetPosition()));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(243, 5);
	m_objects.push_back(std::make_unique<Object>(TexID::SBox, 2, 4, (int)TexID::BoxBB, true, false, 0, 0.25f, tmp.GetPosition()));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(247, 6);
	m_objects.push_back(std::make_unique<Object>(TexID::SBox, 2, 4, (int)TexID::BoxBB, true, false, 0, 0.25f, tmp.GetPosition()));
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(243, 9);
	m_objects.push_back(std::make_unique<Object>(TexID::QBox, 2, 1, (int)TexID::BoxBB, true, false, 0, 1.f, tmp.GetPosition()));
	frames = { 1, 1 };
	m_objects.back()->GetAnimSpr()->SetFrames(frames);

	tmp = Collisions::Get()->GetTile(288, 9);
	m_objects.push_back(std::make_unique<Object>(TexID::YCoin, 1, 1, (int)TexID::YcoinBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f)));

	tmp = Collisions::Get()->GetTile(302, 3);
	m_objects.push_back(std::make_unique<Object>(TexID::Goal, 1, 1, (int)TexID::GoalBB, true, true, 0, 1.f, sf::Vector2f(tmp.GetPosition().x - 6.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 30.f)));*/
}

void World::AddForeGroundSprites()
{
	m_foregroundSprites[0].second.Init(TexID::Pipe1);
	m_foregroundSprites[0].second.SetScale(sf::Vector2f(sX, sY));
	m_foregroundSprites[0].second.SetPosition(m_enemies[m_foregroundSprites[0].first]->GetPosition() + sf::Vector2f(-37.f, 0.f));

	m_foregroundSprites[1].second.Init(TexID::Pipe2);
	m_foregroundSprites[1].second.SetScale(sf::Vector2f(sX, sY));
	m_foregroundSprites[1].second.SetPosition(m_enemies[m_foregroundSprites[1].first]->GetPosition() + sf::Vector2f(-37.f, 0.f));

	m_foregroundSprites[2].second.Init(TexID::Pipe3);
	m_foregroundSprites[2].second.SetScale(sf::Vector2f(sX, sY));
	m_foregroundSprites[2].second.SetPosition(m_enemies[m_foregroundSprites[2].first]->GetPosition() + sf::Vector2f(-37.f, 0.f));
}

void World::AddGUI()
{
	m_font.loadFromFile("Resources/Fonts/arial.ttf");

	std::vector<TexID> ids{ TexID::Name, TexID::Time };

	for (int i = 0; i < (int)GUI::MAX; i++)
	{
		m_sprites[i].Init(ids[i]);
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
