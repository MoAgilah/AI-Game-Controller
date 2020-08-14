#ifndef TileH
#define TileH

#include <SFML\Graphics.hpp>
#include "../Utils.h"

#include <vector>

enum Types
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, CRN, OWAY
};

class Camera;
class Tile
{
public:
	Tile();
	Tile(sf::Font& font);
	~Tile();

	void SetID(int gX, int gY);
	std::string GetID();

	void SetType(int type);
	int GetType();

	void SetPosition(sf::Vector2f pos);
	sf::Vector2f GetPosition();

	bool GetVisible();
	void SetVisible(bool vis);

	sf::Vector2f GetOrigin();
	sf::RectangleShape GetRect();
	std::vector<sf::RectangleShape> GetSlopeBBox();
	int GetRowNum();
	int GetCowNum();

	void SetFillColour(sf::Color col);
	
	void Render(sf::RenderWindow& window);
private:
	void InvertY(int& y);
	void CreateDesSlope();
	void CreateAscSlope();

	int m_colNum;
	int m_rowNum;
	int m_type;
	bool visible;
	bool hasFont;
	std::string m_id;
	//Tile
	sf::RectangleShape m_rect;
	sf::Text m_text;
	sf::Font* m_font;
	//Tile's Slope
	sf::RectangleShape m_srect;
	std::vector<sf::RectangleShape> m_slope;

	
};


#endif
