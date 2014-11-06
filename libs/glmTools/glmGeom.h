//
//  glmGeom.h
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include <vector>
#include <math.h>
#include <iostream>

#include "glm.hpp"
#include "matrix_transform.hpp"

#ifndef PI
#define PI       3.14159265358979323846
#endif

#ifndef TWO_PI
#define TWO_PI   6.28318530717958647693
#endif

#ifndef FOUR_PI
#define FOUR_PI 12.56637061435917295385
#endif

#ifndef HALF_PI
#define HALF_PI  1.57079632679489661923
#endif

#ifndef QUARTER_PI
#define QUARTER_PI 0.785398163
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD (PI/180.0)
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG (180.0/PI)
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

#ifndef ABS
#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

const glm::vec3 UP_NORMAL = glm::vec3(0.0f, 0.0f, 1.0f);

void wrapRad(double &_angle);
float mapValue(const float &value, const float &inputMin, const float &inputMax, const float &outputMin, const float &outputMax, bool clamp = true);
float lerpValue(const float &_start, const float &_stop, float const &_amt);

glm::vec3 getCentroid(std::vector<glm::vec3> &_pts);
std::vector<glm::vec3> getConvexHull(std::vector<glm::vec3> &_pts);

bool lineSegmentIntersection(const glm::vec3 &_line1Start, const glm::vec3 &_line1End,
                             const glm::vec3 &_line2Start, const glm::vec3 &_line2End,
                             glm::vec3 &_intersection );

//  C++ implementation of Kabsch Algorithm ( http://en.wikipedia.org/wiki/Kabsch_algorithm )
//  from http://boscoh.com/code/rmsd.c
//  More resources http://nghiaho.com/?page_id=671
void calculate_rotation_rmsd(std::vector<glm::vec3> _ref_xlist, std::vector<glm::vec3> _mov_xlist, glm::vec3 &mov_com, glm::vec3 &mov_to_ref, glm::mat3 &U, double* rmsd);
void fast_rmsd(std::vector<glm::vec3> _ref_xlist, std::vector<glm::vec3> _mov_xlist, double* rmsd);