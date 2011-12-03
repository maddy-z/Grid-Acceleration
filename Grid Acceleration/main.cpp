#include <cmath>
#include <cfloat>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#include "Common.h"

#include "RayTracer.h"
#include "RayTracing_Stats.h"

// =====================================================
//  Static Global Variables used to parse the arguments 
// =====================================================

char * input_file  = NULL;
char * output_file = NULL;

int imageWidth  = 100;
int imageHeight = 100;

int max_bounces = 0;
float min_weight = 0.1f;

int grid_x = 0;
int grid_y = 0;
int grid_z = 0;

bool NO_SHADOWS = false;
bool VISUAL_GRID = false;
bool VISUAL_GRID_COUNT = false;

RayTracer::Grid_Rendering_Type rendering_type = RayTracer::NORMAL;


// ==============
//  Test Program
// ==============

int main(int argc, char ** argv)

{

	// Sample Command Lines:

	// raytracer -input input_scene.txt -size 1000 1000 -output output_scene.tga
	// raytracer -input input_scene.txt -size 1000 1000 -output output_scene.tga -no_shadows

	// raytracer -input input_scene.txt -size 1000 1000 -output output_scene.tga -grid 10 10 10 -visual_grid
	// raytracer -input input_scene.txt -size 1000 1000 -output output_scene.tga -grid 10 10 10 -visual_grid -no_shadows

	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-input")) 
		{
			i++; assert (i < argc); 
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-output")) 
		{
			i++; assert (i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) 
		{
			i++; assert (i < argc); 
			imageWidth = atoi(argv[i]);
			
			i++; assert (i < argc);
			imageHeight = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-grid"))
		{
			i++; assert (i < argc);
			grid_x = atoi(argv[i]);

			i++; assert (i < argc);
			grid_y = atoi(argv[i]);

			i++; assert (i < argc);
			grid_z = atoi(argv[i]);

			assert ( grid_x > 0 && grid_y > 0 && grid_z > 0 );

			if ( i == argc - 1 )
				continue;

			i++; assert (i < argc);
			if ( !strcmp(argv[i], "-visualize_grid") )
			{
				VISUAL_GRID = true;
				rendering_type = RayTracer::VISUAL_GRID;
			}
			else if ( !strcmp(argv[i], "-visualize_grid_count") ) 
			{
				VISUAL_GRID_COUNT = true;
				rendering_type = RayTracer::VISUAL_GRID_COUNT;
			}

			assert ( !( VISUAL_GRID == true && VISUAL_GRID_COUNT == true ) );

		}
		else if (!strcmp(argv[i], "-no_shadows")) 
		{
			NO_SHADOWS = true;
		}
		else 
		{
			printf ("Hoops error with command line argument %d: '%s'\n", i, argv[i]);
		}
	}

	printf("Command Arguments Information:\n");
	printf("\t-input  %s\n", input_file);
	printf("\t-output  %s\n", output_file);
	printf("\t-size  %d %d\n", imageHeight, imageWidth);
	printf("\t-grid  %d %d %d\n", grid_x, grid_y, grid_z);

	if ( NO_SHADOWS ) { printf("\t-no_shadows\n"); }
	if ( VISUAL_GRID ) { printf("\t-visualize_grid\n"); }
	if ( VISUAL_GRID_COUNT ) { printf("\t-visualize_grid_count\n"); }

	std::cout << std::endl;

	// system("pause");
	// return 0;

	// ======================
	//  Scene Initialization
	// ======================

	SceneParser sparser ( input_file );
	// std::cout << sparser.toString() << std::endl;

	Image img ( imageWidth, imageHeight );
	img.SetAllPixels ( sparser.getBackgroundColor() );

	RayTracer rayTracer ( &sparser, max_bounces,  min_weight );
	Camera * cam = sparser.getCamera();

	Vec3f minCoor, maxCoor;
	findMinMaxCoor ( &sparser, minCoor, maxCoor );	

	/*
	std::cout << "Min Coor = " << minCoor.toString() << std::endl;
	std::cout << "Max Coor = " << maxCoor.toString() << std::endl;

	system("pause");
	return 0;
	*/

	if ( grid_x == 0 && grid_y == 0 && grid_z == 0 ) 
	{
		std::cout << "Start Ray Trace with Statistics" << std::endl;

		RayTracing_Stats::Initialize(imageWidth, imageHeight, grid_x, grid_y, grid_z, minCoor, maxCoor);

		for (int i = 0; i < imageHeight; ++i)
		{
			for (int j = 0; j < imageWidth; ++j)
			{
				Vec2f cameraPoint( (float)(i) / (float)(imageHeight), (float)(j) / (float)(imageWidth) );
				Ray ray = cam -> generateRay(cameraPoint);
				Hit hitPoint ( FLT_MAX, NULL, Vec3f(0.0f, 1.0f, 0.0f) );

				Vec3f pixel = rayTracer.traceRayStat(ray, EPSILON, 0, 1.0f, 1.0f, hitPoint, NO_SHADOWS);
				if (pixel[0] == -1.0f && pixel[1] == -1.0f && pixel[2] == -1.0f)
					continue;

				if ( pixel[0] > 1.0f ) pixel.Set ( 1.0f, pixel[1], pixel[2] );
				if ( pixel[1] > 1.0f ) pixel.Set ( pixel[0], 1.0f, pixel[2] );
				if ( pixel[2] > 1.0f ) pixel.Set ( pixel[0], pixel[1], 1.0f );
				
				img.SetPixel(j, i, pixel);
			}
		}

		std::cout << "End Ray Trace with Statistics" << std::endl;
	}
	else 
	{
		assert ( grid_x > 0 && grid_y > 0 && grid_z > 0 );
		assert ( ! ( VISUAL_GRID && VISUAL_GRID_COUNT ) );

		RayTracing_Stats::Initialize(imageWidth, imageHeight, 
									 grid_x, grid_y, grid_z, 
									 minCoor, maxCoor);

		// ========================================================
		//  Grid Acceleration is enabled to accelerate ray tracing
		// ========================================================	

		// =============
		//  Create Grid
		// =============
		
		std::cout << "Start to Create Grid" << endl;

		Grid grid ( minCoor, maxCoor, grid_x, grid_y, grid_z, NULL );

		std::cout << "End to Create Grid" << endl << endl;

		/*
		Grid grid ( Vec3f(-2.0f, -2.0f, -2.0f), Vec3f(2.0f, 2.0f, 2.0f), grid_x, grid_y, grid_z, NULL );

		std::cout << grid.toString() << std::endl;
		system("pause");
		return 0;
		*/

		// =============================
		//  Insert Primitives Into Grid
		// =============================
		
		std::cout << "Start to Insert Primitives Into Grid" << endl;

		const Group * group = sparser.getGroup();
		for (int i = 0; i < group -> getObjectNumber(); ++i) {
			const Object3D * obj = group -> getObjectOfIndex(i);
			obj -> registIntoGrid(grid);
		}

		// grid.printRegistrationInfo(std::cout);

		// system("pause");
		// return 0;
	
		std::cout << "End to Insert Primitives Into Grid" << endl << endl;

		// ===================
		//  Start Ray Tracing
		// ===================

		std::cout << "Start to Ray Trace with Accelerating Grid" << std::endl;

		for (int i = 0; i < imageHeight; ++i) 
		{
			for (int j = 0; j < imageWidth; ++j)
			{
				Vec2f cameraPoint( (float)(i) / (float)(imageHeight), (float)(j) / (float)(imageWidth) );
				Ray ray = cam -> generateRay(cameraPoint);
				Hit hitPoint ( FLT_MAX, NULL, Vec3f(0.0f, 1.0f, 0.0f) );

				Vec3f pixel = rayTracer.traceRayFast(ray, EPSILON, 0, 1.0f, 1.0f, hitPoint, NO_SHADOWS, grid, rendering_type);
				if (pixel[0] == -1.0f && pixel[1] == -1.0f && pixel[2] == -1.0f)
					continue;

				if ( pixel[0] > 1.0f ) pixel.Set( 1.0f, pixel[1], pixel[2] );
				if ( pixel[1] > 1.0f ) pixel.Set( pixel[0], 1.0f, pixel[2] );
				if ( pixel[2] > 1.0f ) pixel.Set( pixel[0], pixel[1], 1.0f );
				
				img.SetPixel(j, i, pixel);
			}
		}

		std::cout << "End to Ray Trace with Accelerating Grid" << std::endl;
	}

	if (output_file != NULL) {
		img.SaveTGA(output_file);
	}

	
	// Write Statistic Data Into File Stream

	std::string fileName(output_file);
	std::string fileNameWithoutExt = fileName.substr(0, fileName.length()-4);
	fileNameWithoutExt += string("_log.txt");
	const char * fileNameArray = fileNameWithoutExt.c_str();

	ofstream output_file(fileNameArray);
	RayTracing_Stats::PrintStatistics(output_file);

	std::cout << std::endl;

	// system("pause");
	return 0;

}