#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Controller/Controller.h"

class ControllerManager
{
public:
	ControllerManager();
	~ControllerManager() = default;

	static ControllerManager* GetCtrlMgr() { return m_instance.get(); }
	Controller* GetController() { return m_controller.get(); }

private:

	std::unique_ptr<Controller> m_controller;
	static std::unique_ptr<ControllerManager> m_instance;
};
