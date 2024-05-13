#include "../Game/Camera.h"
#include "../Collisions/Tile.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../GameObjects/Player.h"

Camera* Camera::m_instance = nullptr;

Camera::Camera()
{
	if (!m_nameTex.loadFromFile("Sprites/name.png"))
	{
		std::cout << "failed to load name sprite" << std::endl;
	}

	m_nameSpr.setTexture(m_nameTex);
	m_nameSpr.setOrigin((float)m_nameTex.getSize().x * 0.5f, (float)m_nameTex.getSize().y * 0.5f);

	if (!m_timeTex.loadFromFile("Sprites/time.png"))
	{
		std::cout << "failed to load name sprite" << std::endl;
	}

	m_timeSpr.setTexture(m_timeTex);
	m_timeSpr.setOrigin((float)m_timeTex.getSize().x * 0.5f, (float)m_timeTex.getSize().y * 0.5f);

	m_font.loadFromFile("Fonts/arial.ttf");

	m_text.push_back(std::make_unique<sf::Text>());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize(15);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);
	m_text.back()->setString("x 00");

	m_text.push_back(std::make_unique<sf::Text>());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize((unsigned)15);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);

	m_displaytime = (int)Timer::Get()->CurrentTime();
	m_text.back()->setString(std::to_string(m_displaytime));

	m_tileNum = 0;

	//initialise camera view
	m_camera.reset(sf::FloatRect(0, 0, scrX, scrY));
	m_camera.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	m_camera.setCenter(scrX * 0.5f, scrY * 0.5f);

	//initialise screen bounds
	m_curScrBounds.left = m_camera.getCenter().x - scrX * 0.5f;
	m_curScrBounds.right = m_camera.getCenter().x + scrX * 0.5f;

	m_curScrBounds.bottom = m_camera.getCenter().y - scrX * 0.5f;
	m_curScrBounds.top = m_camera.getCenter().y + scrX * 0.5f;
}

void Camera::Reset(sf::RenderWindow& window)
{
	//scroll the screen view with the player

	float posX = Game::GetGameMgr()->GetPlayer()->GetPosition().x - scrX * 0.5f;

	//if not beyond half the screen from initial pos
	if (posX < -1)
	{
		posX = -1;
	}

	//if past the half way point from initial pos
	if (posX > 11734 - scrX)
	{
		posX = 11734 - scrX;
	}

	//reset the camera position
	m_camera.reset(sf::FloatRect(posX, 0, scrX, scrY));

	window.setView(m_camera);
}

Camera* Camera::GetCamera()
{
	if (m_instance == nullptr)
	{
		m_instance = new Camera();
	}

	return m_instance;
}

bool Camera::OnScreen(const Player* ply) const
{
	float screenBot = m_curScrBounds.bottom;
	float screenTop = m_curScrBounds.top;

	float pos = ply->GetPosition().y;
	float posLeft = ply->GetPosition().y + ply->GetOrigin().y * sY;
	float posRight = ply->GetPosition().y - ply->GetOrigin().y * sY;

	//if center is on screen
	if (pos > screenBot && pos < screenTop)
	{
		return true;
	}
	else if (pos < screenBot && posLeft > screenBot)
	{
		return true;
	}
	else if (pos > screenTop && posRight < screenTop)
	{
		return true;
	}

	return false;
}

bool Camera::IsInView(sf::Vector2f position, sf::Vector2f origin)
{
	float screenLeft = m_curScrBounds.left;
	float screenRight = m_curScrBounds.right;

	float pos = position.x;
	float posLeft = pos + origin.x * sX;
	float posRight = pos - origin.x * sX;

	////dont let anymore tiles past 240 be visible
	//if (m_tileNum == 240)
	//	return false;

	//if center is on screen
	if (pos > screenLeft && pos < screenRight
		|| pos < screenLeft && posLeft > screenLeft
		|| pos > screenRight && posRight < screenRight)
	{
		//m_tileNum++;
		return true;
	}
	//if center is not on screen check left hand side
	else if (pos < screenLeft && posLeft > screenLeft)
	{
		//m_tileNum++;
		return true;
	}
	//if center is not on screen check right hand side
	else if (pos > screenRight && posRight < screenRight)
	{
		//m_tileNum++;
		return true;
	}

	return false;
}

bool Camera::IsInView(const sf::Sprite* spr) const
{
	float screenLeft = m_curScrBounds.left;
	float screenRight = m_curScrBounds.right;

	float pos = spr->getPosition().x;
	float posLeft = pos + spr->getOrigin().x * sX;
	float posRight = pos - spr->getOrigin().x * sX;

	//if center on screen
	if (pos > screenLeft && pos < screenRight)
	{
		return true;
	}
	//if center is not on screen check left hand side
	else if (pos < screenLeft && posLeft > screenLeft)
	{
		return true;
	}
	//if center is not on screen check right hand side
	else if (pos > screenRight && posRight < screenRight)
	{
		return true;
	}

	return false;
}

bool Camera::IsinView(const sf::RectangleShape& rect) const
{
	float screenLeft = m_curScrBounds.left;
	float screenRight = m_curScrBounds.right;
	float pos = rect.getPosition().x;
	float posLeft = pos + rect.getOrigin().x * sX;
	float posRight = pos - rect.getOrigin().x * sX;

	//if center is on screen
	if (pos > screenLeft && pos < screenRight)
	{
		return true;
	}
	//if center is not on screen check left hand side
	else if (pos < screenLeft && posLeft > screenLeft)
	{
		return true;
	}
	//if center is not on screen check right hand side
	else if (pos > screenRight && posRight < screenRight)
	{
		return true;
	}

	return false;
}

void Camera::Update()
{
	//update the screen boundaries
	m_curScrBounds.left = m_camera.getCenter().x - scrX * 0.5f;
	m_curScrBounds.right = m_camera.getCenter().x + scrX * 0.5f;

	m_curScrBounds.bottom = m_camera.getCenter().y - scrX * 0.5f;
	m_curScrBounds.top = m_camera.getCenter().y + scrX * 0.5f;

	//update the text positions
	m_nameSpr.setPosition(m_curScrBounds.left + m_nameSpr.getOrigin().x + 20, m_curScrBounds.bottom + m_nameSpr.getOrigin().y + 20);
	m_text[0]->setPosition(m_nameSpr.getPosition() + sf::Vector2f(m_nameSpr.getOrigin().x + 10, -9));
	m_timeSpr.setPosition(m_camera.getCenter().x, m_curScrBounds.bottom + m_timeSpr.getOrigin().y + 20);

	//update time
	m_displaytime = (int)Timer::Get()->CurrentTime();
	m_text[1]->setString(std::to_string(m_displaytime));
	m_text[1]->setPosition(m_timeSpr.getPosition() + sf::Vector2f(m_timeSpr.getOrigin().x + 10, -9));

	//reset number of tiles extracted
	m_tileNum = 0;
}

void Camera::RenderGui(sf::RenderWindow& window) const
{
	window.draw(m_nameSpr);
	window.draw(m_timeSpr);

	for (auto const& text : m_text)
		window.draw(*text.get());
}
