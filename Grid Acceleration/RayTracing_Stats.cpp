#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

#include "RayTracing_Stats.h"

int RayTracing_Stats::width;
int RayTracing_Stats::height;

int RayTracing_Stats::num_x;
int RayTracing_Stats::num_y;
int RayTracing_Stats::num_z;

Vec3f RayTracing_Stats::min_bounds;
Vec3f RayTracing_Stats::max_bounds;

unsigned long long RayTracing_Stats::start_time;
unsigned long long RayTracing_Stats::num_nonshadow_rays;
unsigned long long RayTracing_Stats::num_shadow_rays;
unsigned long long RayTracing_Stats::num_intersections;
unsigned long long RayTracing_Stats::num_grid_cells_traversed;

// ====================================================================
// ====================================================================

void RayTracing_Stats::PrintStatistics(ostream & os)
{
	int delta_time = time ( NULL ) - start_time;
	
	if (delta_time == 0) 
		delta_time = 1;
	
	int secs  = delta_time % 60;
	int min   = ( delta_time / 60 ) % 60; 
	int hours = delta_time / ( 60 * 60 );
	
	int num_rays = num_nonshadow_rays + num_shadow_rays;
	
	float rays_per_sec =  float(num_rays) / float(delta_time);
	float rays_per_pixel =  float(num_rays) / float(width * height);
	float intersections_per_ray = num_intersections / float(num_rays);
	float traversed_per_ray = num_grid_cells_traversed / float(num_rays);

	char str[50];

	os << "**************************************************" << endl;
	os << "RAY TRACING STATISTICS" << endl << endl;
	sprintf(str, "%ld:%02d:%02d", hours, min, secs);
	os << "Total time                 " << string(str) << endl;
	sprintf(str, "%d (%dx%d)", width * height, width, height);
	os << "Num pixels                 " << string(str) << endl;
	sprintf(str, "(%0.2f %0.2f %0.2f) -> (%0.2f %0.2f %0.2f)", min_bounds.x(), min_bounds.y(), min_bounds.z(), max_bounds.x(), max_bounds.y(), max_bounds.z());
	os << "Scene bounds               " << string(str) << endl;
	sprintf(str, "%d (%dx%dx%d)", num_x * num_y * num_z, num_x, num_y, num_z);
	os << "Num grid cells             " << string(str) << endl;
	sprintf(str, "%lld", num_nonshadow_rays);
	os << "Num non-shadow rays        " << string(str) << endl;
	sprintf(str, "%lld", num_shadow_rays);
	os << "Num shadow rays            " << string(str) << endl;
	sprintf(str, "%lld", num_intersections);
	os << "Total intersections        " << string(str) << endl;
	sprintf(str, "%lld", num_grid_cells_traversed);
	os << "Total cells traversed      " << string(str) << endl;
	sprintf(str, "%0.1f", rays_per_sec);
	os << "Rays per second            " << string(str) << endl;
	sprintf(str, "%0.1f", rays_per_pixel);
	os << "Rays per pixel             " << string(str) << endl;
	sprintf(str, "%0.1f", intersections_per_ray);
	os << "Intersections per ray      " << string(str) << endl;
	sprintf(str, "%0.1f", traversed_per_ray);
	os << "Cells traversed per ray    " << string(str) << endl;
	os << "**************************************************" << endl;
	
	return;
}

// ====================================================================
// ====================================================================
