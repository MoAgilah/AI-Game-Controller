#pragma once

#include <utility>
#include <vector>
#include "../Utilities/Point.h"

enum XVelocity
{
	walking, running, sprinting
};

enum YVelocity
{
	jumping, falling
};

enum PhysicsType
{
	ground, slope, rise, drop
};

struct Velocity
{
	float min;
	float max;
};

class PhysicsController
{
public:
	PhysicsController();
	~PhysicsController() = default;

	void Update(bool direction, const Point& currVelocity);

	void SetWalking();
	void SetRunning();
	void SetSprinting();
	void SetOnSlope();
	void SetAerial();
	void SetFalling();

	PhysicsType GetPhysicsType() const { return (PhysicsType)m_currType; }
	XVelocity GetXVelocityType() const { return (XVelocity)m_currX; }

	float GetXAcceleration() const { return m_currAccelerations.x; }
	float GetYAcceleration() const { return m_currAccelerations.y; }

	float GetMaxXVelocity() const { return m_maxVelocity.first.max; }
	float GetMaxYVelocity() const { return m_maxVelocity.second; }

	float GetAirTimer() const { return m_airTime; }
private:

	int m_currX;
	int m_currY;
	PhysicsType m_currType;
	std::pair<Velocity, float> m_maxVelocity;
	Point m_currAccelerations;
	float m_groundAcceleration;
	float m_airTime;
	std::vector<Velocity> m_groundVelocities;
	std::vector<Velocity> m_slopedVelocities;
	std::vector<float> m_aerialVelocities;
	std::vector<float> m_slopedAccelerations;
	std::vector<float> m_aerialAccelerations;
	std::vector<float> m_aerialTimer;
};