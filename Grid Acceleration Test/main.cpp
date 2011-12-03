#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include "../Grid Acceleration/Vector.h"
#include "../Grid Acceleration/Grid.h"

#include "GridTest.h"

int main(int argc, char ** argv)
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry & registry = CppUnit::TestFactoryRegistry::getRegistry();

	runner.addTest ( registry.makeTest() );
	runner.run();

	system("pause");
	return 0;
}