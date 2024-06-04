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
constexpr float RightMost = 11350.f;

constexpr float FPS = 60.0f;

constexpr int MAXKEYS = 6;

constexpr float c_maxAirTime = 1.f;
constexpr float m_moveSpeed = 3.0f;
constexpr float m_jumpSpeed = 3.0f;
