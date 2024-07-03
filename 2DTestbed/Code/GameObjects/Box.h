#pragma once
#include "Object.h"
#include <memory>
#include <vector>

class Box : public Object
{
public:
	Box(AnimatedSprite* sprite, TexID boxId, const sf::Vector2f& initPos);
	~Box() override = default;

	void Reset() override;

	void SetCanHit(bool canHit) { m_canHit = canHit; }
	bool GetCanHit() const { return m_canHit; }
	void SetJustHit(bool justHit) { m_justHit = justHit; }
	bool GetJustHit() const { return m_justHit; }

	void WasJustHit();

private:

	virtual void Animate(float deltaTime) = 0;

	bool m_canHit = true;
	bool m_justHit = false;
};

class QBox : public Box
{
public:
	explicit QBox(const sf::Vector2f& initPos);
	~QBox() override = default;

	void Update(float deltaTime) final;

	void Reset() override;

};

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
	void Scatter(int idx, AnimatedSprite* sprite);

	bool m_justSmashed = false;
	std::vector<std::shared_ptr<AnimatedSprite>> m_boxPieces;
};