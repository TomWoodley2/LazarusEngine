#pragma once
#include "..\..\..\Recast\Include\Recast.h"

class NavMeshBuild
{
public:
	NavMeshBuild();
	
	void BuildrcHeightField();
	void BuildrcCompactHeightField();
	void BuildrcContourSet();
	void BuildrcPolyMesh();
	void BuildrcPolyMeshDetail();

protected:
	rcContext * ctx;
	rcHeightfield * Heightfield;
	

};