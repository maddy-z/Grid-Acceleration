#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "Object3D.h"
#include "Triangle.h"

#include "Vector.h"
#include "Matrix.h"

#include "RayTracing_Stats.h"

bool Triangle::intersect(const Ray & r, Hit & h, float tmin)

{

	RayTracing_Stats::IncrementNumIntersections();

	const Vec3f & e1 = b - a;
	const Vec3f & e2 = c - a;

	const Vec3f & ori = r.getOrigin();
	const Vec3f & dir = r.getDirection();

	// P
	Vec3f P;
	Vec3f::Cross3(P, dir, e2);
	
	float det = e1.Dot3(P);

	Vec3f T; 
	if ( det > 0 ) {
		T = ori - a;
	}
	else {
		T = a - ori;
		det = -det;
	}

	// if ( det < 0.0001f )
	//	return false;

	float u = T.Dot3(P);
	if ( u < 0.0f || u > det )
		return false;

	// Q
	Vec3f Q;
	Vec3f::Cross3(Q, T, e1);

	float v = dir.Dot3(Q);
	if ( v < 0.0f || u + v > det )
		return false;

	float tTmp = e2.Dot3(Q) / det;
	if (tTmp > 1.0e-6 && tTmp > tmin && tTmp < h.getT()) 
	{
		normal.Normalize();
		h.set(tTmp, material, normal, r);

		return true;
	}
	else 
		return false;

	/*
	Vec3f ab = a - b;
	Vec3f ac = a - c;
	Vec3f ar = a - r.getOrigin();
	Vec3f dir = r.getDirection();

	float coff[3][3] = {
							{ ab[0], ac[0], dir[0] },
							{ ab[1], ac[1], dir[1] },
							{ ab[2], ac[2], dir[2] }
					   };

	float mat_1[3][3] = {
							{ ar[0], ac[0], dir[0] },
							{ ar[1], ac[1], dir[1] },
							{ ar[2], ac[2], dir[2] }
					    };

	float mat_2[3][3] = {
							{ ab[0], ar[0], dir[0] },
							{ ab[1], ar[1], dir[1] },
							{ ab[2], ar[2], dir[2] }
					    };

	float mat_3[3][3] = {
							{ ab[0], ac[0], ar[0] },
							{ ab[1], ac[1], ar[1] },
							{ ab[2], ac[2], ar[2] }
					    };

	float coff_det = calcDeterminant3_3(coff);
	if (coff_det < 1.0e-6 && coff_det > -1.0e-6)
		return false;

	float beta = calcDeterminant3_3(mat_1) / coff_det;
	float alpha = calcDeterminant3_3(mat_2) / coff_det;
	float t = calcDeterminant3_3(mat_3) / coff_det;

	if (beta > 0.0f && alpha > 0.0f && beta + alpha < 1.0f)				// Intesection if ( p + r < 1 && r > 0 && p > 0 )
	{
		assert (tmin >= 0.0f);

		if (t > 1.0e-6 && t > tmin && t < h.getT()) 
		{
			normal.Normalize();
			h.set(t, material, normal, r);

			return true;
		}
		else
			return false;
	}
	else { 
		return false;
	}
	*/

}


bool Triangle::calcBoundingBox(Vec3f & minCoor, Vec3f & maxCoor) const
{
	minCoor = a;
	maxCoor = a;
	
	for (int i = 0; i < 3; ++i) 
	{
		if (minCoor[i] > b[i]) minCoor.Set(b[i], i);
		if (maxCoor[i] < b[i]) maxCoor.Set(b[i], i);
		
		if (minCoor[i] > c[i]) minCoor.Set(c[i], i);
		if (maxCoor[i] < c[i]) maxCoor.Set(c[i], i);
	}
	
	Vec3f Del ( BOUNDINGBOX_EPSILON, BOUNDINGBOX_EPSILON, BOUNDINGBOX_EPSILON );
	minCoor -= Del;
	maxCoor += Del;

	return true;
}