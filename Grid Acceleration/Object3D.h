#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include "Vector.h"
#include "Material.h"

#define BOUNDINGBOX_EPSILON 1e-3

class Grid;
class Ray;
class Hit;

// =================================================
// =================================================

class Object3D
{

public:

	Object3D(Material * m = NULL) : material(m) {}
	virtual ~Object3D() { material = NULL; }

	const Material * getMaterial() const { return material; }
	void setMaterial(Material * mat) { material = mat; } 

	// Pure Virtual Function
	virtual bool calcBoundingBox(Vec3f & minCoor, Vec3f & maxCoor) const = 0;
	virtual bool intersect(const Ray & r, Hit & h, float tmin) = 0;
	virtual void registIntoGrid(Grid & grid) const;

	virtual std::string toString()
	{
		string header = "Object3D:\n";

		if (material != NULL) {
			return header + material->toString();
		} else {
			return header;
		}
	}

protected:
	
	Material * material;

};

#endif