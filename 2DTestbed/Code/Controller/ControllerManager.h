#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Controller/Controller.h"

class ControllerManager
{
public:
	ControllerManager();
	~ControllerManager();

	static ControllerManager* GetCtrlMgr() { return m_instance; }
	Controller* GetController() { return m_controller.get(); }

private:

	std::unique_ptr<Controller> m_controller;
	static ControllerManager* m_instance;
};
