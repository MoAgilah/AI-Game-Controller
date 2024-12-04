#pragma once

#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

enum class ShaderID
{
	None = -1,
	Flash,
	FadeInOut,
	Max,
};

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager() = default;

	sf::Shader* GetShader(ShaderID id);

private:

	void AddShaders();

	std::map<ShaderID, std::unique_ptr<sf::Shader>> m_ShaderMap;
};