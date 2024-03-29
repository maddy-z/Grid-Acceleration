#include <cassert>
#include <cfloat>

#include "Object3D.h"
#include "Group.h"
#include "Vector.h"

class Object3D;

Group::Group(int num) : objs_num(num)
{
	if (num == 0) {
		objs = NULL;
		return;
	}

	assert (num > 0);
	objs = new Object3D * [num];

	return;
}

Group::~Group()
{
	for (int i = 0; i < objs_num; ++i) {
		delete objs[i];
	}

	if (objs != NULL)
		delete [] objs;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
	bool iFlag = false;

	for (int i = 0; i < objs_num; ++i) {
		if ( objs[i]->intersect(r, h, tmin) )
			iFlag = true;
	}

	return iFlag;
}

void Group::addObject(int index, Object3D *obj)
{
	assert (index >= 0 && index < objs_num);

	objs[index] = obj;
	return;
}


bool Group::calcBoundingBox(Vec3f & minCoor, Vec3f & maxCoor) const
{
	Vec3f min ( FLT_MAX, FLT_MAX, FLT_MAX ), max ( (-1.0f) * FLT_MAX, (-1.0f) * FLT_MAX, (-1.0f) * FLT_MAX );
	Vec3f min_tmp, max_tmp;

	for (int i = 0; i < objs_num; ++i) 
	{
		objs[i] -> calcBoundingBox(min_tmp, max_tmp);
	
		for (int j = 0; j < 3; ++j) 
		{
			if (min_tmp[j] < min[j]) min.Set(min_tmp[j], j);
			if (max_tmp[j] > max[j]) max.Set(max_tmp[j], j);
		}
	}

	minCoor = min;
	maxCoor = max;

	return true;
}

void Group::registIntoGrid(Grid & grid) const
{
	for (int i = 0; i < objs_num; ++i) {
		( (const Object3D *)(objs[i]) ) -> registIntoGrid(grid);
	}

	return;
}