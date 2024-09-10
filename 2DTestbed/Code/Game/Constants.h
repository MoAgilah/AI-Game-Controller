#pragma once
#include "../Utilities/Point.h"

class GameConstants
{
public:

	static bool Automated;
	static bool DControl;		// shows debug window for the controller
	static bool DRender;		// shows if should render debug elements
	static float RightMost;
	static float FPS;
	static float Gravity;
	static float MaxAirTime;
	static float GroundSpeed;
	static float AerialSpeed;
	static Point ScreenDim;
	static Point Scale;
};