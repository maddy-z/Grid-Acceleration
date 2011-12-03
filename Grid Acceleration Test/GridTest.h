#ifndef _GRID_TEST_H_
#define _GRID_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../Grid Acceleration/Grid.h"
#include "../Grid Acceleration/Vector.h"

// =======================================
// Grid Test Based on CppUnit::TestFixture
// =======================================

class GridTest : public CppUnit::TestFixture

{

	CPPUNIT_TEST_SUITE( GridTest );
		CPPUNIT_TEST( testRasterizeSphere );
		CPPUNIT_TEST( testInitializeRayMarch );
		CPPUNIT_TEST( testNextCell );
		CPPUNIT_TEST( testIntersect );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

	void testRasterizeSphere();
	void testInitializeRayMarch();
	void testNextCell();
	void testIntersect();

private:
	
	Grid * grid_4_4_4_1;
	Grid * grid_4_4_4_2;
	Grid * grid_4_4_4_3;
	Grid * grid_4_4_4_4;

	Vec3f min_coor1, max_coor1;
	Vec3f min_coor2, max_coor2;
	Vec3f min_coor3, max_coor3;
	Vec3f min_coor4, max_coor4;
};

#endif