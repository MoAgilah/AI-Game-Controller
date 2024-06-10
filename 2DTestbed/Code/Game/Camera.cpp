#include "../Game/Camera.h"
#include "../Collisions/Tile.h"
#include "../Game/Timer.h"
#include "../Game/Game.h"
#include "../GameObjects/Player.h"
#include <assert.h>
#include "../Game/Constants.h"

Camera::Camera()
{
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

void Camera::Update()
{
	//scroll the screen view with the player
	float posX = Game::GetGameMgr()->GetPlayer()->GetPosition().x - scrX * 0.5f;

	if (posX < 0)
		posX = 0;

	//reset the m_camera position
	m_camera.reset(sf::FloatRect(posX, 0, scrX, scrY));

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
	window.setView(m_camera);
}
