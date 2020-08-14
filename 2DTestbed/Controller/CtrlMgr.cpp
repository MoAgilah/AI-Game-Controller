#include "../Controller/CtrlMgr.h"
#include "../Controller/Controller.h"
#include "../Controller/ANNView.h"

CtrlMgr* CtrlMgr::m_instance = nullptr;

CtrlMgr::CtrlMgr()
{
	m_controller = new Controller();
}

CtrlMgr::~CtrlMgr()
{
}

CtrlMgr * CtrlMgr::GetCtrlMgr()
{
	if (m_instance == nullptr)
	{
		m_instance = new CtrlMgr();
	}

	return m_instance;
}

Controller * CtrlMgr::GetController()
{
	return m_controller;
}
