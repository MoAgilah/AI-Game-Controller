#include "CParams.h"
#include "../Game/Constants.h"

double CParams::dPi = 0;//
double CParams::dHalfPi = 0;//
double CParams::dTwoPi = 0;//
int CParams::WindowWidth = 600;//
int CParams::WindowHeight = 600;//
int CParams::iFramesPerSecond = (int)FPS;//
int CParams::iNumInputs = 0;//
int CParams::iNumOutputs = 0;//
double CParams::dBias = -1;//
int CParams::iNumSensors = 16;//
int CParams::iNumPlayers = 300;//
int CParams::iNumTicks = 2000;//needs doing
double CParams::dSigmoidResponse = 1;//
int CParams::iNumAddLinkAttempts = 5;
int CParams::iNumTrysToFindLoopedLink = 5;//
int CParams::iNumTrysToFindOldLink = 5;//
double CParams::dYoungFitnessBonus = 1.3;//
int CParams::iYoungBonusAgeThreshhold = 10;//
double CParams::dSurvivalRate = 0.2;//
int CParams::iNumGensAllowedNoImprovement = 15;//
int CParams::iMaxPermittedNeurons = 1000000;//
double CParams::dChanceAddLink = 2.0;//new
double CParams::dChanceAddNode = 0.5;
double CParams::dChanceAddRecurrentLink = 0.05;//
double CParams::dMutationRate = 0.5;//
double CParams::dMaxWeightPerturbation = 0.9;//
double CParams::dProbabilityWeightReplaced = 0.4;//new

double CParams::dActivationMutationRate = 0.2;//new
double CParams::dMaxActivationPerturbation = 0.9;//new

double CParams::dCompatibilityThreshold = 0.26;
int CParams::iOldAgeThreshold = 50;
double CParams::dOldAgePenalty = 0.7;
double CParams::dCrossoverRate = 0.75;//









