#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <iostream>
using namespace std;

#include "Vector.h"
#include "Global.h"
#include "Scene_Parser.h"

std::string NEW_LINE_CHAR("\n");

bool findMinMaxCoor(const SceneParser *p, Vec3f & minCoor, Vec3f & maxCoor)
{
	minCoor.Set( FLT_MAX, FLT_MAX, FLT_MAX ), maxCoor.Set( (-1.0f) * FLT_MAX, (-1.0f) * FLT_MAX, (-1.0f) * FLT_MAX );
	Vec3f min_tmp, max_tmp;

	const Group * group = p->getGroup();
	
	for (int i = 0; i < group -> getObjectNumber(); ++i) 
	{
		const Object3D * obj = group -> getObjectOfIndex(i);
		obj->calcBoundingBox(min_tmp, max_tmp);

		for (int j = 0; j < 3; ++j) {
			if (minCoor[j] > min_tmp[j]) minCoor.Set(min_tmp[j], j);
			if (maxCoor[j] < max_tmp[j]) maxCoor.Set(max_tmp[j], j);
		}
	}

	return true;
}
