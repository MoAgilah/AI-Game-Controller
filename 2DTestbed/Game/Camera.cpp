#include "../Game/Camera.h"
#include "../Collisions/Tile.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../GameObjects/Player.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
	if (!nameTex.loadFromFile("Sprites/name.png"))
	{
		std::cout << "failed to load name sprite" << std::endl;
	}

	name.setTexture(nameTex);
	name.setOrigin(nameTex.getSize().x * 0.5f, nameTex.getSize().y * 0.5f);
	
	if (!timeTex.loadFromFile("Sprites/time.png"))
	{
		std::cout << "failed to load name sprite" << std::endl;
	}

	time.setTexture(timeTex);
	time.setOrigin(timeTex.getSize().x * 0.5f, timeTex.getSize().y * 0.5f);

	m_font.loadFromFile("Fonts/arial.ttf");

	m_text.push_back(new sf::Text());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize(15);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);
	m_text.back()->setString("x 00");

	m_text.push_back(new sf::Text());
	m_text.back()->setFont(m_font);
	m_text.back()->setCharacterSize((unsigned)15);
	m_text.back()->setOutlineColor(sf::Color::Black);
	m_text.back()->setOutlineThickness(1.f);
	m_text.back()->setFillColor(sf::Color::Yellow);

	displaytime = (int)Timer::Get()->CurrentTime();
	m_text.back()->setString(std::to_string(displaytime));

	tileNum = 0;

	//initialise camera view
	camera.reset(sf::FloatRect(0, 0, scrX, scrY));
	camera.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	camera.setCenter(scrX * 0.5f, scrY * 0.5f);

	//initialise screen bounds
	curScrBounds.left = camera.getCenter().x - scrX * 0.5f;
	curScrBounds.right = camera.getCenter().x + scrX * 0.5f;

	curScrBounds.bottom = camera.getCenter().y - scrX * 0.5f;
	curScrBounds.top = camera.getCenter().y + scrX * 0.5f;
}

Camera::~Camera()
{
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
	camera.reset(sf::FloatRect(posX, 0, scrX, scrY));

	window.setView(camera);
}

Camera* Camera::GetCamera()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}

	return instance;
}

sf::View Camera::GetView()
{
	return camera;
}

void Camera::Update(float deltaTime)
{
	//update the screen boundaries
	curScrBounds.left = camera.getCenter().x - scrX * 0.5f;
	curScrBounds.right = camera.getCenter().x + scrX * 0.5f;
	
	curScrBounds.bottom = camera.getCenter().y - scrX * 0.5f;
	curScrBounds.top = camera.getCenter().y + scrX * 0.5f;

	//update the text positions
	name.setPosition(curScrBounds.left + name.getOrigin().x + 20, curScrBounds.bottom + name.getOrigin().y + 20);
	m_text[0]->setPosition(name.getPosition() + sf::Vector2f(name.getOrigin().x + 10, -9));
	time.setPosition(camera.getCenter().x, curScrBounds.bottom + time.getOrigin().y + 20);

	//update time
	displaytime = (int)Timer::Get()->CurrentTime();
	m_text[1]->setString(std::to_string(displaytime));
	m_text[1]->setPosition(time.getPosition()+ sf::Vector2f(time.getOrigin().x + 10,-9));

	//reset number of tiles extracted
	tileNum = 0;
}

bool Camera::OnScreen(Player* ply)
{
	float screenBot = curScrBounds.bottom;
	float screenTop = curScrBounds.top;

	float pos = ply->GetPosition().y;
	float posLeft = ply->GetPosition().y + ply->GetOrigin().y * sY;
	float posRight = ply->GetPosition().y - ply->GetOrigin().y * sY;

	//if center is on screen
	if (pos > screenBot && pos < screenTop)
	{
		return true;
	}
	else if (pos < screenBot)
	{
		//if center is not on screen check left hand side
		if (posLeft > screenBot)
		{
			return true;
		}
	}
	else if (pos > screenTop)
	{
		//if center is not on screen check right hand side
		if (posRight < screenTop)
		{
			return true;
		}
	}

	return false;
}

bool Camera::IsInView(sf::Vector2f position, sf::Vector2f ori)
{
	float screenLeft = curScrBounds.left;
	float screenRight = curScrBounds.right;

	float pos = position.x;
	float posLeft = pos + ori.x * sX;
	float posRight = pos - ori.x * sX;
	
	//dont let anymore tiles past 240 be visible
	if (tileNum == 240)
	{
		return false;
	}

	//if center is on screen
	if (pos > screenLeft && pos < screenRight)
	{
		tileNum++;
		return true;
	}
	else if (pos < screenLeft)
	{
		//if center is not on screen check left hand side
		if (posLeft > screenLeft)
		{
			tileNum++;
			return true;
		}
	}
	else if (pos > screenRight)
	{
		//if center is not on screen check right hand side
		if (posRight < screenRight)
		{
			tileNum++;
			return true;
		}
	}

	return false;
}

bool Camera::IsInView(sf::Sprite spr)
{
	float screenLeft = curScrBounds.left;
	float screenRight = curScrBounds.right;

	float pos = spr.getPosition().x;
	float posLeft = pos + spr.getOrigin().x * sX;
	float posRight = pos - spr.getOrigin().x * sX;

	//if center on screen
	if (pos > screenLeft && pos < screenRight)
	{
		return true;
	}
	else if (pos < screenLeft)
	{
		//if center is not on screen check left hand side
		if (posLeft > screenLeft)
		{
			return true;
		}
	}
	else if (pos > screenRight)
	{
		//if center is not on screen check right hand side
		if (posRight < screenRight)
		{
			return true;
		}
	}

	return false;
}

bool Camera::IsinView(sf::RectangleShape rect)
{
	float screenLeft = curScrBounds.left;
	float screenRight = curScrBounds.right;
	float pos = rect.getPosition().x;
	float posLeft = pos + rect.getOrigin().x * sX;
	float posRight = pos - rect.getOrigin().x * sX;

	//if center is on screen
	if (pos > screenLeft && pos < screenRight)
	{
		return true;
	}
	else if (pos < screenLeft)
	{
		//if center is not on screen check left hand side
		if (posLeft > screenLeft)
		{
			return true;
		}
	}
	else if (pos > screenRight)
	{
		//if center is not on screen check right hand side
		if (posRight < screenRight)
		{
			return true;
		}
	}

	return false;
}

void Camera::RenderGui(sf::RenderWindow & window)
{
	window.draw(name);
	window.draw(time);

	for (int i = 0; i < m_text.size(); i++)
	{
		window.draw(*m_text[i]);
	}
}
