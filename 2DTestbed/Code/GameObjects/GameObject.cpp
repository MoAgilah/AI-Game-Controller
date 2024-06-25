#include "../GameObjects/GameObject.h"
#include "../Collisions/Collisions.h"
#include "../Game/Camera.h"
#include "../Game/Constants.h"

int GameObject::s_objectNum = 0;

GameObject::GameObject(TexID boxId)
	: m_direction(true)
{
	m_spawnData.m_initialDir = m_direction;
	m_spawnData.m_initialAnim = 0;

	m_bbox = std::make_shared<BoundingBox>(boxId);

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

GameObject::GameObject(TexID sprId, TexID boxId)
	: m_type((int)sprId), m_direction(true)
{
	m_spawnData.m_initialDir = m_direction;
	m_spawnData.m_initialAnim = 0;

	m_spr = std::make_shared<Sprite>(sprId);
	m_bbox = std::make_shared<BoundingBox>(boxId);

	SetDirection(m_spawnData.m_initialDir);

	Collisions::Get()->AddCollidable(this);
	m_objectID = s_objectNum++;
}

void GameObject::Render(sf::RenderWindow& window)
{
	m_spr->Render(window);
	m_bbox->Render(window);
}

void GameObject::Reset()
{
	SetPosition(m_spawnData.m_initialPos);
	SetPrevPosition(m_spawnData.m_initialPos);

	m_visible = false;
	SetDirection(m_spawnData.m_initialDir);
	m_onGround = false;
	m_airbourne = false;
}

void GameObject::SetPosition(sf::Vector2f pos)
{
	m_spr->SetPosition(pos);
	m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x, m_spr->GetPosition().y + 3.5f));
}

void GameObject::SetPosition(float x, float y)
{
	m_spr->SetPosition(sf::Vector2f(x, y));
	m_bbox->Update(sf::Vector2f(m_spr->GetPosition().x, m_spr->GetPosition().y + 3.5f));
}

void GameObject::SetDirection(bool dir)
{
	m_direction = dir;
	if (m_direction)
	{
		// flip X
		m_spr->SetScale({ sX, sY });
	}
	else
	{
		//unflip x
		m_spr->SetScale({ -sX, sY });
	}
}

AnimatedGameObject::AnimatedGameObject(TexID id, int rows, int cols, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: GameObject((TexID)bTyp)
{
	m_type = (int)id;
	m_spr = std::make_shared<AnimatedSprite>(id, rows, cols, FPS, symmetrical, initAnim, animSpd);
	m_spawnData.m_initialAnim = initAnim;
	m_spawnData.m_initialDir = dir;
	SetDirection(m_spawnData.m_initialDir);
}

AnimatedGameObject::AnimatedGameObject(TexID id, int bTyp, bool dir, bool symmetrical, int initAnim, float animSpd)
	: GameObject((TexID)bTyp)
{
	m_type = (int)id;
	m_spr = std::make_shared<AnimatedSprite>(id, FPS, symmetrical, initAnim, animSpd);
	m_spawnData.m_initialAnim = initAnim;
	m_spawnData.m_initialDir = dir;
	SetDirection(m_spawnData.m_initialDir);
}

void AnimatedGameObject::Reset()
{
	GetAnimSpr()->ChangeAnim(m_spawnData.m_initialAnim);
	GameObject::Reset();
}
