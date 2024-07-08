#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

enum class TexID
{
	None = -1,
	Background,
	Mario, MarioBB, MarioSmlBB, Super, SuperBB,
	Koopa, KoopaBB, Bill, BillBB, Rex, RexBB, RexSmlBB, PPlant, PPlantBB, Chuck, ChuckBB,
	Coin, CoinBB, YCoin, YcoinBB, ChkPnt, ChkPntBB, Shroom, ShroomBB,  Goal, GoalBB, QBox, SBox, BreakingBox, BoxBB,
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