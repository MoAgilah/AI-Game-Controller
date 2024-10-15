#include "PhysicsController.h"
#include<format>
#include<iostream>
#include "Constants.h"

PhysicsController::PhysicsController()
	: m_currX(XVelocity::walking), m_currY(YVelocity::jumping), m_currType(PhysicsType::ground)
{
	m_groundVelocities.push_back({ 0, 2.f * GameConstants::Scale.x });
	m_groundVelocities.push_back({ 0, 1.5f * GameConstants::Scale.x });
	m_groundVelocities.push_back({ m_groundVelocities[XVelocity::running].max, 3.f * GameConstants::Scale.x });

	m_slopedVelocities.push_back({ 0, 1.25f * GameConstants::Scale.x });
	m_slopedVelocities.push_back({ 0, 1.f * GameConstants::Scale.x });
	m_slopedVelocities.push_back({ m_slopedVelocities[XVelocity::running].max, 2.f * GameConstants::Scale.x });

	m_aerialVelocities = { 2.f * GameConstants::Scale.y, 3.5f * GameConstants::Scale.y, 4.f * GameConstants::Scale.y, 4.5f * GameConstants::Scale.y };
	m_aerialTimer = { 0.35f, 0.25f,0.25f };
	m_maxVelocity = { m_groundVelocities[m_currX], m_aerialVelocities[m_currY] };

	m_groundAcceleration = 0.0546875f;
	m_slopedAccelerations = { 0.125f, 0.1875f, 0.25f };
	m_aerialAccelerations = { 0.0625f, 0.03125f };

	m_currAccelerations.x = m_groundAcceleration;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
}

void PhysicsController::Update(bool direction, const Point& currVelocity)
{
	if ((m_currType < PhysicsType::rise) && (m_currX > XVelocity::walking))
	{
		if (direction)
		{
			if ((currVelocity.x >= m_maxVelocity.first.max) && (m_currX < XVelocity::sprinting))
			{
				m_maxVelocity.first = m_groundVelocities[++m_currX];
			}
			else if ((currVelocity.x <= m_maxVelocity.first.min) && (m_currX > XVelocity::walking))
			{
				m_maxVelocity.first = m_groundVelocities[--m_currX];
			}
		}
		else
		{
			if ((currVelocity.x <= -m_maxVelocity.first.max) && (m_currX < XVelocity::sprinting))
			{
				m_maxVelocity.first = m_groundVelocities[++m_currX];
			}
			else if ((currVelocity.x >= -m_maxVelocity.first.min) && (m_currX > XVelocity::walking))
			{
				m_maxVelocity.first = m_groundVelocities[--m_currX];
			}
		}
	}

	if (m_currType == PhysicsType::rise)
	{
		if (direction)
		{
			if ((currVelocity.y >= m_maxVelocity.first.max) && (m_currX < XVelocity::sprinting))
			{
				++m_currX;
				m_currAccelerations.y = m_aerialAccelerations[YVelocity::falling];
			}
			else if ((currVelocity.y <= m_maxVelocity.first.min) && (m_currX > XVelocity::walking))
			{
				if (--m_currX == XVelocity::walking)
					m_currAccelerations.y = m_aerialAccelerations[YVelocity::jumping];
			}
		}
		else
		{
			if ((currVelocity.y <= -m_maxVelocity.first.max) && (m_currX < XVelocity::sprinting))
			{
				++m_currX;
				m_currAccelerations.y = m_aerialAccelerations[YVelocity::falling];
			}
			else if ((currVelocity.y >= -m_maxVelocity.first.min) && (m_currX > XVelocity::walking))
			{
				if (--m_currX == XVelocity::walking)
					m_currAccelerations.y = m_aerialAccelerations[YVelocity::jumping];
			}
		}
	}
}

void PhysicsController::SetWalking()
{
	m_currType = PhysicsType::ground;
	m_currAccelerations.x = m_groundAcceleration;
	m_currX = XVelocity::walking;
	m_maxVelocity.first = m_groundVelocities[m_currX];
	m_currY = YVelocity::jumping;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
}

void PhysicsController::SetRunning()
{
	m_currType = PhysicsType::ground;
	m_currAccelerations.x = m_groundAcceleration;
	m_currX = XVelocity::running;
	m_maxVelocity.first = m_groundVelocities[m_currX];
	m_currY = YVelocity::jumping;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
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
	m_airTime = m_aerialTimer[m_currX];
}

void PhysicsController::SetFalling()
{
	m_currType = PhysicsType::drop;
	m_maxVelocity.second = m_aerialVelocities[m_currType];
	m_currY = YVelocity::falling;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
}
