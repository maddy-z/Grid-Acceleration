#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "Scene_Parser.h"

class Hit;
class Ray;
class Grid;

// ==================================
// ==================================

#define EPSILON 1.0e-4

class RayTracer

{

public:
	
	enum Grid_Rendering_Type { NORMAL, VISUAL_GRID, VISUAL_GRID_COUNT };

public:
	
	RayTracer( SceneParser * sp,
			   int max_b,
			   float min_wei ) : sparser(sp), max_bounces(max_b), min_weight(min_wei), IS_GRID_SET_UP(false)
	{
		assert (max_b >= 0);
		assert (min_weight > 0.0f);
	}

	virtual ~RayTracer() {}

	Vec3f traceRay(Ray & ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit & hit) const;
	Vec3f traceRayStat(Ray & ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit & hit, bool noshadow_flag) const;
	Vec3f traceRayFast(Ray & ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit & hit, bool noshadow_flag, const Grid & sceneGrid, Grid_Rendering_Type type) const;

	// bool prepareGridAcceleration(int nx, int ny, int nz);

	static Vec3f getMirrorDirection(const Vec3f & normal, const Vec3f & incoming);
	static bool getTransmittedDirection(const Vec3f & normal, const Vec3f & incoming, float index_i, float index_t, Vec3f & transmitted);

private:

	SceneParser * sparser;
	int max_bounces;
	float min_weight;

	bool IS_GRID_SET_UP;

};

#endif