#pragma once

#include <SFML/Graphics.hpp>

class FontManager
{
public:
	FontManager();
	~FontManager() = default;

	const sf::Font& GetMenuFont() { return m_menuFont; }
	const sf::Font& GetStandardFont() { return m_standardFont; }

private:

	sf::Font m_menuFont;
	sf::Font m_standardFont;
};