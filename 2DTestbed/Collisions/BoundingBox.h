#ifndef BoundingBoxH
#define BoundingBoxH

#include <SFML\Graphics.hpp>
#include "../Utils.h"

#include <iostream>
#include <string>

class Camera;
class Tile;
class BoundingBox
{
public:
	BoundingBox(std::string filepath, int id);
	~BoundingBox();

	void Update(sf::Vector2f pos);
	void Render(sf::RenderWindow& window);

	bool Intersects(Tile tile);
	bool Intersects(BoundingBox* box);
	
	sf::Sprite* GetSprite();
	int GetID();
	void SetID(int ID);

	int GetBoxNumber();
private:
	int m_id;
	sf::Sprite m_bbox;
	sf::Texture m_bboxTex;

	int number;
	static int numOn;
};

#endif