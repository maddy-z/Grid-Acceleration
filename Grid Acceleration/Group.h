#ifndef _GROUP_H_
#define _GROUP_H_

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

#include "Vector.h"
#include "Matrix.h"

#include "Object3D.h"
#include "Sphere.h"

class Ray;
class Hit;
class Grid;

// ====================================================
// ====================================================

class Group : public Object3D

{

	// friend ostream & operator << (ostream &, const Group &);

public:

	Group(int num = 0);
	virtual ~Group();

	virtual bool intersect(const Ray & r, Hit & h, float tmin);
	void addObject(int index, Object3D * obj);

	const Object3D * getObjectOfIndex(int i) const { assert ( 0 <= i && i < objs_num ); return objs[i]; }
	int getObjectNumber() const { return objs_num; }

	virtual bool calcBoundingBox(Vec3f & minCoor, Vec3f & maxCoor) const;
	virtual void registIntoGrid(Grid & grid) const;

	virtual std::string toString()
	{
		char str[10];
		sprintf(str, "%d", objs_num);

		string re = string("Group Information:\n") 
				  + string("Object Number = ") + string(str) + NEW_LINE_CHAR;
		
		re += NEW_LINE_CHAR;
		for (int i = 0; i < objs_num; ++i)
		{
			re += objs[i]->toString() + NEW_LINE_CHAR;
		}

		return re;
	}


private:

	int objs_num;
	Object3D ** objs;

};

/*
inline ostream & operator << (ostream & os, const Group & g)
{
	os << "Group Information:"	<< endl
	   << "Object Number = "	<< g.objs_num << endl;
	
	for (int i = 0; i < g.objs_num; ++i)
		os << g.objs[i] << endl;

	return os;
}
*/

#endif