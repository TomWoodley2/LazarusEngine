#include "wander.h"

wander::wander()
{
	//m_NavigationMesh = new NavMesh();
	m_NavigationMesh.loadNavMesh();
	m_NavigationMesh.BuildNavMesh();
	m_NavigationMesh.initNav();
	m_NavigationMesh.NavSetWalkFlags();
	m_NavigationMesh.NavSetToolFollow();
	
}

void wander::setStart()
{
	m_NavigationMesh.NavSetRandomStart();
	m_isStartSet = true;
}

void wander::setEnd()
{
	m_NavigationMesh.NavSetRandomEnd();
	m_isEndSet = true;
}

void wander::move()
{
	/*if (m_isStartSet == false)
	{
		setStart();
	}
	if (m_isEndSet == false)
	{
		setEnd();
	}*/

	m_NavigationMesh.NavSetRandomTarget();

	m_NavigationMesh.HandleNavToggle();
}

void wander::RenderNav()
{
	m_NavigationMesh.HandleNavRender();
}
