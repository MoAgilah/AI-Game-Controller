#pragma once

#include <memory>
#include <vector>
#include "Object.h"

class Box : public Object
{
public:
	Box(AnimatedSprite* sprite, const sf::Vector2f& initPos);
	~Box() override = default;

	void Reset() override;

	void SetCanHit(bool canHit) { m_canHit = canHit; }
	bool GetCanHit() const { return m_canHit; }
	void SetJustHit(bool justHit) { m_justHit = justHit; }
	bool GetJustHit() const { return m_justHit; }

	void WasJustHit();

private:

	bool m_canHit = true;
	bool m_justHit = false;
};

enum QBoxAnims { DUD, ROTATE };

class QBox : public Box
{
public:
	explicit QBox(const sf::Vector2f& initPos);
	~QBox() override = default;

	void Update(float deltaTime) final;

	void Reset() override;

};

enum SBoxAnims { WAIT, SPIN };
enum ShatterAnims { SCATTER, DROP };

class SBox : public Box

{
public:
	explicit SBox(const sf::Vector2f& initPos);
	~SBox() override = default;
	void Update(float deltaTime) final;
	void Render(sf::RenderWindow& window) final;

	void Reset() override;

	bool GetJustSmashed() const { return m_justSmashed; }
	void SetJustSmashed(bool justSmashed) { m_justSmashed = justSmashed; }

private:
	void Scatter();

	bool m_justSmashed = false;
};