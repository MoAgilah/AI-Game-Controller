#include "Sprite.h"
#include <format>
#include <iostream>

Sprite::Sprite(std::string_view filePath)
{
	if (!m_texture.loadFromFile(std::format("Sprites/{}", filePath)))
		std::cout << std::format("failed to load Sprites/{}", filePath) << std::endl;

	m_texture.setSmooth(true);
	m_sprite.setTexture(m_texture);
}

AnimatedSprite::AnimatedSprite(std::string_view filePath, int rows, int columns, float framesPerSec, bool symmetrical, int initialAnim, float animSpeed)
	: Sprite(filePath), m_animSpeed(animSpeed), m_framesPerSecond(framesPerSec / 1000.0f), m_symmetrical(symmetrical)
{
	ChangeAnim(initialAnim);

	//set single frame size
	m_frameSize = sf::Vector2u(GetTextureSize().x / columns, GetTextureSize().y / rows);

	//set first frame to display
	SetTextureRect(sf::IntRect(m_frame.m_current * m_frameSize.x, m_animation.m_current * m_frameSize.y, m_frameSize.x, m_frameSize.y));
	SetScale(sf::Vector2f(sX, sY));
	SetOrigin(sf::Vector2f((float)m_frameSize.x / 2.f, (float)m_frameSize.y  / 2.f));
}

void AnimatedSprite::Update(float dt, bool direction)
{
	m_currentTime += m_animSpeed * dt;

	if (m_currentTime >= m_framesPerSecond)
	{
		m_currentTime = 0.0f;

		++m_frame.m_current;

		if (m_symmetrical)
		{
			if (m_frame.m_current >= m_frame.m_max)
			{
				m_frame.m_current = 0;
			}
		}
		else
		{
			if (m_frame.m_current >= m_numFrames[m_animation.m_current])
			{
				m_frame.m_current = 0;
				++m_animEnd;
			}
		}

		//set new frame
		SetTextureRect(sf::IntRect(m_frame.m_current * m_frameSize.x, m_animation.m_current * m_frameSize.y, m_frameSize.x, m_frameSize.y));

		//if going left flip animation
		if (direction == false)
		{
			// flip X
			SetScale({ -sX, sY });
		}
		else
		{
			//unflip x
			SetScale({ sX, sY });
		}

	}
}

void  AnimatedSprite::ChangeAnim(int animNum)
{
	m_animEnd = 0;
	m_animation.m_current = animNum;
}

void AnimatedSprite::SetFrames(std::span<int> numFrames)
{
	m_numFrames.assign(numFrames.begin(), numFrames.end());
	m_animation.m_max = m_numFrames[m_animation.m_current];
}


