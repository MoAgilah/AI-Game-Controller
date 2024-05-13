#include "../Game/Level.h"
#include "../Game/Camera.h"

#include "../../Collisions/BoundingBox.h"
#include "../../Collisions/Collisions.h"
#include "../GameObjects/Object.h"
#include "../GameObjects/Koopa.h"
#include "../GameObjects/Bill.h"
#include "../GameObjects/Rex.h"
#include "../GameObjects/PPlant.h"
#include "../GameObjects/Chuck.h"

Level::Level()
{
	m_textures.push_back(std::make_unique<sf::Texture>());
	m_textures.back()->setSmooth(true);

	if (!m_textures.back()->loadFromFile("Sprites/background.png"))
	{
		std::cout << "background failed to load" << std::endl;
	}

	m_sprites.push_back(std::make_unique<sf::Sprite>());
	m_sprites.back()->setTexture(*m_textures.front());
	m_sprites.back()->setScale(sX, sY);
	m_sprites.back()->setPosition(0, -480);

	m_textures.push_back(std::make_unique<sf::Texture>());
	if (!m_textures.back()->loadFromFile("Sprites/ppPipe1.png"))
	{
		std::cout << "pipe purple failed to load" << std::endl;
	}
	m_textures.back()->setSmooth(true);

	m_textures.push_back(std::make_unique<sf::Texture>());
	if (!m_textures.back()->loadFromFile("Sprites/ppPipe2.png"))
	{
		std::cout << "pipe purple failed to load" << std::endl;
	}
	m_textures.back()->setSmooth(true);

	m_textures.push_back(std::make_unique<sf::Texture>());
	if (!m_textures.back()->loadFromFile("Sprites/ppPipe3.png"))
	{
		std::cout << "pipe purple failed to load" << std::endl;
	}
	m_textures.back()->setSmooth(true);

	AddEnemies();
	AddObjects();
}

void Level::AddObject(const std::string& filepath, int rows, int cols, float fps, int bTyp, int strloc, const sf::Vector2f& position, bool onTheGround)
{
	m_objects.push_back(std::make_unique<Object>(filepath, rows, cols, fps, bTyp, strloc));
	m_objects.back()->SetPosition(position);
	m_objects.back()->SetOnGround(onTheGround);
}

void Level::Update(float deltaTime)
{
	for (const auto& enemy : m_enemy)
		enemy->Update(deltaTime);

	for (const auto& object : m_objects)
		object->Update(deltaTime);
}

void Level::Render(sf::RenderWindow & window)
{
	window.draw(*m_sprites.front());

	for (const auto& enemy : m_enemy)
	{
		if (!enemy->GetActive())
			continue;

		if (!enemy->GetVisible())
			continue;

		enemy->Render(window);
	}

	for (const auto& object : m_objects)
	{
		if (!object->GetActive())
			continue;

		if (!object->GetVisible())
			continue;

		object->Render(window);
	}

	for (const auto& sprite : m_sprites)
	{
		if (!Camera::GetCamera()->IsInView(sprite.get()))
			continue;

		window.draw(*sprite.get());
	}
}

void Level::ResetLevel()
{
	for (const auto& enemy : m_enemy)
		enemy->Revive();

	m_objects.resize(14);

	for (const auto& object : m_objects)
		object->Reset();
}

void Level::AddEnemies()
{
	Tile tmp = Collisions::Get()->GetTile(32, 5);
	m_enemy.push_back(std::make_unique<Bill>("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX-10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY + 3.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(33, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(47, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(55, 8);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(72, 7);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(82, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(89, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(112, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(113, 5);
	m_enemy.push_back(std::make_unique<PPlant>("pplant.png", 2, 2, FPS, PPLANT, 2, false, false, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	m_sprites.push_back(std::make_unique<sf::Sprite>());
	m_sprites.back()->setTexture(*m_textures[1]);
	m_sprites.back()->setScale(sX, sY);
	m_sprites.back()->setOrigin(m_textures[1]->getSize().x / 2.f, m_textures[1]->getSize().x / 2.f);
	m_sprites.back()->setPosition(m_enemy.back()->GetPosition() + sf::Vector2f(0.f, 40.f));

	tmp = Collisions::Get()->GetTile(138, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(139, 6);
	m_enemy.push_back(std::make_unique<PPlant>("pplant.png", 2, 2, FPS, PPLANT, 2, false, false, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	m_sprites.push_back(std::make_unique<sf::Sprite>());
	m_sprites.back()->setTexture(*m_textures[2]);
	m_sprites.back()->setScale(sX, sY);
	m_sprites.back()->setOrigin(m_textures[1]->getSize().x / 2.f, m_textures[1]->getSize().x / 2.f);
	m_sprites.back()->setPosition(m_enemy.back()->GetPosition() + sf::Vector2f(0.f, 40.f));

	tmp = Collisions::Get()->GetTile(172, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(182, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(184, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(186, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(189, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(203, 4);
	m_enemy.push_back(std::make_unique<Bill>("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY - 2.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(207, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(227, 4);
	m_enemy.push_back(std::make_unique<Bill>("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY - 2.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(242, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(257, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(263, 3);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(280, 4);
	m_enemy.push_back(std::make_unique<Bill>("bill.png", 1, 1, FPS, BILL, 1, false, true, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x *sX - 10.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY - 2.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(284, 6);
	m_enemy.push_back(std::make_unique<PPlant>("pplant.png", 2, 2, FPS, PPLANT, 2, false, false, 0, 1.f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x + tmp.GetOrigin().x * sX, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	m_sprites.push_back(std::make_unique<sf::Sprite>());
	m_sprites.back()->setTexture(*m_textures[3]);
	m_sprites.back()->setScale(sX, sY);
	m_sprites.back()->setOrigin(m_textures[1]->getSize().x / 2.f, m_textures[1]->getSize().x / 2.f);
	m_sprites.back()->setPosition(m_enemy.back()->GetPosition() + sf::Vector2f(0.f, 40.f));

	tmp = Collisions::Get()->GetTile(290, 6);
	m_enemy.push_back(std::make_unique<Rex>("rex.png", 3, 2, FPS, REX, 1, false, false, 0, .5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());

	tmp = Collisions::Get()->GetTile(298, 3);
	m_enemy.push_back(std::make_unique<Chuck>("chuck.png", 6, 2, FPS, CHUCK, 1, false, false, 3, 0.5f));
	m_enemy.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y *sY + 6.f));
	m_enemy.back()->SetInitialPos(m_enemy.back()->GetPosition());
}

void Level::AddObjects()
{
	Tile tmp = Collisions::Get()->GetTile(17, 9);
	m_objects.push_back(std::make_unique<Object>("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x,tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(37, 6);
	m_objects.push_back(std::make_unique<Object>("bopwr.png", 2, 1, FPS, QBOX, 2, true, false, 0, 1.f));
	m_objects.back()->GetAnimSpr()->SetFrames({ 1, 1 });
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(88, 9);
	m_objects.push_back(std::make_unique<Object>("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(102, 3);
	m_objects.push_back(std::make_unique<Object>("shroom.png", 1, 1, FPS, SHROOM, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(120, 6);
	m_objects.push_back(std::make_unique<Object>("boassort.png", 1, 1, FPS, SMBOX, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(121, 6);
	m_objects.push_back(std::make_unique<Object>("boassort.png", 1, 1, FPS, SMBOX, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(151, 5);
	m_objects.push_back(std::make_unique<Object>("chpnt.png", 1, 1, FPS, CHKPOINT, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * sY +10.f));

	tmp = Collisions::Get()->GetTile(179, 11);
	m_objects.push_back(std::make_unique<Object>("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(239, 6);
	m_objects.push_back(std::make_unique<Object>("bospin.png", 2, 4, FPS, SPBOX, 2, true, false, 0, 0.25f));
	m_objects.back()->GetAnimSpr()->SetFrames({ 1, 4 });
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(243, 5);
	m_objects.push_back(std::make_unique<Object>("bospin.png", 2, 4, FPS, SPBOX, 2, true, false, 0, 0.25f));
	m_objects.back()->GetAnimSpr()->SetFrames({ 1, 4 });
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(247, 6);
	m_objects.push_back(std::make_unique<Object>("bospin.png", 2, 4, FPS, SPBOX, 2, true, false, 0, 0.25f));
	m_objects.back()->GetAnimSpr()->SetFrames({ 1, 4 });
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(243, 9);
	m_objects.push_back(std::make_unique<Object>("bopwr.png", 2, 1, FPS, QBOX, 2, true, false, 0, 1.f));
	m_objects.back()->GetAnimSpr()->SetFrames({ 1, 1 });
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition()));

	tmp = Collisions::Get()->GetTile(288, 9);
	m_objects.push_back(std::make_unique<Object>("ycoin.png", 1, 1, FPS, YCOIN, 2, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x, tmp.GetPosition().y - tmp.GetOrigin().y * 2.f - 4.f));

	tmp = Collisions::Get()->GetTile(302, 3);
	m_objects.push_back(std::make_unique<Object>("goal.png", 1, 1, FPS, GOAL,1, true, true, 0, 1.f));
	m_objects.back()->SetPosition(sf::Vector2f(tmp.GetPosition().x-6.f, tmp.GetPosition().y - tmp.GetOrigin().y * sY +30.f));
}

