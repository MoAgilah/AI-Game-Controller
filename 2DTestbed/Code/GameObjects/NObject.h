#pragma once

#include "GameObject.h"

class Mushroom : public StaticObject
{
public:
	Mushroom(bool dir, const sf::Vector2f& pos);
	~Mushroom() override = default;

	void Update(float deltaTime) override;
};