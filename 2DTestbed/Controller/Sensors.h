#pragma once

#include <SFML\Graphics.hpp>
#include "../Utils.h"

class Sensors
{
public:
	Sensors(sf::Vector2f pos, float size);
	~Sensors();
	void Update(sf::Vector2f pos);
	void Render(sf::RenderWindow& window);
	void ClearSensors();
	std::vector<Sensor*> GetSensors();
private:
	sf::Vector2f strPnt;
	float m_size;

	std::vector<Sensor*> m_Sensors;
};