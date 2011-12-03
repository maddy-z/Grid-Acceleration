#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "../Grid Acceleration/Grid.h"
#include "../Grid Acceleration/Ray.h"
#include "../Grid Acceleration/Vector.h"
#include "../Grid Acceleration/MarchInfo.h"

#include "GridTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION ( GridTest );

// ===================================
//  Implemention Of Class -- GridTest
// ===================================

void GridTest::setUp()
{
	min_coor1.Set(0.0f, 0.0f, 0.0f); max_coor1.Set(1.0f, 1.0f, 1.0f);
	min_coor2.Set(0.0f, 0.0f, 0.0f); max_coor2.Set(1.0f, 1.0f, 1.0f);
	min_coor3.Set(0.0f, 0.0f, 0.0f); max_coor3.Set(1.0f, 1.0f, 1.0f);
	min_coor4.Set(-2.0f, -2.0f, -2.0f); max_coor4.Set(2.0f, 2.0f, 2.0f);

	grid_4_4_4_1 = new Grid ( min_coor1, max_coor1, 4, 4, 4, NULL );	
	grid_4_4_4_2 = new Grid ( min_coor2, max_coor2, 4, 4, 4, NULL );
	grid_4_4_4_3 = new Grid ( min_coor3, max_coor3, 4, 4, 4, NULL );
	grid_4_4_4_4 = new Grid ( min_coor4, max_coor4, 4, 4, 4, NULL );
}


void GridTest::tearDown()
{
	delete grid_4_4_4_1;
	delete grid_4_4_4_2;
	delete grid_4_4_4_3;
	delete grid_4_4_4_4;
}


void GridTest::testInitializeRayMarch()
{
	std::cout << "Start to test Function of initializeRayMarch" << std::endl;
	
	Vec3f dir(1.0f, 0.0f, 0.0f);
	Vec3f origin1(0.5f, 0.5f, 0.5f), origin2(-2.5f, 0.5f, 0.5f), origin3(-3.0f, -3.5f, 0.5f);

	Ray r1(dir, origin1), r2(dir, origin2), r3(dir, origin3);
	
	MarchInfo mi;
	
	bool re1 = grid_4_4_4_4->initializeRayMarch(mi, r1, 0.0f);
	CPPUNIT_ASSERT ( true );
	std::cout << mi.toString() << endl;
	
	bool re2 = grid_4_4_4_4->initializeRayMarch(mi, r2, 0.0f);
	CPPUNIT_ASSERT ( re2 == true );
	std::cout << mi.toString() << endl;

	bool re3 = grid_4_4_4_4->initializeRayMarch(mi, r3, 0.0f);
	CPPUNIT_ASSERT ( re3 == false);
	std::cout << mi.toString() << endl;

	std::cout << "End to test Function of initializeRayMarch" << std::endl;

	return;
}


void GridTest::testNextCell()
{
	// TODO
	std::cout << "Start to test Function of nextCall" << std::endl;

	Vec3f origin ( -3.0f, -2.0f, 0.5f );
	Vec3f dir ( 5.0f, 2.0f, 0.0f );

	Grid grid(Vec3f(-2.0f, -2.0f, -2.0f), Vec3f(2.0f, 2.0f, 2.0f), 4, 4, 4, NULL);
	Ray r1(dir, origin);
	MarchInfo mi;

	grid.initializeRayMarch(mi, r1, 0.0f);
	std::cout << mi.toString() << std::endl;

	mi.nextCell();
	// std::cout << mi.toString() << std::endl;
	CPPUNIT_ASSERT (mi.get_i() == 1 && mi.get_j() == 0 && mi.get_k() == 2);

	mi.nextCell();
	// std::cout << mi.toString() << std::endl;
	CPPUNIT_ASSERT (mi.get_i() == 1 && mi.get_j() == 1 && mi.get_k() == 2);
	
	std::cout << "End to test Function of nextCall" << std::endl;

	return;
}


void GridTest::testRasterizeSphere()
{
	std::cout << "Start to test Function of rasterizeSphere" << endl;

	std::cout << grid_4_4_4_1->toString() << endl;
	grid_4_4_4_1->rasterizeSphere(Vec3f(0.5f, 0.5f, 0.5f), 0.1f);
	grid_4_4_4_1->printRasterizedSphere();
	std::cout << endl;
	
	std::cout << grid_4_4_4_2->toString() << endl;
	grid_4_4_4_2->rasterizeSphere(Vec3f(0.5f, 0.5f, 0.5f), 0.25f);
	grid_4_4_4_2->printRasterizedSphere();
	std::cout << endl;

	std::cout << grid_4_4_4_3->toString() << endl;
	grid_4_4_4_3->rasterizeSphere(Vec3f(0.5f, 0.5f, 0.5f), 0.5f);
	grid_4_4_4_3->printRasterizedSphere();
	std::cout << endl;

	std::cout << "End to test Function of rasterizeSphere" << endl;

	return;
}


void GridTest::testIntersect()
{

	std::cout << "Start to test Function of intersect" << endl;

	Grid grid_3x3_1(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.9f, 0.9f, 0.9f), 3, 3, 3, NULL);
	grid_3x3_1.rasterizeSphere(Vec3f(0.75f, 0.15f, 0.15f), 0.1f);
	grid_3x3_1.printRasterizedSphere();

	Grid grid_3x3_2(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.9f, 0.9f, 0.9f), 3, 3, 3, NULL);
	grid_3x3_2.rasterizeSphere(Vec3f(0.45f, 0.15f, 0.15f), 0.1f);
	grid_3x3_2.printRasterizedSphere();

	MarchInfo mi;
	Hit h1(FLT_MAX, NULL, Vec3f(0.0f, 0.0f, 1.0f)),
		h2(FLT_MAX, NULL, Vec3f(0.0f, 0.0f, 1.0f));

	Ray r1(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(-1.0f, 0.1f, 0.1f));
	
	grid_3x3_1.initializeRayMarch(mi, r1, 0.0f);
	std::cout << endl << mi.toString() << endl;

	grid_3x3_1.intersect(r1, h1, 0.0f);
	std::cout << endl << h1.toString() << endl;

	Vec3f result1 = h1.getIntersectionPoint();
	// CPPUNIT_ASSERT ( result1[0] == 0.6f && result1[1] == 0.1f && result1[2] == 0.1f );

	grid_3x3_2.intersect(r1, h2, 0.0f);
	std::cout << endl << h2.toString() << endl;

	Vec3f result2 = h2.getIntersectionPoint();
	// CPPUNIT_ASSERT ( result2[0] == 0.3f && result2[1] == 0.1f && result2[2] == 0.1f );

	std::cout << "End to test Function of intersect" << endl;

	return;

}