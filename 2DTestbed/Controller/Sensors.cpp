#include "../Controller/Sensors.h"
#include "../Game/Game.h"

Sensors::Sensors(sf::Vector2f pos, float size)
	:strPnt(pos), m_size(size)
{
	for (int i = 0; i < 8; ++i)
	{
		m_Sensors.push_back(new Sensor);
		m_Sensors.back()->ClearSensor();
	}

	Update(pos);
}

Sensors::~Sensors()
{
	for (int i = 0; i < 8; ++i)
	{
		delete m_Sensors[i];
		m_Sensors[i] = nullptr;
	}

	m_Sensors.clear();
}

void Sensors::Update(sf::Vector2f pos)
{
	strPnt = pos;
	float size = 16;
	sf::Vector2f halway;

	for (int i = 0; i < 8; ++i)
	{
		m_Sensors[i]->points[0] = sf::Vertex(sf::Vector2f(strPnt.x, strPnt.y));
	}

	//forward
	m_Sensors[0]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x + size * sX, strPnt.y));
	halway = m_Sensors[0]->points[1].position;
	m_Sensors[0]->points[2] = sf::Vertex(sf::Vector2f(halway.x + size * sX, halway.y));

	//diagonal down right
	m_Sensors[1]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x + size * sX, strPnt.y + size * sY));
	halway = m_Sensors[1]->points[1].position;
	m_Sensors[1]->points[2] = sf::Vertex(sf::Vector2f(halway.x + size * sX, halway.y + size * sY));
	
	//down
	m_Sensors[2]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x, strPnt.y + size * sY));
	halway = m_Sensors[2]->points[1].position;
	m_Sensors[2]->points[2] = sf::Vertex(sf::Vector2f(halway.x, halway.y + size * sY));

	//diagonal down right
	m_Sensors[3]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x + size * sX, strPnt.y - size * sY));
	halway = m_Sensors[3]->points[1].position;
	m_Sensors[3]->points[2] = sf::Vertex(sf::Vector2f(halway.x + size * sX, halway.y - size * sY));
	
	//backward
	m_Sensors[4]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x - size * sX, strPnt.y));
	halway = m_Sensors[4]->points[1].position;
	m_Sensors[4]->points[2] = sf::Vertex(sf::Vector2f(halway.x - size * sX, halway.y));
	
	//daigonal up left
	m_Sensors[5]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x - size * sX, strPnt.y - size * sY));
	halway = m_Sensors[5]->points[1].position;
	m_Sensors[5]->points[2] = sf::Vertex(sf::Vector2f(halway.x - size * sX, halway.y - size * sY));

	//diagonal up
	m_Sensors[6]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x, strPnt.y - size * sY));
	halway = m_Sensors[6]->points[1].position;
	m_Sensors[6]->points[2] = sf::Vertex(sf::Vector2f(halway.x, halway.y - size * sY));
	
	//diagonal up left
	m_Sensors[7]->points[1] = sf::Vertex(sf::Vector2f(strPnt.x - size * sX, strPnt.y + size * sY));
	halway = m_Sensors[7]->points[1].position;
	m_Sensors[7]->points[2] = sf::Vertex(sf::Vector2f(halway.x - size * sX, halway.y + size * sY));
}

void Sensors::Render(sf::RenderWindow & window)
{
	for (int i = 0; i < m_Sensors.size(); ++i)
	{
		window.draw(m_Sensors[i]->points, 3, sf::LineStrip);
	}
}

void Sensors::ClearSensors()
{
	for (int i = 0; i < m_Sensors.size(); ++i)
	{
		Sensor* tmpSen = m_Sensors[i];
		tmpSen->ClearSensor();
	}
}

std::vector<Sensor*> Sensors::GetSensors()
{
	return m_Sensors;
}
