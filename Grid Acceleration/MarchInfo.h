#ifndef _MARCHINFO_H_
#define _MARCHINFO_H_

#include <iostream>
#include <string>
using namespace std;

#include "Vector.h"

// =====================
//  Forward Declaration
// =====================

class Grid;

// =========================
//  Definition of MarchInfo
// =========================

class MarchInfo
{
	friend class Grid;

public:

	MarchInfo(): tmin(0.0f), 
				 i(0), j(0), k(0),
				 sign_x(1), sign_y(1), sign_z(1),
				 next_x(0.0f), next_y(0.0f), next_z(0.0f),
				 dtx(0.0f), dty(0.0f), dtz(0.0f),
				 surfaceNormal(0.0f, 0.0f, 0.0f)
	{}

	virtual ~MarchInfo() {}

	int get_i() const { return i; }
	int get_j() const { return j; }
	int get_k() const { return k; }

	void nextCell();

	virtual std::string toString();

private:

	float tmin;

	int i, j, k;
	int sign_x, sign_y, sign_z;

	float next_x, next_y, next_z;
	float dtx, dty, dtz;

	Vec3f surfaceNormal;

};

#endif