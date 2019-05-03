#pragma once
#include "Node.h"
#include "NavMesh/NavMesh.h"


class wander {
	
public:
	wander();
	~wander();
	void setStart();
	void setEnd();
	void move();

	void RenderNav();

protected:
	bool m_isStartSet = false;
	bool m_isEndSet = false;
	
	NavMesh m_NavigationMesh;
};