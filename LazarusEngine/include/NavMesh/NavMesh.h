#pragma once
//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>

//#include "..\SDL\SDL.h"
//#include "..\SDL\SDL_opengl.h"
//#ifdef __APPLE__
//#	include <OpenGL/glu.h>
//#else
//#	include <GL/glu.h>
//#endif

#include <vector>
#include <string>

#include "imgui.h"
//#include "imguiRenderGL.h"

#include "Recast.h"
#include "RecastDebugDraw.h"
#include "InputGeom.h"
#include "TestCase.h"
//#include "Filelist.h"
#include "Sample_SoloMesh.h"
#include "Sample_TileMesh.h"
#include "Sample_TempObstacles.h"
#include "Sample_Debug.h"
#include "NavMeshTesterTool.h"

#ifdef WIN32
#	define snprintf _snprintf
#	define putenv _putenv
#endif

	using std::string;
	using std::vector;

	

class NavMesh
{
public:
	NavMesh();

	//Recast Stuff
	void NavMeshMain();
	void loadNavMesh();
	void BuildNavMesh();

	//Detour Stuff
	void initNav();
	void HandleNavToggle();
	void HandleNavRender();

	void NavSetRandomStart();
	void NavSetRandomEnd();
	void NavSetRandomTarget();
	void NavSetWalkFlags();
	void NavSetToolFollow();

	/*Sample* createSolo() { return new Sample_SoloMesh(); }
	Sample* createTile() { return new Sample_TileMesh(); }
	Sample* createTempObstacle() { return new Sample_TempObstacles(); }
	Sample* createDebug() { return new Sample_Debug(); }*/

protected:

	NavMeshTesterTool* DTNav = 0;

	const string pathObj = "assets/Mesh/TestLevel.obj";
	const string pathSet = "assets/Mesh/TestLevel.gset";

	float rayStart[3];
	bool processHitTestShift = false;

	BuildContext ctx;
	BuildSettings m_settings;
	InputGeom* geom = 0;
	Sample* sample = 0;
	TestCase* test = 0;
};