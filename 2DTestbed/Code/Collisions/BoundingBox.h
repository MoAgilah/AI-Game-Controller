#ifndef BoundingBoxH
#define BoundingBoxH

#include "../Drawables/Sprite.h"
#include <SFML\Graphics.hpp>
#include <string>

class Camera;
class Tile;
class BoundingBox
{
public:
	BoundingBox(TexID id);
	~BoundingBox() = default;

	void SetTexture(TexID id);

	void Update(sf::Vector2f pos);
	void Render(sf::RenderWindow& window);

	bool Intersects(Tile tile);
	bool Intersects(BoundingBox* box);

	sf::Sprite* GetSprite() { return m_bbox.GetSprite(); }

	int GetID() const { return m_id; }
	void SetID(int ID) { m_id = ID; }

	int GetBoxNumber() const { return number; }
private:
	bool Intersects(const sf::FloatRect& box);

	int m_id;
	Sprite m_bbox;
	sf::Texture m_bboxTex;

	int number;
	static int numOn;
};

#endif