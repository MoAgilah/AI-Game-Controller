#include <iostream>
#include <SFML/Graphics.hpp>
#include "NEAT/CParams.h"
#include "Controller/ANNView.h"
#include "Controller/ControllerManager.h"
#include "Game/Constants.h"
#include "Game/GameManager.h"
#include "GameObjects/Player.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode((int)screenDim.x, (int)screenDim.y), "SFML works!");
	window.setFramerateLimit((unsigned int)FPS);
	float t = 0.0f;
	float dt = 1.f / FPS;

	GameManager gameMgr;
	gameMgr.Update(dt);
	ControllerManager ctrlMgr;

	//initialise begin
	CParams g_params;

	gameMgr.GetLogger()->AddDebugLog("Current Generation: " + std::to_string(ctrlMgr.GetController()->GetCurrentGeneration()));
	gameMgr.GetLogger()->AddExperimentLog("Current Generation: " + std::to_string(ctrlMgr.GetController()->GetCurrentGeneration()));
	gameMgr.GetLogger()->AddExperimentLog(GameManager::GetGameMgr()->GetLogger()->GetTimeStamp());

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

		ctrlMgr.GetController()->GetAnnView()->Update();

		window.clear(sf::Color::White);
		while (frameTime > 0.0)
		{
			float deltaTime = std::min(frameTime, dt);
			//do update

			ctrlMgr.GetController()->GetGridInputs();

			gameMgr.Update(deltaTime);

			//end update
			frameTime -= deltaTime;
			t += deltaTime;
		}

		//do render
		gameMgr.Render(window);
#ifdef DControl
		ctrlMgr.GetController()->GetAnnView()->Render(window);
#endif // DControl

		//end render
		window.display();
	}

	return 0;
}