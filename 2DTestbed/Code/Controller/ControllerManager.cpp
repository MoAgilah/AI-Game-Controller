#include "../Controller/ControllerManager.h"

std::unique_ptr<ControllerManager> ControllerManager::m_instance = nullptr;

ControllerManager::ControllerManager()
{
	m_instance.reset(this);
	m_controller = std::make_unique<Controller>();
}
