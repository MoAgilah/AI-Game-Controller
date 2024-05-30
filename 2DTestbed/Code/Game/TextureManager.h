#pragma once

#include <memory>
#include <map>

#include <SFML/Graphics.hpp>

enum class TexID
{
	Background,
	Mario, MarioBB, MarioSmlBB, Super, SuperBB, SuperSmlBB,
	Koopa, KoopaBB, Bill, BillBB, Rex, RexBB, RexSmlBB, PPlant, PPlantBB, Chuck, ChuckBB,
	YCoin, YcoinBB, QBox, SBox, Box, BoxBB, Shroom, ShroomBB, Pipe1, Pipe2, Pipe3, ChkPnt, ChkPntBB, Goal, GoalBB,
	Name, Time, Max
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