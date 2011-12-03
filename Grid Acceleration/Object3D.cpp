#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

#include "Object3D.h"
#include "Vector.h"
#include "Grid.h"

void Object3D::registIntoGrid(Grid & grid) const
{
	Vec3f minCoor, maxCoor;
	if ( !calcBoundingBox(minCoor, maxCoor) )
		return;
	
	Vec3f minGridCoor = grid.getMinCoor();
	Vec3f maxGridCoor = grid.getMaxCoor();
	
	float del[3];
	del[0] = grid.getDelta_X();
	del[1] = grid.getDelta_Y();
	del[2] = grid.getDelta_Z();

	int min_i[3], max_i[3];

	for (int i = 0; i < 3; ++i) 
	{
		min_i[i] = ( int )( ( minCoor[i] - minGridCoor[i] ) / del[i] );
		max_i[i] = ( int )( ( maxCoor[i] - minGridCoor[i] ) / del[i] );
	}
	
	if (min_i[0] == grid.getNum_X()) min_i[0] = grid.getNum_X() - 1;
	if (min_i[1] == grid.getNum_Y()) min_i[1] = grid.getNum_Y() - 1;
	if (min_i[2] == grid.getNum_Z()) min_i[2] = grid.getNum_Z() - 1;

	if (max_i[0] == grid.getNum_X()) max_i[0] = grid.getNum_X() - 1;
	if (max_i[1] == grid.getNum_Y()) max_i[1] = grid.getNum_Y() - 1;
	if (max_i[2] == grid.getNum_Z()) max_i[2] = grid.getNum_Z() - 1;

	assert ( 0 <= min_i[0] && min_i[0] < grid.getNum_X() );
	assert ( 0 <= min_i[1] && min_i[1] < grid.getNum_Y() );
	assert ( 0 <= min_i[2] && min_i[2] < grid.getNum_Z() );

	assert ( 0 <= max_i[0] && max_i[0] < grid.getNum_X() );
	assert ( 0 <= max_i[1] && max_i[1] < grid.getNum_Y() );
	assert ( 0 <= max_i[2] && max_i[2] < grid.getNum_Z() );

	for (int i = min_i[0]; i <= max_i[0]; ++i) 
	{
		for (int j = min_i[1]; j <= max_i[1]; ++j) 
		{
			for (int k = min_i[2]; k <= max_i[2]; ++k) {
				grid.registObjectIntoGrid(this, i, j, k);
			}
		}
	}

	return;
}