#include "../Game/Camera.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"
#include "../Game/Constants.h"

Camera::Camera()
{

	// initialise m_camera view
	m_camera.reset(sf::FloatRect(0, 0, GameConstants::ScreenDim.x, GameConstants::ScreenDim.y));
	m_camera.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	m_camera.setCenter(GameConstants::ScreenDim * 0.5f);

	m_viewBox.Reset(sf::Vector2f(253,237));
	m_viewBox.Update(m_camera.getCenter());
	m_viewBox.SetFillColour(sf::Color(255, 0, 0, 128));
	GameConstants::LeftMost = m_viewBox.GetMin().x;
}


void Camera::Update()
{
	//scroll the screen view with the player
	float posX = GameManager::Get()->GetPlayer()->GetPosition().x - GameConstants::ScreenDim.x * 0.5f;

	if (posX < 0)
		posX = 0;

	//reset the m_camera position
	m_camera.reset(sf::FloatRect(posX, 0, GameConstants::ScreenDim.x, GameConstants::ScreenDim.y));
	m_viewBox.Update(m_camera.getCenter());
}

void Camera::Reset(sf::RenderWindow& window)
{
	window.setView(m_camera);
	Update();
}

bool Camera::IsInView(AABB* box)
{
	return box->Intersects(&m_viewBox);
}

bool Camera::CheckVerticalBounds(AABB* box)
{
	return box->GetPosition().y > (m_camera.getCenter().y + (GameConstants::ScreenDim.y * 0.5f)) - (box->GetExtents().y * 2);
}

void Camera::RenderViewBox(sf::RenderWindow& window)
{
	m_viewBox.Render(window);
}
