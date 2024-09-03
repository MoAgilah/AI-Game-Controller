#include "../Game/Camera.h"
#include "../Game/Timer.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Player.h"
#include "../Game/Constants.h"

Camera::Camera()
{

	// initialise m_camera view
	m_camera.reset(sf::FloatRect(0, 0, screenDim.x, screenDim.y));
	m_camera.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	m_camera.setCenter(screenDim * 0.5f);

	m_viewBox.Reset(sf::Vector2f(253,237));
	m_viewBox.Update(m_camera.getCenter());
	m_viewBox.SetFillColour(sf::Color(255, 0, 0, 128));
}


void Camera::Update()
{
	//scroll the screen view with the player
	float posX = GameManager::GetGameMgr()->GetPlayer()->GetPosition().x - screenDim.x * 0.5f;

	if (posX < 0)
		posX = 0;

	//reset the m_camera position
	m_camera.reset(sf::FloatRect(posX, 0, screenDim.x, screenDim.y));
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

void Camera::RenderViewBox(sf::RenderWindow& window)
{
	m_viewBox.Render(window);
}
