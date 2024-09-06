#include "Sprite.h"
#include <format>
#include <iostream>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"

Sprite::Sprite(TexID id)
{
	SetTexture(id);
}

void Sprite::SetTexture(TexID id)
{
	m_texID = id;
	try
	{
		m_sprite.setTexture(GameManager::GetGameMgr()->GetTextureMgr()->GetTexture(m_texID), true);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << std::endl;
	}

	SetOrigin(sf::Vector2f((float)m_sprite.getTexture()->getSize().x * 0.5f, (float)m_sprite.getTexture()->getSize().y * 0.5f));
}

void Sprite::SetFrameSize(const sf::Vector2u& size, int currentFrame, int currentAnim)
{
	m_frameSize = size;

	//set first frame to display
	SetTextureRect(sf::IntRect(currentFrame * m_frameSize.x, currentAnim * m_frameSize.y, m_frameSize.x, m_frameSize.y));
	SetScale(sf::Vector2f(scale));
	SetOrigin(sf::Vector2f((float)m_frameSize.x / 2.f, (float)m_frameSize.y / 2.f));
}

AnimatedSprite::AnimatedSprite(TexID id, int rows, int columns, float framesPerSec, bool symmetrical, float animSpeed)
	: Sprite(id), m_animSpeed(animSpeed), m_framesPerSecond(framesPerSec / 1000.0f), m_symmetrical(symmetrical)
{
	//set single frame size
	SetFrameSize(sf::Vector2u(GetTextureSize().x / columns, GetTextureSize().y / rows), m_frame.m_current, m_animation.m_current);
}

AnimatedSprite::AnimatedSprite(TexID id, float framesPerSec, bool symmetrical, float animSpeed)
	: Sprite(id), m_animSpeed(animSpeed), m_framesPerSecond(framesPerSec / 1000.0f), m_symmetrical(symmetrical)
{
}

void AnimatedSprite::Update(float dt)
{
	if (!m_loop)
	{
		if (m_frame.m_current == m_numFrames[m_animation.m_current])
			return;
	}

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
				if (m_loop)
					m_frame.m_current = 0;
				else
					--m_frame.m_current;
				++m_animCycles;
			}
		}

		//set new frame
		SetTextureRect(sf::IntRect(m_frame.m_current * GetFrameSize().x, m_animation.m_current * GetFrameSize().y, GetFrameSize().x, GetFrameSize().y));
	}
}

void  AnimatedSprite::ChangeAnim(int animNum)
{
	m_animCycles = 0;
	m_animation.m_current = animNum;
}

void AnimatedSprite::SetFrames(const std::vector<int>& numFrames)
{
	m_numFrames.assign(numFrames.begin(), numFrames.end());
	m_animation.m_max = m_numFrames[m_animation.m_current];
}

void AnimatedSprite::SetFrameData(int rows, int columns, const std::vector<int>& numFrames)
{
	//set single frame size
	SetFrameSize(sf::Vector2u(GetTextureSize().x / columns, GetTextureSize().y / rows), m_frame.m_current, m_animation.m_current);

	m_numFrames = numFrames;
	m_animation.m_max = m_numFrames[m_animation.m_current];
}

void AnimatedSprite::UpdateAnimSpeed(float animSpd)
{
	if (m_animSpeed != animSpd)
		m_animSpeed = animSpd;
}

