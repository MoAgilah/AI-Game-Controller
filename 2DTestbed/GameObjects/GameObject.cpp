#include "../GameObjects/GameObject.h"
#include "../../Collisions/Collisions.h"
#include "../Game/Camera.h"

int GameObject::s_objectNum = 0;
int GameObject::s_numOfEnemies = 0;
int GameObject::s_numOfObjects = 0;

GameObject::GameObject(std::string filepath, int rows, int cols, float fps, int bTyp, int strloc, bool dir, bool symmetrical, int initAnim, float animSpd)
{
	m_spr = std::make_unique<AnimatedSprite>(filepath, rows, cols, fps, strloc, symmetrical, initAnim, animSpd);

	//extract id for bounding box
	std::string s_id = filepath.substr(0, strloc);
	m_bbox = std::make_unique<BoundingBox>(s_id, bTyp);

	m_initialDir = m_direction = dir;

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

void GameObject::SetOnGround(bool grnd)
{
	if (grnd)
	{
		m_onGround = true;
		m_falling = false;
	}
	else
	{
		m_onGround = false;
		m_falling = true;
	}
}



