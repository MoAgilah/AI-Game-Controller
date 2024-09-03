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
	case TexID::Super:			return "Super";
	case TexID::Koopa:			return "Koopa";
	case TexID::Bill:			return "Bill";
	case TexID::Rex:			return "Rex";
	case TexID::PPlant:			return "PPlant";
	case TexID::Chuck:			return "Chuck";
	case TexID::Coin:			return "Coin";
	case TexID::YCoin:			return "YCoin";
	case TexID::ChkPnt:			return "ChkPnt";
	case TexID::Shroom:			return "Shroom";
	case TexID::Goal:			return "Goal";
	case TexID::QBox:			return "QBox";
	case TexID::SBox:			return "SBox";
	case TexID::BreakingBox:	return "BreakingBox";
	case TexID::Pipe1:			return "Pipe1";
	case TexID::Pipe2:			return "Pipe2";
	case TexID::Pipe3:			return "Pipe3";
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
	auto it = m_textureMap.find(id);
	if (it == m_textureMap.end())
	{
		throw std::invalid_argument(std::format("Texture {} not recognised", (int)id));
		return sf::Texture();
	}

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
