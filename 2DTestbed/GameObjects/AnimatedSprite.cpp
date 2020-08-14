#include "../GameObjects/AnimatedSprite.h"
  
AnimatedSprite::AnimatedSprite(std::string filePath, int rows, int columns, float framesPerSec, int id, bool symmetrical, int initialAnim, float animationSpeed)
{
	m_currentFrame = 0;
	m_currentAnim = initialAnim;
	animEnd = 0;
	m_currentTime = 0;
	m_framesPerSecond = framesPerSec / 1000.0f;
	m_animSpeed = animationSpeed;
	m_symmetrical = symmetrical;

	if (!m_texture.loadFromFile("Sprites/" + filePath))
	{
		std::cout << filePath << " failed to load " << std::endl;
	}

	m_texture.setSmooth(true);

	m_sprite.setTexture(m_texture);

	float width((float)(m_texture.getSize().x)), height((float)(m_texture.getSize().y));

	m_columns = columns;
	m_rows = rows;

	//set single frame size
	m_frameSize = sf::Vector2f(width / m_columns, height / m_rows);

	//set a max frame if symmetrical
	m_maxFrame = (int)(width / m_frameSize.x);

	//set first frame to display
	m_sprite.setTextureRect(sf::IntRect((int)(m_currentFrame * m_frameSize.x), (int)(m_currentAnim * m_frameSize.y), (int)m_frameSize.x, (int)m_frameSize.y));
	m_sprite.setScale(sX, sY);
	m_sprite.setOrigin(m_frameSize.x / 2.f, m_frameSize.y  / 2.f);
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::Update(float dt, bool direction)
{
	m_currentTime += m_animSpeed * dt;

	if (m_currentTime >= m_framesPerSecond)
	{
		m_currentTime = 0.0f;

		++m_currentFrame;

		if (m_symmetrical)
		{
			if (m_currentFrame >= m_maxFrame)
			{
				m_currentFrame = 0;
			}
		}
		else
		{
			if (m_currentFrame >= m_numFrames[m_currentAnim])
			{
				m_currentFrame = 0;
				++animEnd;
			}
		}

		//set new frame
		m_sprite.setTextureRect(sf::IntRect(int(m_currentFrame * m_frameSize.x), int(m_currentAnim * m_frameSize.y), int(m_frameSize.x), int(m_frameSize.y)));
		
		//if going left flip animation
		if (direction == false)
		{
			// flip X
			m_sprite.setScale({ -sX, sY });
		}
		else
		{
			//unflip x
			m_sprite.setScale({ sX, sY });
		}
		
	}
}

void AnimatedSprite::Render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void  AnimatedSprite::ChangeAnim(int animNum)
{
	animEnd = 0;
	m_currentAnim = animNum;
}

void AnimatedSprite::Move(float x, float y)
{ 
	m_sprite.move(x, y);
}

void AnimatedSprite::SetFrames(std::vector<int> numFrames)
{
	m_numFrames = numFrames;
}

void AnimatedSprite::SetPosition(sf::Vector2f pos)
{
	m_sprite.setPosition(pos);
}

sf::Sprite* AnimatedSprite::GetSpr() 
{ 
	return &m_sprite; 
}

sf::Vector2f AnimatedSprite::GetFrameSize() 
{
	return m_frameSize; 
}

bool AnimatedSprite::PlayedNumTimes(int val)
{
	return animEnd == val;
}

bool AnimatedSprite::playedOnce()
{
	if (animEnd > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


