#include "ShaderManager.h"
#include <utility>
#include <format>
#include <iostream>
#include <optional>

std::optional<std::pair<std::string,sf::Shader::Type>> ShaderIDToString(ShaderID id)
{
	switch (id)
	{
	case ShaderID::Flash:			return std::make_pair< std::string, sf::Shader::Type>("FlashShader.frag", sf::Shader::Fragment);
	case ShaderID::FadeInOut:		return std::make_pair< std::string, sf::Shader::Type>("FadeInOutShader.frag", sf::Shader::Fragment);
	default:						return std::nullopt;
	}
}

ShaderManager::ShaderManager()
{
	AddShaders();
}

sf::Shader* ShaderManager::GetShader(ShaderID id)
{
	auto it = m_ShaderMap.find(id);
	if (it == m_ShaderMap.end())
	{
		throw std::invalid_argument(std::format("shader id {} not recognised", (int)id));
		return nullptr;
	}

	return m_ShaderMap[id].get();
}

void ShaderManager::AddShaders()
{
	for (int i = 0; i < (int)ShaderID::Max; i++)
	{

		auto pair = ShaderIDToString((ShaderID)i);
		if (!pair.has_value())
		{
			std::cout << std::format("unidentified shader id {}", pair->first) << std::endl;
			continue;
		}

		sf::Shader* shader = new sf::Shader();
		if (!shader->loadFromFile(std::format("Resources/Shaders/{}", pair->first), pair->second))
		{
			std::cout << std::format("failed to load Resources/Shaders/{}", pair->first) << std::endl;
			continue;
		}

		m_ShaderMap.try_emplace((ShaderID)i, std::move(shader));
	}
}
