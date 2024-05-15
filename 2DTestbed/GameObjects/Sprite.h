#ifndef AnimatedSpriteH
#define AnimatedSpriteH

#include <SFML/Graphics.hpp>
#include <string>

#include <vector>
#include <span>

#include "../Utils.h"

class Sprite
{
public:
	explicit Sprite(std::string_view filePath);
	virtual ~Sprite() = default;

	virtual void Render(sf::RenderWindow& window) { window.draw(m_sprite); }

	sf::Sprite* GetSprite() { return &m_sprite; }

	void Move(float x, float y) { m_sprite.move(x, y); }

	const sf::Vector2f& GetPosition() const { return m_sprite.getPosition(); }
	void SetPosition(const sf::Vector2f& pos) { m_sprite.setPosition(pos); }

	const sf::Vector2f& GetOrigin() const { return m_sprite.getOrigin(); }
	void SetOrigin(const sf::Vector2f& pos) { m_sprite.setOrigin(pos); }

	sf::Vector2u GetTextureSize() const { return m_sprite.getTexture()->getSize(); }
	void SetTextureRect(const sf::IntRect& rect) { m_sprite.setTextureRect(rect); }

	void SetScale(const sf::Vector2f& factors) { m_sprite.setScale(factors); }
private:

	sf::Sprite m_sprite;
	sf::Texture m_texture;
};

struct Range
{
	int m_current = 0;
	int m_max;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(std::string_view filePath, int rows, int columns, float framesPerSec, bool symmetrical, int initialAnim, float animationSpeed);
	~AnimatedSprite() final = default;

	void Update(float dt, bool direction = true);

	void ChangeAnim(int animNum);

	void SetFrames(std::span<int> numFrames);

	bool PlayedNumTimes(int val) const { return m_animEnd == val; }
	bool playedOnce() const { return m_animEnd > 0; }
private:
	int m_animEnd = 0;
	float m_currentTime = 0;
	int m_frameTime = 0;
	float m_animSpeed = 0;
	float m_framesPerSecond = 0;
	bool m_symmetrical;

	Range m_frame;
	Range m_animation;

	sf::Vector2u m_frameSize;
	std::vector<int> m_numFrames;
};

#endif