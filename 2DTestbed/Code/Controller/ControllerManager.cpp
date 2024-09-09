#include "../Controller/ControllerManager.h"

ControllerManager* ControllerManager::m_instance = nullptr;

ControllerManager::ControllerManager()
{
	m_instance = this;
	m_controller = std::make_unique<Controller>();
}

ControllerManager::~ControllerManager()
{
	if (m_instance)
		m_instance = nullptr;
}
