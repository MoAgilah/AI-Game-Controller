#include <iostream>

#include <format>
#include <SFML/Graphics.hpp>
#include "Utils.h"

#include "../2DTestbed/Game/Game.h"
#include "../2DTestbed/GameObjects/Player.h"
#include "../2DTestbed/Game/Camera.h"
#include "../2DTestbed/Controller/ANNView.h"
#include "../2DTestbed/Controller/CtrlMgr.h"
#include "../2DTestbed/NEAT/CParams.h"

int main()
{
	sf::Vector2i screenDimensions(scrX, scrY);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "SFML works!");
	window.setFramerateLimit((unsigned int)FPS);
	float dt = 1.f / FPS;

	//initialise begin
	CParams g_params;
	CtrlMgr::GetCtrlMgr()->GetController();

	Game::GetGameMgr()->GetLogger()->AddDebugLog(std::format("Current Generation: {}", CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentGeneration()));
	Game::GetGameMgr()->GetLogger()->AddExperimentLog(std::format("Current Generation: {}", CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentGeneration()));
	Game::GetGameMgr()->GetLogger()->AddExperimentLog(Game::GetGameMgr()->GetLogger()->GetTimeStamp());

	//initialise end
	sf::Clock clock;
	sf::Event event;
	float currentTime = clock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		Camera::GetCamera()->Reset(window);
		CtrlMgr::GetCtrlMgr()->GetController()->GetAnnView()->Update();

		window.clear(sf::Color::White);

		while (frameTime > 0.0)
		{
			float deltaTime = std::min(frameTime, dt);
			//do update
#ifdef GridInput
			CtrlMgr::GetCtrlMgr()->GetController()->GetGridInputs();
#else
			CtrlMgr::GetCtrlMgr()->GetController()->GetSensorInputs();
#endif
			Game::GetGameMgr()->Update(deltaTime);

			//end update
			frameTime -= deltaTime;
		}

		//do render
		Game::GetGameMgr()->Render(window);
#ifdef DControl
		CtrlMgr::GetCtrlMgr()->GetController()->GetAnnView()->Render(window);
#endif // DControl

		//end render
		window.display();
	}

	return 0;
}