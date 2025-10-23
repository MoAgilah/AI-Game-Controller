#include "CParams.h"
#include "../Game/Constants.h"

int CParams::WindowWidth = 600;
int CParams::WindowHeight = 600;
int CParams::iFramesPerSecond = (int)GameConstants::FPS;
int CParams::iNumInputs = 255;
int CParams::iNumOutputs = 7;
int CParams::iNumPlayers = 300;
int CParams::iNumTicks = 60;
int CParams::iNumAddLinkAttempts = 5;
int CParams::iNumTrysToFindLoopedLink = 5;
int CParams::iNumTrysToFindOldLink = 5;
int CParams::iYoungBonusAgeThreshhold = 10;
int CParams::iNumGensAllowedNoImprovement = 15;
int CParams::iMaxPermittedNeurons = 1000000;
int CParams::iOldAgeThreshold = 50;
double CParams::dPi = 3.14159265359;
double CParams::dHalfPi = 1.57079632679;
double CParams::dTwoPi = 6.28318530718;
double CParams::dBias = -1;
double CParams::dSigmoidResponse = 1;
double CParams::dYoungFitnessBonus = 1.3;
double CParams::dSurvivalRate = 0.2;
double CParams::dChanceAddLink = 0.3;
double CParams::dChanceAddNode = 0.1;
double CParams::dChanceAddRecurrentLink = 0.05;
double CParams::dMutationRate = 0.5;
double CParams::dMaxWeightPerturbation = 0.9;
double CParams::dProbabilityWeightReplaced = 0.4;
double CParams::dActivationMutationRate = 0.2;
double CParams::dMaxActivationPerturbation = 0.9;
double CParams::dCompatibilityThreshold = 0.26;
double CParams::dOldAgePenalty = 0.7;
double CParams::dCrossoverRate = 0.75;









