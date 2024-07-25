#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

enum class TexID
{
	None = -1,
	Background,
	Mario, Super,
	Koopa, Bill, Rex, PPlant, Chuck,
	Coin, YCoin, ChkPnt, Shroom, Goal, QBox, SBox, BreakingBox,
	Pipe1, Pipe2, Pipe3, Name, Time, Max,
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager() = default;

	const sf::Texture& GetTexture(TexID id);

private:
	void AddTextures();

	std::map<TexID, sf::Texture> m_textureMap;
};