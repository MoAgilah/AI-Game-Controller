#include "../Game/Camera.h"
#include "../Collisions/Tile.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../GameObjects/Player.h"
#include <assert.h>

Camera::Camera()
	: m_sprites{ Sprite("name.png"), Sprite("time.png") }
{
	std::vector<std::string> filenames {"Sprites/name.png", "Sprites/time.png"};

	m_font.loadFromFile("Fonts/arial.ttf");

	for (int i = 0; i < (int)GUI::MAX; i++)
	{
		m_text[i].setFont(m_font);
		m_text[i].setCharacterSize(15);
		m_text[i].setOutlineColor(sf::Color::Black);
		m_text[i].setOutlineThickness(1.f);
		m_text[i].setFillColor(sf::Color::Yellow);
	}

	m_text[(int)GUI::NAME].setString("x 00");
	m_text[(int)GUI::TIME].setString(std::to_string((int)Timer::Get()->CurrentTime()));

	//initialise m_camera view
	m_camera.reset(sf::FloatRect(0, 0, scrX, scrY));
	m_camera.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	m_camera.setCenter(scrX * 0.5f, scrY * 0.5f);

	//initialise screen bounds
	m_curScrBounds.left = m_camera.getCenter().x - scrX * 0.5f;
	m_curScrBounds.width = scrX;

	m_curScrBounds.top = m_camera.getCenter().y - scrY * 0.5f;
	m_curScrBounds.height = scrY;
}

bool Camera::OnScreen(const Player* ply) const
{
	float screenBot = m_curScrBounds.top + m_curScrBounds.height;
	float screenTop = m_curScrBounds.top;

	float pos = ply->GetPosition().y;
	float posLeft = ply->GetPosition().y + ply->GetOrigin().y * sY;
	float posRight = ply->GetPosition().y - ply->GetOrigin().y * sY;

	//if center is on screen
	if (pos < screenBot && pos > screenTop)
	{
		return true;
	}
	else if (pos > screenBot)
	{
		//if center is not on screen check left hand side
		if (posLeft < screenBot)
			return true;
	}
	else if (pos < screenTop)
	{
		//if center is not on screen check right hand side
		if (posRight > screenTop)
			return true;
	}

	return false;
}

bool Camera::IsInView(const sf::Vector2f& position, const sf::Vector2f& origin) const
{
	float screenLeft = m_curScrBounds.left;
	float screenRight = m_curScrBounds.left + m_curScrBounds.width;

	float pos = position.x;
	float posLeft = pos + origin.x * sX;
	float posRight = pos - origin.x * sX;

	//if center is on screen
	if (pos > screenLeft && pos < screenRight)
	{
		return true;
	}
	else if (pos < screenLeft)
	{
		//if center is not on screen check left hand side
		if (posLeft > screenLeft)
			return true;
	}
	else if (pos > screenRight)
	{
		//if center is not on screen check right hand side
		if (posRight < screenRight)
			return true;
	}

	return false;
}

bool Camera::IsInView(const sf::Sprite* spr) const
{
	return IsInView(spr->getPosition(), spr->getOrigin());
}

bool Camera::IsinView(const sf::RectangleShape& rect) const
{
	return IsInView(rect.getPosition(), rect.getOrigin());
}

void Camera::Reset(sf::RenderWindow& window)
{
	//scroll the screen view with the player
	float posX = Game::GetGameMgr()->GetPlayer()->GetPosition().x - scrX * 0.5f;

	if (posX < 0)
		posX = 0;

	//reset the m_camera position
	m_camera.reset(sf::FloatRect(posX, 0, scrX, scrY));

	window.setView(m_camera);
}

void Camera::UpdateGUI(float deltaTime)
{
	//update the screen boundaries
	m_curScrBounds.left = m_camera.getCenter().x - scrX * 0.5f;
	m_curScrBounds.top = m_camera.getCenter().y - scrY * 0.5f;

	m_sprites[(int)GUI::NAME].SetPosition(sf::Vector2f(m_curScrBounds.left + m_sprites[(int)GUI::NAME].GetOrigin().x + 20,
		m_curScrBounds.top + m_sprites[(int)GUI::NAME].GetOrigin().y + 20));

	m_sprites[(int)GUI::TIME].SetPosition(sf::Vector2f(m_camera.getCenter().x,
		m_curScrBounds.top + m_sprites[(int)GUI::TIME].GetOrigin().y + 20));

	for (int i = 0; i < (int)GUI::MAX; i++)
		m_text[i].setPosition(m_sprites[i].GetPosition() + sf::Vector2f(m_sprites[i].GetOrigin().x + 10, -9));

	Timer* timer = Timer::Get();

	timer->UpdateTime(deltaTime);
	m_text[(int)GUI::TIME].setString(std::to_string((int)timer->CurrentTime()));

	timer = nullptr;
}

void Camera::RenderGUI(sf::RenderWindow& window)
{
	for (auto& sprite : m_sprites)
		window.draw(*sprite.GetSprite());

	for (const auto& text : m_text)
		window.draw(text);
}
