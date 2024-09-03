#include "Box.h"
#include "../Game/GameManager.h"

Box::Box(AnimatedSprite* sprite, const sf::Vector2f& initPos)
	: Object(sprite, sf::Vector2f(16,16))
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
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
	: Box(new AnimatedSprite(TexID::QBox, 2, 4, FPS, false, 0.35f), initPos)
{
	auto animSpr = GetAnimSpr();
	animSpr->SetFrames({ 1, 4 });
	animSpr->ChangeAnim(QBoxAnims::ROTATE);
}

void QBox::Update(float deltaTime)
{
	if (GetActive())
	{
		auto animSpr = GetAnimSpr();
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
	GetAnimSpr()->ChangeAnim(QBoxAnims::ROTATE);
	Box::Reset();
}

SBox::SBox(const sf::Vector2f& initPos)
	: Box(new AnimatedSprite(TexID::SBox, 2, 4, FPS, false, 0.35f), initPos)
{
	GetAnimSpr()->SetFrames({ 1, 4 });
}

void SBox::Update(float deltaTime)
{
	if (GetActive())
	{
		auto animSpr = GetAnimSpr();

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
	auto animSpr = GetAnimSpr();

	if (GetSprite()->GetTexID() != TexID::SBox)
	{
		animSpr->SetTexture(TexID::SBox);
		animSpr->SetFrameData(2, 4, { 1, 4 });
	}
	animSpr->ChangeAnim(SBoxAnims::WAIT);
	Box::Reset();
}

void SBox::Scatter()
{
	auto animSpr = GetAnimSpr();

	animSpr->Move(0, c_moveSpeed*0.75);

	if (GetPosition().y > 600.f + (float)animSpr->GetFrameSize().y)
	{
		SetActive(false);
	}

	if (animSpr->PlayedOnce())
	{
		animSpr->ChangeAnim(ShatterAnims::DROP);
		animSpr->SetShouldLoop(true);
	}
}
