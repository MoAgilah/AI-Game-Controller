#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../NEAT/Cga.h"
#include "../Controller/ANNView.h"
#include "../Game/Timer.h"

class Player;
class AutomatedPlayer;

class AIController
{
public:
	AIController();
	~AIController() = default;

	void InitAIController();
	bool Update(float deltaTime);

	Player* GetCurrentPlayer();

	const std::vector<double>& GetGridInputs();
	std::vector<double> GetFitnessScores() const;

	ANNView* GetAnnView() { return m_AnnView.get(); }
	int GetCurrentPlayerNum() const { return m_currPlayer; }
	int GetCurrentGeneration() const { return m_generations; }
	double BestFitness() const { return m_bestFitness; }

private:
	double ColourToInput(sf::Color col);
	void EndOfRunCalculation(AutomatedPlayer* ply, bool stalled);

	// --- motion/rotation bookkeeping for the current player ---
	float m_prevPosX = 0.f;     // last frame X for the *current* player
	float m_prevPosY = 0.f;     // last frame Y for the *current* player
	int   m_inactiveFrames = 0; // consecutive frames with no real movement

	// per-player stall flags (due to inactivity timeout)
	std::vector<bool> m_playerStalled;

	int m_currPlayer = 0;
	int m_generations = 0;
	double m_bestFitness = 0;

	Timer m_timer;
	std::unique_ptr<Cga> m_pop;
	std::vector<double> m_inputs;
	std::unique_ptr<ANNView> m_AnnView;
	std::vector<std::shared_ptr<AutomatedPlayer>> m_players;

	// =========================
	// Shaping trackers (per-player)
	// =========================
	std::vector<float> m_rightwardPixels;   // sum of positive dx this run
	std::vector<int>   m_crouchFrames;      // frames spent crouched

	std::vector<int>   m_superFrames;       // frames spent in Super state
	std::vector<bool>  m_startedSuper;      // whether player started Super at run begin
	std::vector<int>   m_framesBoxAhead;    // frames a "box" was reachable ahead/above
	std::vector<int>   m_framesTriedBox;    // frames we tried (jumped) while a box was reachable
};
