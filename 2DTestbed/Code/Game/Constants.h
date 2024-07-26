#pragma once

//Dcontrol shows debug for the controller
#define DControl

//DRender shows debug for the testbed
//#define DRender

//scale factor
constexpr float sX = 2.34375f;
constexpr float sY = 2.5f;

//screen dimensions
constexpr int scrX = 600;
constexpr int scrY = 600;

constexpr bool Automated = false;
constexpr float LeftMost = 10.f;
constexpr float RightMost = 11400.f;

constexpr float FPS = 60.0f;

const float c_gravity = 0.981f;
constexpr float c_maxAirTime = 1.f;
constexpr float c_moveSpeed = 3.0f;
constexpr float c_jumpSpeed = 3.0f;
