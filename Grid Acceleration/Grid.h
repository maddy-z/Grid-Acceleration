#ifndef _GRID_H_
#define _GRID_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include "Object3D.h"
#include "Object3DVector.h"
#include "Sphere.h"

#include "Vector.h"
#include "Hit.h"
#include "Ray.h"

#include "Material.h"
#include "MarchInfo.h"

#include "Scene_Parser.h"


// =========================
//  Definition of Grid Cell
// =========================

typedef struct Grid_Cell
{
	Object3DVector objs;
} Grid_Cell;

//typedef struct Object3D_Record
//{
//	bool visited;
//	Object3D * obj;
//} Object3D_Record;

// ==========================
//  Definition of Class Grid
// ==========================

class Grid : public Object3D

{

public:

	Grid(Vec3f min, 
		 Vec3f max,
		 int nx, 
		 int ny, 
		 int nz,
		 Material * m = NULL);

	virtual ~Grid();

	bool initializeRayMarch(MarchInfo & mi, const Ray & r, float tmin) const;

	// Only For Test
	void rasterizeSphere(Vec3f center, float radius);
	void printRasterizedSphere();

	Vec3f getMinCoor() const { return min_coor; }
	Vec3f getMaxCoor() const { return max_coor; }

	int getNum_X() const { return num_x; }
	int getNum_Y() const { return num_y; }
	int getNum_Z() const { return num_z; }

	float getDelta_X() const { return del_x; }
	float getDelta_Y() const { return del_y; }
	float getDelta_Z() const { return del_z; }
	
	// Useless Virtual Function for Class Grid
	virtual bool intersect(const Ray & r, Hit & h, float tmin);
	virtual bool calcBoundingBox(Vec3f & minCoor, Vec3f & maxCoor) const;
	
	bool intersectWithOverlapObject(const Ray & r, Hit & h, float tmin) const;
	bool intersectWithGridVisualization(const Ray & r, Hit & h, float tmin) const;
	bool intersectWithGridVisualizationCount(const Ray & r, Hit & h, float tmin) const;

	void registObjectIntoGrid(const Object3D * obj, int i, int j, int k);
	void printRegistrationInfo(ostream & os);

	bool IsCellValid(const MarchInfo & mi) const;

	virtual std::string toString()
	{
		string header = "Grid:\n";

		char str1[DEFAULT_CHAR_ARRAY_NUMBER], str2[DEFAULT_CHAR_ARRAY_NUMBER], str3[DEFAULT_CHAR_ARRAY_NUMBER], 
			 str4[DEFAULT_CHAR_ARRAY_NUMBER], str5[DEFAULT_CHAR_ARRAY_NUMBER], str6[DEFAULT_CHAR_ARRAY_NUMBER];

		sprintf_s(str1, "Num_x = %d", num_x);
		sprintf_s(str2, "Num_y = %d", num_y);
		sprintf_s(str3, "Num_z = %d", num_z);
		
		sprintf_s(str4, "del_x = %f", del_x);
		sprintf_s(str5, "del_y = %f", del_y);
		sprintf_s(str6, "del_z = %f", del_z);

		string re = header + 
					string("\tMin Coor = ") + min_coor.toString() + string("\n") + 
					string("\tMax Coor = ") + max_coor.toString() + string("\n") + 
					string("\t") + string(str1) + string("\n") + 
					string("\t") + string(str2) + string("\n") + 
					string("\t") + string(str3) + string("\n") + 
					string("\t") + string(str4) + string("\n") + 
					string("\t") + string(str5) + string("\n") + 
					string("\t") + string(str6) + string("\n");

		if ( material ) {
			re += string("\t") + material -> toString();
		}

		return re;
	
	}


private:

	Vec3f min_coor;
	Vec3f max_coor;
	
	int num_x;
	int num_y;
	int num_z;

	float del_x;
	float del_y;
	float del_z;

	bool * voxels;
	struct Grid_Cell * cells;

	Material * selfRendering_m;

};

#endif