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
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(1);
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
			animSpr->ChangeAnim(0);
			SetCanHit(false);
			SetJustHit(false);
		}
	}
}

void QBox::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(1);
	Box::Reset();
}

SBox::SBox(const sf::Vector2f& initPos)
	: Box(new AnimatedSprite(TexID::SBox, 2, 4, FPS, false, 0.35f), TexID::BoxBB, initPos)
{
	std::vector<int> frames{ 1, 4 };
	static_cast<AnimatedSprite*>(GetSprite())->SetFrames(frames);

	for (size_t i = 0; i < 4; i++)
	{
		m_boxPieces.push_back(std::make_shared<AnimatedSprite>(TexID::BoxPiece, 1, 8, FPS, false, 1.f));
		std::vector<int> frames{ 8 };
		m_boxPieces[i]->SetFrames(frames);
	}
}

void SBox::Update(float deltaTime)
{
	if (GetActive())
	{
		if (GetJustSmashed())
		{
			const Camera* camera = Game::GetGameMgr()->GetCamera();

			for (int i = 0; i < m_boxPieces.size(); i++)
			{
				if (camera->IsInView(m_boxPieces[i]->GetSprite()))
				{
					m_boxPieces[i]->Update(deltaTime);
					Scatter(i, m_boxPieces[i].get());
				}
			}
		}
		else
		{
			auto animSpr = static_cast<AnimatedSprite*>(GetSprite());

			if (!GetCanHit())
			{
				animSpr->Update(deltaTime);
				if (animSpr->PlayedNumTimes(2))
				{
					animSpr->ChangeAnim(0);
					SetCanHit(true);
				}
			}

			if (GetJustHit())
			{
				animSpr->ChangeAnim(1);
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
		for (const auto& piece : m_boxPieces)
			piece->Render(window);
	}
	else
	{
		Box::Render(window);
	}
}

void SBox::Reset()
{
	static_cast<AnimatedSprite*>(GetSprite())->ChangeAnim(0);
	Box::Reset();
}

void SBox::Scatter(int idx, AnimatedSprite* sprite)
{
}
