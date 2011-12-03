#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cfloat>
#include <iostream>
using namespace std;

#include "Object3D.h"
#include "Material.h"
#include "Vector.h"

#include "Grid.h"
#include "RayTracing_Stats.h"

// ============================
//	Implemention of Class Grid
// ============================

Grid::Grid(Vec3f min, 
		   Vec3f max, 
		   int nx, 
		   int ny, 
		   int nz, 
		   Material* m) : Object3D(m),
						  min_coor(min), 
						  max_coor(max),
						  num_x(nx),
						  num_y(ny),
						  num_z(nz),
						  voxels(NULL),
						  cells(NULL),
						  selfRendering_m(NULL)

{

	assert (min[0] < max[0] && min[1] < max[1] && min[2] < max[2]);
	assert (num_x > 0 && num_y > 0 && num_z > 0);

	del_x = ( max[0] - min[0] ) / ( float )( num_x );
	del_y = ( max[1] - min[1] ) / ( float )( num_y );
	del_z = ( max[2] - min[2] ) / ( float )( num_z );

	voxels = ( bool * )( new bool[num_x * num_y * num_z] );
	cells = ( struct Grid_Cell * )( new struct Grid_Cell[num_x * num_y * num_z] );

	for (int i = 0; i < num_x; ++i) 
	{
		for (int j = 0; j < num_y; ++j) 
		{
			for (int k = 0; k < num_z; ++k) 
			{
				voxels[num_y * num_z * i + num_z * j + k] = false;
			}
		}
	}

	if ( m == NULL ) {
		selfRendering_m = new PhongMaterial(Vec3f(1.0f, 1.0f, 1.0f), 
											Vec3f(0.0f, 0.0f, 0.0f), 
											0.0f, 
											Vec3f(0.0f, 0.0f, 0.0f), 
											Vec3f(0.0f, 0.0f, 0.0f), 
											0.0f);
	}

	return;
}

Grid::~Grid()
{
	delete [] voxels;
	delete [] cells;

	voxels = NULL;
	cells = NULL;

	if ( !selfRendering_m )
		delete selfRendering_m;
}


bool Grid::initializeRayMarch(MarchInfo & mi, const Ray & r, float tmin) const
{
	const Vec3f & origin = r.getOrigin();
	Vec3f dir = r.getDirection(); dir.Normalize();

	float t1, t2;
	float tNear, tFar;

	tNear = ( -1.0f ) * FLT_MAX;
	tFar = FLT_MAX;
	
	mi.sign_x = ( dir[0] >= 0.0f ) ? 1 : -1;
	mi.sign_y = ( dir[1] >= 0.0f ) ? 1 : -1;
	mi.sign_z = ( dir[2] >= 0.0f ) ? 1 : -1;

	mi.dtx = ( dir[0] == 0.0f ) ? ( FLT_MAX ) : ( mi.sign_x * del_x / dir[0] );
	mi.dty = ( dir[1] == 0.0f ) ? ( FLT_MAX ) : ( mi.sign_y * del_y / dir[1] );
	mi.dtz = ( dir[2] == 0.0f ) ? ( FLT_MAX ) : ( mi.sign_z * del_z / dir[2] );

	assert ( mi.dtx > 0.0f && mi.dty > 0.0f && mi.dtz > 0.0f );

	if (   min_coor[0] < origin[0] && origin[0] < max_coor[0] 
		&& min_coor[1] < origin[1] && origin[1] < max_coor[1]
		&& min_coor[2] < origin[2] && origin[2] < max_coor[2] ) 
	{
		// If a ray's origin is in the grid.
		
		int index[3];

		index[0] = ( int )( ( origin[0] - min_coor[0] ) / del_x );
		index[1] = ( int )( ( origin[1] - min_coor[1] ) / del_y );
		index[2] = ( int )( ( origin[2] - min_coor[2] ) / del_z );

		if ( index[0] == num_x ) index[0] = num_x - 1;
		if ( index[1] == num_y ) index[1] = num_y - 1;
		if ( index[2] == num_z ) index[2] = num_z - 1;

		float coor[3];

		coor[0] = ( dir[0] > 0.0f ) ? ( index[0] + 1 ) * del_x + min_coor[0] : index[0] * del_x + min_coor[0];
		coor[1] = ( dir[1] > 0.0f ) ? ( index[1] + 1 ) * del_y + min_coor[1] : index[1] * del_y + min_coor[1];
		coor[2] = ( dir[2] > 0.0f ) ? ( index[2] + 1 ) * del_z + min_coor[2] : index[2] * del_z + min_coor[2];

		mi.tmin = 0.0f;
	
		if ( dir[0] != 0.0f ) { mi.next_x = ( coor[0] - origin[0] ) / dir[0]; }
		else if ( dir[0] == 0.0f ) { mi.next_x = FLT_MAX; }

		if ( dir[1] != 0.0f ) { mi.next_y = ( coor[1] - origin[1] ) / dir[1]; }
		else if ( dir[1] == 0.0f ) { mi.next_y = FLT_MAX; }

		if ( dir[2] != 0.0f ) { mi.next_z = ( coor[2] - origin[2] ) / dir[2]; }
		else if ( dir[2] == 0.0f ) { mi.next_z = FLT_MAX; }

		mi.i = index[0];
		mi.j = index[1];
		mi.k = index[2];

		while (mi.tmin < tmin)
		{
			if ( IsCellValid(mi) == false )
				return false;

			mi.nextCell();
		}

		return true;
	} 
	else {

		// If a ray's origin is not in the grid

		for (int i = 0; i < 3; ++i) 
		{
			if (dir[i] == 0.0f) 
			{
				if (origin[i] < min_coor[i] || origin[i] > max_coor[i]) {
					return false;						// Condition of Parallel
				} else { 
					continue;
				}
			}
			else {
				t1 = ( min_coor[i] - origin[i] ) / dir[i];
				t2 = ( max_coor[i] - origin[i] ) / dir[i];
			}

			if (t1 > t2) {
				float tmp;
				tmp = t1; t1 = t2; t2 = tmp;
			}											// Swap t1 & t2

			bool changeNormal = false;

			if (t1 > tNear) { tNear = t1; changeNormal = true; }
			if (t2 < tFar)  { tFar = t2; }

			if (tNear > tFar) return false;				// Not intersect with Grid
			if (tFar < 0.0f) return false;				// Grid is behind the ray 

			if (i == 0 && mi.dtx != FLT_MAX) { mi.next_x = t1; }
			else if (( i == 0 && mi.dtx == FLT_MAX ) || dir[0] == 0.0f) { mi.next_x = FLT_MAX; }
			
			if (i == 1 && mi.dty != FLT_MAX) { mi.next_y = t1; }
			else if (( i == 1 && mi.dty == FLT_MAX ) || dir[1] == 0.0f) { mi.next_y = FLT_MAX; }

			if (i == 2 && mi.dtz != FLT_MAX) { mi.next_z = t1; }
			else if (( i == 2 && mi.dtz == FLT_MAX ) || dir[2] == 0.0f) { mi.next_z = FLT_MAX; }
		
			if ( changeNormal ) {
				if (i == 0) mi.surfaceNormal.Set((-1.0f) * mi.sign_x, 0.0f, 0.0f);
				if (i == 1) mi.surfaceNormal.Set(0.0f, (-1.0f) * mi.sign_y, 0.0f);
				if (i == 2) mi.surfaceNormal.Set(0.0f, 0.0f, (-1.0f) * mi.sign_z);
			}
		}

		// To update info of MarchInfo mi

		Vec3f pt = origin + dir * tNear;
		
		mi.i = ( int )( ( pt[0] - min_coor[0] ) / del_x );
		mi.j = ( int )( ( pt[1] - min_coor[1] ) / del_y );
		mi.k = ( int )( ( pt[2] - min_coor[2] ) / del_z );

		if ( mi.i < 0 ) mi.i = 0;
		if ( mi.j < 0 ) mi.j = 0;
		if ( mi.k < 0 ) mi.k = 0;

		if ( mi.i == num_x ) mi.i = num_x - 1;
		if ( mi.j == num_y ) mi.j = num_y - 1;
		if ( mi.k == num_z ) mi.k = num_z - 1;

		mi.tmin = tNear;

		while (mi.next_x <= tNear) mi.next_x += mi.dtx;
		while (mi.next_y <= tNear) mi.next_y += mi.dty;
		while (mi.next_z <= tNear) mi.next_z += mi.dtz;

		/*
		if ( min_coor[0] < origin[0] && origin[0] < max_coor[0] ) {
			while (mi.next_x <= tNear)
				mi.next_x += mi.dtx;
		}
		if ( min_coor[1] < origin[1] && origin[1] < max_coor[1] ) {
			while (mi.next_y <= tNear)
				mi.next_y += mi.dty;
		}
		if ( min_coor[2] < origin[2] && origin[2] < max_coor[2] ) {
			while (mi.next_z <= tNear)
				mi.next_z += mi.dtz;
		}
		*/

		while ( mi.tmin < tmin ) 
		{
			if ( IsCellValid(mi) == false )
				return false;
			
			mi.nextCell();
		}

		return true;
	}
}


void Grid::rasterizeSphere(Vec3f center, float radius)
{
	Vec3f coord;

	for (int i = 0; i < num_x; ++i) 
	{
		for (int j = 0; j < num_y; ++j) 
		{
			for (int k = 0; k < num_z; ++k) 
			{
				coord.Set ( min_coor[0] + (i + 0.5f) * del_x, 
							min_coor[1] + (j + 0.5f) * del_y,
							min_coor[2] + (k + 0.5f) * del_z );

				if ( (coord - center).Length() <= radius ) 
				{
					voxels[num_y * num_z * i + num_z * j + k] = true;
				}
				else 
				{
					voxels[num_y * num_z * i + num_z * j + k] = false;
				}
			}
		}
	}

	return;
}


void Grid::printRasterizedSphere()
{
	for (int i = 0; i < num_x; ++i) 
	{
		for (int j = 0; j < num_y; ++j) 
		{
			for (int k = 0; k < num_z; ++k) 
			{
				// assert ( voxels[num_y * num_z * i + num_z * j + k] == cells[num_y * num_z * i + num_z * j + k].flag );

				// assert ( *(bool *)((char *)(voxels) + (num_y*num_z*i + (num_z*j + k)) * sizeof(bool)) == 
				//		(*(struct Grid_Cell *)((char *)(cells) + (num_y*num_z*i + (num_z*j + k)) * sizeof(struct Grid_Cell))).flag );
				
				if ( voxels[num_y * num_z * i + num_z * j + k] == true ) {
					printf("< %d, %d, %d >\n", i, j, k);
				}

				// if ((*(struct Grid_Cell *)((char *)(cells) + (num_y*num_z*i + num_z*j + k) * sizeof(struct Grid_Cell))).flag == true) {
				//	printf("< %d, %d, %d >\n", i, j, k);
				// }
			}
		}
	}

	return;
}


void Grid::registObjectIntoGrid(const Object3D * obj, int i, int j, int k)
{
	if (obj == NULL) {
		return;
	}

	if (i < 0 || i >= num_x ||
		j < 0 || j >= num_y || 
		k < 0 || k >= num_z)
	{
		return;
	}

	cells[num_y * num_z * i + num_z * j + k].objs.addObject((Object3D *)(obj));

	return;
}


bool Grid::intersect(const Ray & r, Hit & h, float tmin)
{
	RayTracing_Stats::IncrementNumIntersections();

	MarchInfo mi;

	bool re = initializeRayMarch(mi, r, tmin);
	if ( re == false ) {
		return false;
	}

	int i, j, k;

	while ( IsCellValid(mi) ) 
	{
		RayTracing_Stats::IncrementNumGridCellsTraversed();

		i = mi.get_i();
		j = mi.get_j();
		k = mi.get_k();

		if ( voxels[num_y * num_z * i + num_z * j + k] == true ) 
		{
			if (mi.tmin < h.getT()) 
			{
				h.set(mi.tmin, material, mi.surfaceNormal, r);
				return true;
			}
			else return false;
		}
		else {
			mi.nextCell();
		}
	}

	return false;
}


bool Grid::intersectWithOverlapObject(const Ray & r, Hit & h, float tmin) const
{
	MarchInfo mi;

	bool re = initializeRayMarch ( mi, r, tmin );
	if ( re == false ) {
		return false;
	}

	int index_i, index_j, index_k;

	while ( IsCellValid(mi) ) 
	{
		RayTracing_Stats::IncrementNumGridCellsTraversed();

		index_i = mi.get_i();
		index_j = mi.get_j();
		index_k = mi.get_k();

		Object3DVector & obj_array = cells[num_y * num_z * index_i + num_z * index_j + index_k].objs;
		
		int num_objs = obj_array.getNumOfObjects();
		if ( num_objs > 0 ) 
		{
			bool flag = false;
			
			for (int i = 0; i < num_objs; ++i) {
				Object3D * obj = obj_array.getObject(i);
				
				if (obj->intersect(r, h, tmin) == true)
					flag = true;
			}

			if ( flag ) return true;
		}
		
		mi.nextCell();
	}

	return false;
}


bool Grid::intersectWithGridVisualization(const Ray & r, Hit & h, float tmin) const
{
	RayTracing_Stats::IncrementNumIntersections();

	MarchInfo mi;

	bool re = initializeRayMarch(mi, r, tmin);
	if ( re == false ) {
		return false;
	}

	int index_i, index_j, index_k;

	while ( IsCellValid(mi) ) 
	{
		RayTracing_Stats::IncrementNumGridCellsTraversed();

		index_i = mi.get_i();
		index_j = mi.get_j();
		index_k = mi.get_k();

		Object3DVector & obj_array = cells[num_y * num_z * index_i + num_z * index_j + index_k].objs;
		
		int num_objs = obj_array.getNumOfObjects();
		if ( num_objs > 0 ) 
		{
			if ( mi.tmin < h.getT() ) 
			{
				h.set(mi.tmin, selfRendering_m, mi.surfaceNormal, r);
				return true;
			}
			else
				return false;
		} else {
			mi.nextCell();
		}
	}

	return false;
}

bool Grid::intersectWithGridVisualizationCount(const Ray & r, Hit & h, float tmin) const
{
	// TODO
	return false;
}


bool Grid::IsCellValid(const MarchInfo & mi) const 
{
	int index_i = mi.get_i(), 
		index_j = mi.get_j(), 
		index_k = mi.get_k();

	return ( 0 <= index_i && index_i < num_x &&
			 0 <= index_j && index_j < num_y &&
			 0 <= index_k && index_k < num_z );
}


bool Grid::calcBoundingBox(Vec3f & minCoor, Vec3f & maxCoor) const
{
	assert (min_coor[0] < max_coor[0] && 
			min_coor[1] < max_coor[1] &&
			min_coor[2] < max_coor[2]);

	minCoor = min_coor;
	maxCoor = max_coor;

	return true;
}


void Grid::printRegistrationInfo(ostream & os)
{
	for (int i = 0; i < num_x; ++i) 
	{
		for (int j = 0; j < num_y; ++j) 
		{
			for (int k = 0; k < num_z; ++k) 
			{
				int n = cells[num_y * num_z * i + num_z * j + k].objs.getNumOfObjects();
				if (n > 0) {
					os << "Index Of < " << i << ", " << j << ", " << k << " >" << " -> " << n << endl;					
				}
			}
		}
	}

	return;
}