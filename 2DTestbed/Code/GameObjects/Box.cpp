#include "Box.h"
#include "../Game/Game.h"
#include "../Game/Camera.h"

Box::Box(AnimatedSprite* sprite, TexID boxId, const sf::Vector2f& initPos)
	: Object(sprite, boxId)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBBox()->Update(GetPosition());
}

void Box::Reset()
{
	m_canHit = true;
	m_justHit = false;
	Object::Reset();
}

void Box::WasJustHit()
{
	if (m_canHit)
		m_justHit = true;
}

QBox::QBox(const sf::Vector2f& initPos)
	: Box(new AnimatedSprite(TexID::QBox, 2, 4, FPS, false, 0.35f), TexID::BoxBB, initPos)
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(QBoxAnims::ROTATE);
	std::vector<int> frames{ 1, 4 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);
}

void QBox::Update(float deltaTime)
{
	if (GetActive())
	{
		auto animSpr = static_cast<AnimatedSprite*>(GetSprite());
		animSpr->Update(deltaTime);

		if (GetJustHit())
		{
			animSpr->ChangeAnim(QBoxAnims::DUD);
			SetCanHit(false);
			SetJustHit(false);
		}
	}
}

void QBox::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(QBoxAnims::ROTATE);
	Box::Reset();
}

void QBox::ResolveCollisions(Object* other)
{
}

void QBox::ResolveCollisions(Tile* tile)
{
}

SBox::SBox(const sf::Vector2f& initPos)
	: Box(new AnimatedSprite(TexID::SBox, 2, 4, FPS, false, 0.35f), TexID::BoxBB, initPos)
{
	std::vector<int> frames{ 1, 4 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);
}

void SBox::Update(float deltaTime)
{
	if (GetActive())
	{
		auto animSpr = static_cast<AnimatedSprite*>(GetSprite());

		if (GetJustSmashed())
		{
			if (GetSprite()->GetTexID() != TexID::BreakingBox)
			{
				animSpr->SetTexture(TexID::BreakingBox);
				animSpr->SetFrameData(2, 9, { 9, 9 });
				animSpr->SetScale(sf::Vector2f(1, 1));
				animSpr->ChangeAnim(ShatterAnims::SCATTER);
				animSpr->SetShouldLoop(false);
			}

			animSpr->Update(deltaTime);
			Scatter();
		}
		else
		{
			if (!GetCanHit())
			{
				animSpr->Update(deltaTime);
				if (animSpr->PlayedNumTimes(2))
				{
					animSpr->ChangeAnim(SBoxAnims::WAIT);
					SetCanHit(true);
				}
			}

			if (GetJustHit())
			{
				animSpr->ChangeAnim(SBoxAnims::SPIN);
				SetCanHit(false);
				SetJustHit(false);
			}
		}
	}
}

void SBox::Render(sf::RenderWindow& window)
{
	if (GetJustSmashed())
	{
		window.draw(*GetSprite()->GetSprite());
	}
	else
	{
		Box::Render(window);
	}
}

void SBox::Reset()
{
	auto animSpr = static_cast<AnimatedSprite*>(GetSprite());

	if (GetSprite()->GetTexID() != TexID::SBox)
	{
		animSpr->SetTexture(TexID::SBox);
		animSpr->SetFrameData(2, 4, { 1, 4 });
	}
	animSpr->ChangeAnim(SBoxAnims::WAIT);
	Box::Reset();
}

void SBox::ResolveCollisions(Object* other)
{
}

void SBox::ResolveCollisions(Tile* tile)
{
}

void SBox::Scatter()
{
	auto animSpr = static_cast<AnimatedSprite*>(GetSprite());

	animSpr->Move(0, c_moveSpeed*0.75);

	if (GetPosition().y > 600.f + (float)animSpr->GetFrameSize().y)
	{
		SetActive(false);
	}

	if (animSpr->PlayedOnce())
	{
		animSpr->ChangeAnim(ShatterAnims::FAll);
		animSpr->SetShouldLoop(true);
	}
}
