#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define	DEFAULT_CHAR_ARRAY_NUMBER 50

extern std::string NEW_LINE_CHAR; 

// =====================
//  Forward Declaration
// =====================

class Vec3f;
class SceneParser;

// ==================
//  Utility Function
// ==================

bool findMinMaxCoor(const SceneParser * p, Vec3f & minCoor, Vec3f & maxCoor);

#endif