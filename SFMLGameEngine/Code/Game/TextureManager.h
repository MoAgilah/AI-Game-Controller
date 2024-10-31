#pragma once

#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

enum class TexID
{
	None = -1,
	Title, Background,
	Mario, Super,
	Koopa, Bill, Rex, PPlant, Chuck,
	Shroom, Goal, Coin, YCoin, ChkPnt, QBox, SBox, BreakingBox,
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