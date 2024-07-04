#include "TextureManager.h"

#include <iostream>
#include <format>
#include <stdexcept>

std::string TexIDToString(TexID id)
{
	switch (id)
	{
	case TexID::Background:		return "Background";
	case TexID::Mario:			return "Mario";
	case TexID::MarioBB:		return "MarioBB";
	case TexID::MarioSmlBB:		return "MarioSmlBB";
	case TexID::Super:			return "Super";
	case TexID::SuperBB:		return "SuperBB";
	case TexID::Koopa:			return "Koopa";
	case TexID::KoopaBB:		return "KoopaBB";
	case TexID::Bill:			return "Bill";
	case TexID::BillBB:			return "BillBB";
	case TexID::Rex:			return "Rex";
	case TexID::RexBB:			return "RexBB";
	case TexID::RexSmlBB:		return "RexSmlBB";
	case TexID::PPlant:			return "PPlant";
	case TexID::PPlantBB:		return "PPlantBB";
	case TexID::Chuck:			return "Chuck";
	case TexID::ChuckBB:		return "ChuckBB";
	case TexID::Coin:			return "Coin";
	case TexID::CoinBB:			return "CoinBB";
	case TexID::YCoin:			return "YCoin";
	case TexID::YcoinBB:		return "YcoinBB";
	case TexID::QBox:			return "QBox";
	case TexID::SBox:			return "SBox";
	case TexID::BreakingBox:	return "BreakingBox";
	case TexID::BoxBB:			return "BoxBB";
	case TexID::Shroom:			return "Shroom";
	case TexID::ShroomBB:		return "ShroomBB";
	case TexID::Pipe1:			return "Pipe1";
	case TexID::Pipe2:			return "Pipe2";
	case TexID::Pipe3:			return "Pipe3";
	case TexID::ChkPnt:			return "ChkPnt";
	case TexID::ChkPntBB:		return "ChkPntBB";
	case TexID::Goal:			return "Goal";
	case TexID::GoalBB:			return "GoalBB";
	case TexID::Name:			return "Name";
	case TexID::Time:			return "Time";
	default: return "";
	}
}

TextureManager::TextureManager()
{
	AddTextures();
}

const sf::Texture& TextureManager::GetTexture(TexID id)
{
	if (auto it = m_textureMap.find(id); it == m_textureMap.end())
		throw std::invalid_argument(std::format("Texture {} not recognised", (int)id));

	return m_textureMap[id];
}

void TextureManager::AddTextures()
{
	for (int i = 0; i < (int)TexID::Max; i++)
	{
		sf::Texture texture;
		if (!texture.loadFromFile(std::format("Resources/Sprites/{}.png", TexIDToString((TexID)i))))
		{
			std::cout << std::format("failed to load Resources/Sprites/{}.png", TexIDToString((TexID)i)) << std::endl;
			continue;
		}

		texture.setSmooth(true);
		m_textureMap.try_emplace((TexID)i, texture);
	}
}
