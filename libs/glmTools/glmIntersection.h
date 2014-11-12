//
//  glmIntersection.h
//
//  Refactor Matthias Rohrbach code ( 16.09.14 )
//  https://github.com/robandrose/ofxIntersection
//

#pragma once

#include "glmString.h"

#include "glmRay.h"
#include "glmLine.h"
#include "glmPlane.h"
#include "glmTriangle.h"

#define EPS 1.0E-10

// 2D
//
bool LineSegmentIntersection(const glm::vec3 &_line1Start, const glm::vec3 &_line1End,
                             const glm::vec3 &_line2Start, const glm::vec3 &_line2End,
                             glm::vec3 &_intersection );

// 3D
//
class IntersectionData{
public:
    IntersectionData():pos(0,0,0),dir(0,0,0),normal(0,0,0),dist(0),isIntersection(false){};
    
    std::string toString(){
        std::string s="isec: "+ ::toString(isIntersection);
        if(isIntersection){
            s += " at:" + ::toString(pos) + " dir:" + ::toString(dir)+ " dist:" + ::toString(dist) + "normal:" + ::toString(normal);
        }
        return s;
    }
    
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 normal;
    
    float dist;
    bool isIntersection;
};

// Ray
IntersectionData RayPlaneIntersection(const glmRay& ray, const glmPlane& plane);
IntersectionData RayTriangleIntersection(const glmTriangle& triangle, const glmRay& ray);

// Line
glm::vec3 LinePlaneIntersectionFast(glm::vec3& p0, glm::vec3& p1, glmPlane& plane);
IntersectionData LinePlaneIntersection(const glmLine& line, const glmPlane& plane);
IntersectionData LineLineIntersection(const glmLine& line1, const glmLine& line2);
IntersectionData PointLineDistance(glm::vec3& point, glmLine& line);

// Plane
IntersectionData PlanePlaneIntersection(const glmPlane& plane1, const glmPlane& plane2);
IntersectionData PlanePlanePlaneIntersection(const glmPlane& plane1, const glmPlane& plane2, const glmPlane& plane3);

IntersectionData PlaneTriangleIntersection(const glmPlane& plane, const glmTriangle& triangle);
float PointPlaneDistance(const glm::vec3& point, const glmPlane& plane);

// Triangle
IntersectionData TriangleTriangleIntersection(const glmTriangle& triangle1, const glmTriangle& triangle2){};