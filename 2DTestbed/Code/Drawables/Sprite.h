#pragma once

#include <SFML/Graphics.hpp>
#include "../Game/TextureManager.h"
#include <string>

#include <vector>
#include <span>

class Sprite
{
public:
	Sprite() = default;
	explicit Sprite(TexID id);
	virtual ~Sprite() = default;

	TexID GetTexID() const { return m_texID; }

	void SetTexture(TexID id);

	void Render(sf::RenderWindow& window) const { window.draw(m_sprite); }

	sf::Sprite* GetSprite() { return &m_sprite; }

	void Move(float x, float y) { m_sprite.move(x, y); }

	const sf::Vector2f& GetPosition() const { return m_sprite.getPosition(); }
	void SetPosition(const sf::Vector2f& pos) { m_sprite.setPosition(pos); }

	const sf::Vector2f& GetOrigin() const { return m_sprite.getOrigin(); }
	void SetOrigin(const sf::Vector2f& pos) { m_sprite.setOrigin(pos); }

	sf::Vector2u GetTextureSize() const { return m_sprite.getTexture()->getSize(); }
	void SetTextureRect(const sf::IntRect& rect) { m_sprite.setTextureRect(rect); }

	void SetScale(const sf::Vector2f& factors) { m_sprite.setScale(factors); }

	sf::Vector2u GetFrameSize() const { return m_frameSize; }
	void SetFrameSize(const sf::Vector2u& size, int currentFrame = 1, int currentAnim = 1);

private:

	TexID m_texID;
	sf::Sprite m_sprite;
	sf::Vector2u m_frameSize;
};

struct Range
{
	int m_current = 0;
	int m_max;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(TexID id, int rows, int columns, float framesPerSec, bool symmetrical, float animationSpeed);
	AnimatedSprite(TexID id, float framesPerSec, bool symmetrical, float animationSpeed);
	~AnimatedSprite() final = default;

	void Update(float dt);

	void ChangeAnim(int animNum);

	void SetFrames(const std::vector<int>& numFrames);
	void SetFrameData(int rows, int columns, const std::vector<int>& numFrames);

	bool PlayedNumTimes(int val) const { return m_animCycles == val; }
	bool PlayedOnce() const { return m_animCycles > 0; }

	void SetShouldLoop(bool loop) { m_loop = loop; }
private:
	bool m_loop = true;
	int m_animCycles = 0;
	float m_currentTime = 0;
	int m_frameTime = 0;
	float m_animSpeed = 0;
	float m_framesPerSecond = 0;
	bool m_symmetrical;

	Range m_frame;
	Range m_animation;


	std::vector<int> m_numFrames;
};