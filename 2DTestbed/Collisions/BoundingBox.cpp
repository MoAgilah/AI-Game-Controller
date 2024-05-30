
#include "../Collisions/BoundingBox.h"
#include "../Game/Camera.h"
#include "../Collisions/Tile.h"
#include "../Game/Constants.h"

#include <format>

int BoundingBox::numOn = 0;

BoundingBox::BoundingBox(std::string_view filepath, int id)
	:m_id(id), m_bbox(std::format("{}{}",filepath, "Box.png"))
{
	m_bbox.SetScale(sf::Vector2f(sX, sY));
	m_bbox.SetOrigin(sf::Vector2f((float)m_bbox.GetTextureSize().x * 0.5f, (float)m_bbox.GetTextureSize().y * 0.5f));
	number = numOn++;
}

void BoundingBox::Update(sf::Vector2f pos)
{
	m_bbox.SetPosition(pos);
}

void BoundingBox::Render(sf::RenderWindow& window)
{
	m_bbox.Render(window);
}

bool BoundingBox::Intersects(Tile tile)
{
	return Intersects(tile.GetRect().getGlobalBounds());
}

bool BoundingBox::Intersects(BoundingBox* box)
{
	return Intersects(box->GetSprite()->getGlobalBounds());
}

bool BoundingBox::Intersects(const sf::FloatRect& box)
{
	return m_bbox.GetSprite()->getGlobalBounds().intersects(box);
}


