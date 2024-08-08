#pragma once
#include "../Utilities/Point.h"

//Dcontrol shows debug for the controller
#define DControl

//DRender shows debug for the testbed
//#define DRender
const Point screenDim(600, 600);
const Point scale(2.34375f, 2.5f);
const Point maxVelocity(4.5f, 4.5f);
const Point scaledMaxVel(maxVelocity.x * scale.x, maxVelocity.y * scale.y);

constexpr bool Automated = false;
constexpr float LeftMost = 10.f;
constexpr float RightMost = 11400.f;

constexpr float FPS = 60.0f;

const float c_gravity = 0.981f;
constexpr float c_maxAirTime = 1.f;
constexpr float c_moveSpeed = 3.0f;
constexpr float c_jumpSpeed = 3.0f;
