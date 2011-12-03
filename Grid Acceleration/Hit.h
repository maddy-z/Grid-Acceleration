#ifndef _HIT_H_
#define _HIT_H_

#include "Global.h"

#include "Vector.h"
#include "Ray.h"
#include "Material.h"

// =========================================================
// =========================================================

class Hit

{

public:

	// CONSTRUCTOR & DESTRUCTOR
	Hit() { material = NULL; }
	Hit(float _t, Material * m, Vec3f n) { 
		t = _t; material = m; normal = n; 
	}
	
	Hit(const Hit & h) { 
		t = h.t; 
		material = h.material;
		normal = h.normal; 
		intersectionPoint = h.intersectionPoint;
	}

	virtual ~Hit() {}

	// ACCESSORS
	float getT() const { return t; }
	Material * getMaterial() const { return material; }
	Vec3f getNormal() const { return normal; }
	Vec3f getIntersectionPoint() const { return intersectionPoint; }
  
	// MODIFIER
	void set(float _t, Material * m, Vec3f n, const Ray & ray) 
	{
		assert ( _t >= 0.0f );

		t = _t; material = m; normal = n;
		intersectionPoint = ray.pointAtParameter(t);
	}

	virtual std::string toString()
	{
		char str[20];
		sprintf_s(str, "%f", t);

		return string("Hit:\n")
			 + string("\tT = ") + string(str) + NEW_LINE_CHAR
			 + ( material == NULL ? string("\tMaterial = ") : material -> toString() ) + NEW_LINE_CHAR
			 + string("\tNormal = ") + normal.toString() + NEW_LINE_CHAR
			 + string("\tIntersectionPoint = ") + intersectionPoint.toString() + NEW_LINE_CHAR;
	}


private: 

	// REPRESENTATION
	
	float t;
	Material * material;
	Vec3f normal;
	Vec3f intersectionPoint;

};


// ====================================================================
// ====================================================================

#endif
