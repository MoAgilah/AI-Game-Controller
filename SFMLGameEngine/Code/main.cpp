#include <SFML/Graphics.hpp>
#include "Game/Constants.h"
#include "Game/GameManager.h"

int main()
{
	float t = 0.0f;
	float dt = 1.f / GameConstants::FPS;
	sf::RenderWindow window(sf::VideoMode((int)GameConstants::ScreenDim.x, (int)GameConstants::ScreenDim.y), "Super Mario World: Yoshi's Island 1");
	window.setFramerateLimit((unsigned int)GameConstants::FPS);

	CParams g_params;
	GameManager gameMgr;

	if (GameConstants::Automated)
	{
		gameMgr.GetLogger().AddDebugLog("Current Generation: " + std::to_string(gameMgr.GetAIController()->GetCurrentGeneration()));
		gameMgr.GetLogger().AddExperimentLog("Current Generation: " + std::to_string(gameMgr.GetAIController()->GetCurrentGeneration()));
		gameMgr.GetLogger().AddExperimentLog(gameMgr.GetLogger().GetTimeStamp());
	}

	sf::Clock clock;
	sf::Event event;
	float currentTime = clock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}
				else
				{
					if (!GameConstants::KeyPressed)
					{
						GameConstants::KeyPressed = true;
					}
				}
			}
		}

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		if (GameConstants::GameIsReady)
		{
			gameMgr.GetAIController()->GetAnnView()->Update();
		}

		window.clear(sf::Color::White);

		while (frameTime > 0.0)
		{
			float deltaTime = std::min(frameTime, dt);

			gameMgr.Update(deltaTime);

			frameTime -= deltaTime;
			t += deltaTime;
		}

		gameMgr.Render(window);

		window.display();
	}

	return 0;
}