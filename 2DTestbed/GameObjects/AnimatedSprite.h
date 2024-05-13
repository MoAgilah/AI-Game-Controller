#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

#include "../Utils.h"

class AnimatedSprite
{
public:
	AnimatedSprite(std::string filePath, int rows, int columns, float framesPerSec, int id, bool symmetrical, int m_initialAnim, float animationSpeed);
	~AnimatedSprite();

	void Update(float dt, bool direction = true);
	void Render(sf::RenderWindow& window);

	void ChangeAnim(int animNum);
	void Move(float x, float y);

	void SetFrames(std::vector<int> numFrames);
	void SetPosition(sf::Vector2f pos);

	sf::Sprite* GetSpr();
	sf::Vector2f GetFrameSize();
	bool PlayedNumTimes(int val);
	bool playedOnce();
private:
	bool m_symmetrical;

	int animEnd;
	int m_columns, m_rows;
	int m_currentFrame, m_maxFrame;
	int m_currentAnim, m_maxAnim;
	std::vector<int> m_numFrames;

	float m_framesPerSecond, m_animSpeed;
	float m_currentTime, m_frameTime;

	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2f m_frameSize;
};