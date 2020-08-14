
#include "../Collisions/BoundingBox.h"
#include "../Game/Camera.h"
#include "../Collisions/Tile.h"

int BoundingBox::numOn = 0;

BoundingBox::BoundingBox(std::string filepath, int id)
{
	number = numOn++;

	m_id = id;

	if (!m_bboxTex.loadFromFile("Sprites/" + filepath + "bbox.png"))
	{
		std::cout << "boundingBox failed to load" << std::endl;
	}

	m_bboxTex.setSmooth(true);
	
	m_bbox.setTexture(m_bboxTex);
	m_bbox.scale(sX, sY);
	m_bbox.setOrigin(m_bboxTex.getSize().x * 0.5f, m_bboxTex.getSize().y * 0.5f);
}

BoundingBox::~BoundingBox()
{

}

void BoundingBox::Update(sf::Vector2f pos)
{
	m_bbox.setPosition(pos);
}

void BoundingBox::Render(sf::RenderWindow& window)
{
	window.draw(m_bbox);
}

bool BoundingBox::Intersects(Tile tile)
{
	return m_bbox.getGlobalBounds().intersects(tile.GetRect().getGlobalBounds());
}

bool BoundingBox::Intersects(BoundingBox* box)
{
	return m_bbox.getGlobalBounds().intersects(box->GetSprite()->getGlobalBounds());
}

sf::Sprite* BoundingBox::GetSprite()
{
	return &m_bbox;
}

int BoundingBox::GetID()
{
	return m_id;
}

void BoundingBox::SetID(int ID)
{
	m_id = ID;
}

int BoundingBox::GetBoxNumber()
{
	return number;
}


