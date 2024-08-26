#include "PhysicsController.h"
#include "Constants.h"



PhysicsController::PhysicsController()
	: m_currX(XVelocity::walking), m_currY(YVelocity::jumping), m_currType(PhysicsType::ground)
{
	m_groundVelocities.push_back({ 0, 3.f * scale.x });
	m_groundVelocities.push_back({ 0, 1.5f * scale.x });
	m_groundVelocities.push_back({ m_groundVelocities[XVelocity::running].max, 3.f * scale.x });

	m_slopedVelocities.push_back({ 0, 0.8125f * scale.x });
	m_slopedVelocities.push_back({ 0, 0.8125f * scale.x });
	m_slopedVelocities.push_back({ m_slopedVelocities[XVelocity::running].max, 1.9375f * scale.x });

	m_aerialVelocities = { 3.4375f * scale.y, 3.6875f * scale.y, 3.9375f * scale.y, 4.3125f * scale.y };
	m_maxVelocity = { m_groundVelocities[m_currX], m_aerialVelocities[m_currY] };

	m_groundAcceleration = 0.0546875f;
	m_slopedAccelerations = { 0.125f, 0.1875f, 0.25f };
	m_aerialAccelerations = { 0.0625f, 0.325f };
}

void PhysicsController::Update(const Point& currVelocity)
{
	if (m_currX != XVelocity::walking)
	{
		if (currVelocity.x > 0)
		{
			if (currVelocity.x >= m_maxVelocity.first.max)
			{
				SetSprinting();
			}
		}
		else if (currVelocity.x < 0)
		{
			if (currVelocity.x <= -m_maxVelocity.first.max)
			{
				SetSprinting();
			}
		}
		else
		{
			SetWalking();
		}
	}

	/*if (currVelocity.x >= m_maxVelocity.first.max)
	{
		switch (m_currType)
		{
		case ground:
			if (m_currX < XVelocity::sprinting)
				m_maxVelocity.first = m_groundVelocities[++m_currX];
			break;
		case slope:
			if (m_currX < XVelocity::sprinting)
			{
				m_maxVelocity.first = m_groundVelocities[++m_currX];
				m_currAccelerations.x = m_slopedAccelerations[m_currX];
			}
			break;
		}
	}
	else if (currVelocity.x <= m_maxVelocity.first.min || currVelocity.x <= -m_maxVelocity.first.min)
	{
		switch (m_currType)
		{
		case ground:
			if (m_currX > XVelocity::walking)
				m_maxVelocity.first = m_groundVelocities[--m_currX];
			break;
		case slope:
			if (m_currX > XVelocity::walking)
			{
				m_maxVelocity.first = m_groundVelocities[--m_currX];
				m_currAccelerations.x = m_slopedAccelerations[m_currX];
			}
			break;
		}
	}*/
}

void PhysicsController::SetWalking()
{
	m_currType = PhysicsType::ground;
	m_currAccelerations.x = m_groundAcceleration;
	m_currX = XVelocity::walking;
	m_maxVelocity.first = m_groundVelocities[m_currX];
}

void PhysicsController::SetRunning()
{
	m_currType = PhysicsType::ground;
	m_currAccelerations.x = m_groundAcceleration;
	m_currX = XVelocity::running;
	m_maxVelocity.first = m_groundVelocities[m_currX];
}

void PhysicsController::SetSprinting()
{
	m_currType = PhysicsType::ground;
	m_currAccelerations.x = m_groundAcceleration;
	m_currX = XVelocity::sprinting;
	m_maxVelocity.first = m_groundVelocities[m_currX];
}

void PhysicsController::SetOnSlope()
{
	m_currType = PhysicsType::slope;
	m_currAccelerations.x = m_slopedAccelerations[m_currX];
	m_maxVelocity.first = m_slopedVelocities[m_currX];
}

void PhysicsController::SetAerial()
{
	m_currType = PhysicsType::rise;
	m_maxVelocity.second = m_aerialVelocities[m_currX];
	m_currY = YVelocity::jumping;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
}

void PhysicsController::SetFalling()
{
	m_currType = PhysicsType::drop;
	m_maxVelocity.second = m_aerialVelocities[m_currType];
	m_currY = YVelocity::falling;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
}
