#ifndef CtrlMgrH
#define CtrlMgrH

#include <vector>
#include <SFML\Graphics.hpp>
#include "../Utils.h"

#include "../Controller/Controller.h"

class CtrlMgr
{
public:
	~CtrlMgr();
	static CtrlMgr* GetCtrlMgr();
	Controller* GetController();
private:
	CtrlMgr();
	static CtrlMgr* m_instance;
	Controller* m_controller;

};

#endif
