#include "FontManager.h"

FontManager::FontManager()
{
	m_menuFont.loadFromFile("Resources/Fonts/supermarioworld.ttf");
	m_standardFont.loadFromFile("Resources/Fonts/arial.ttf");
}
