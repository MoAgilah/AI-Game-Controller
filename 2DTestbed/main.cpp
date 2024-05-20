#include <iostream>

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
	float t = 0.0f;
	float dt = 1.f / FPS;

	//initialise begin
	CParams g_params;
	CtrlMgr::GetCtrlMgr()->GetController();

	Game gameMgr;
	gameMgr.GetLogger()->AddDebugLog("Current Generation: " + std::to_string(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentGeneration()));
	gameMgr.GetLogger()->AddExperimentLog("Current Generation: " + std::to_string(CtrlMgr::GetCtrlMgr()->GetController()->GetCurrentGeneration()));
	gameMgr.GetLogger()->AddExperimentLog(Game::GetGameMgr()->GetLogger()->GetTimeStamp());

	//initialise end
	sf::Clock clock;
	sf::Event event;
	float currentTime = clock.getElapsedTime().asSeconds();
	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		CtrlMgr::GetCtrlMgr()->GetController()->GetAnnView()->Update();

		window.clear(sf::Color::White);
		while (frameTime > 0.0)
		{
			float deltaTime = std::min(frameTime, dt);
			//do update

			CtrlMgr::GetCtrlMgr()->GetController()->GetGridInputs();

			gameMgr.Update(deltaTime);

			//end update
			frameTime -= deltaTime;
			t += deltaTime;
		}

		//do render
		gameMgr.Render(window);
#ifdef DControl
		CtrlMgr::GetCtrlMgr()->GetController()->GetAnnView()->Render(window);
#endif // DControl

		//end render
		window.display();
	}

	return 0;
}