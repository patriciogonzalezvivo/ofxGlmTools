//
//  glmIntersection.cpp
//  05_Slicer
//
//  Created by Patricio Gonzalez Vivo on 11/12/14.
//
//

#include "glmIntersection.h"

#include "glm/gtx/norm.hpp"

bool LineSegmentIntersection(const glm::vec3 &_line1Start, const glm::vec3 &_line1End,
                             const glm::vec3 &_line2Start, const glm::vec3 &_line2End,
                             glm::vec3 &_intersection ){
    glm::vec3 diffLA, diffLB;
    double compareA, compareB;
    diffLA = _line1End - _line1Start;
    diffLB = _line2End - _line2Start;
    compareA = diffLA.x*_line1Start.y - diffLA.y*_line1Start.x;
    compareB = diffLB.x*_line2Start.y - diffLB.y*_line2Start.x;
    if (
        (
         ( ( diffLA.x*_line2Start.y - diffLA.y*_line2Start.x ) < compareA ) ^
         ( ( diffLA.x*_line2End.y - diffLA.y*_line2End.x ) < compareA )
         )
        &&
        (
         ( ( diffLB.x*_line1Start.y - diffLB.y*_line1Start.x ) < compareB ) ^
         ( ( diffLB.x*_line1End.y - diffLB.y*_line1End.x) < compareB )
         )
        )
    {
        double lDetDivInv = 1 / ((diffLA.x*diffLB.y) - (diffLA.y*diffLB.x));
        _intersection.x =  -((diffLA.x*compareB) - (compareA*diffLB.x)) * lDetDivInv ;
        _intersection.y =  -((diffLA.y*compareB) - (compareA*diffLB.y)) * lDetDivInv ;
        
        return true;
    }
    
    return false;
};



bool containsValue(std::vector<glm::vec3> *_points, const glm::vec3 &_point){
    for(int i = 0; i < _points->size(); i++){
        if((_points->at(i)-_point).length()<EPS ){
            return true;
        }
    }
    return false;
}

glm::vec3 LinePlaneIntersectionFast(const glm::vec3 &_p0, const glm::vec3 &_p1, const glmPlane &_plane){
    glm::vec3 vec(_p1-_p0);
    float denom = glm::dot(_plane.getNormal(),vec);
    float u = glm::dot(_plane.getNormal(),_plane.getP0()-_p0)/denom;
    return _p0+vec*u;
}

/************** Ray ************/
IntersectionData RayPlaneIntersection(glmRay&ray, glmPlane &plane){
    IntersectionData idata;
    
    float u=0;
    float denom = glm::dot(plane.getNormal(),ray.getVec());
    
    // check if ray is paralles to plane:
    if (fabs(denom) > EPS) {
        u= glm::dot(plane.getNormal(),plane.getP0()-ray.getP0())/denom;
        idata.isIntersection=true;
        idata.pos = ray.getP0()+ray.getVec()*u ;
        return idata;
    }else{
        idata.isIntersection=false;
        return idata;
    }
    
}

IntersectionData RayTriangleIntersection(glmTriangle& triangle, glmRay& ray){
    
    // Copied from ofxRayTriangleIntersection
    
    IntersectionData idata;
    glm::vec3 rayStart=ray.getP0();
    glm::vec3 rayDir=ray.getVec();
    glm::vec3 triNorm = triangle.getNormal();
    float vn = glm::dot(rayDir,triNorm);
    
    glm::vec3 aa = rayStart - triangle.getP0();
    float xpn = glm::dot(aa,triNorm);
    float distance = -xpn / vn;
    
    if (distance < 0) return idata; // behind ray origin. fail
    
    glm::vec3 hitPos(rayDir.x * distance + rayStart.x,
                     rayDir.y * distance + rayStart.y,
                     rayDir.z * distance + rayStart.z);
    
    glm::vec3 hit00 = hitPos - triangle.getP0();
    glm::vec3 hit01 = triangle.getP1() - triangle.getP0();
    glm::vec3 cross0 = glm::cross(hit00,hit01);
    
    if (glm::dot(cross0,triNorm) > EPS)
        return idata; // not in triangle. fail
    
    glm::vec3 hit10 = hitPos - triangle.getP1();
    glm::vec3 hit11 = triangle.getP2() - triangle.getP1();
    glm::vec3 cross1 = glm::cross(hit10,hit11);
    
    if (glm::dot(cross1,triNorm) > EPS)
        return idata; // not in triangle. fail
    
    glm::vec3 hit20 = hitPos - triangle.getP2();
    glm::vec3 hit21 = triangle.getP0() - triangle.getP2();
    glm::vec3 cross2 = glm::cross(hit20,hit21);
    
    if (glm::dot(cross2,triNorm) > EPS)
        return idata;; // not in triangle. fail
    
    idata.isIntersection=true;
    idata.pos=hitPos;
    idata.dist=distance;
    
}

float PointPlaneDistance(const glm::vec3 &_point, const glmPlane &_plane){
    return glm::dot(_plane.getNormal(),_point - _plane.getP0());
}

/************** Line  ************/

IntersectionData LinePlaneIntersection(const glmLine &_line, const glmPlane &_plane){
    IntersectionData idata;
    
    float dist1 = PointPlaneDistance(_line.getP0(), _plane);
    float dist2 = PointPlaneDistance(_line.getP1(), _plane);
    int pos1 = signValue(dist1);
    int pos2 = signValue(dist2);
    
    if(pos1==pos2){
        idata.isIntersection=false;
        return idata;
    };
    
    float u=0;
    float denom = glm::dot(_plane.getNormal(),_line.getVec());
    
    // check if ray is paralles to plane:
    
    if (fabs(denom) > EPS) {
        u= glm::dot(_plane.getNormal(),_plane.getP0()-_line.getP0())/denom;
        // check if intersection is within line-segment:
        if(u>1.0 || u<0){
            idata.isIntersection=false;
            return idata;
        }
        idata.isIntersection=true;
        idata.pos = _line.getP0()+_line.getVec()*u;
        return idata;
    }else{
        idata.isIntersection=false;
        return idata;
    }
}



IntersectionData LineLineIntersection(const glmLine& _line1, const glmLine& _line2){
    IntersectionData idata;
    
    glm::vec3 p13,p43,p21;
    
    glm::vec3 p1 = _line1.getP0();
    glm::vec3 p2 = _line1.getP1();
    glm::vec3 p3 = _line2.getP0();
    glm::vec3 p4 = _line2.getP1();
    
    
    double d1343,d4321,d1321,d4343,d2121;
    double numer,denom;
    double mua, mub;
    glm::vec3 pa, pb;
    
    p13.x = p1.x - p3.x;
    p13.y = p1.y - p3.y;
    p13.z = p1.z - p3.z;
    p43.x = p4.x - p3.x;
    p43.y = p4.y - p3.y;
    p43.z = p4.z - p3.z;
    if (ABS(p43.x) < EPS && ABS(p43.y) < EPS && ABS(p43.z) < EPS){
        idata.isIntersection=false;
        return idata;
    }
    
    p21.x = p2.x - p1.x;
    p21.y = p2.y - p1.y;
    p21.z = p2.z - p1.z;
    
    if (ABS(p21.x) < EPS && ABS(p21.y) < EPS && ABS(p21.z) < EPS){
        idata.isIntersection=false;
        return idata;
    }
    
    d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
    d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
    d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
    d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
    d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
    
    denom = d2121 * d4343 - d4321 * d4321;
    if (ABS(denom) < EPS){
        idata.isIntersection=false;
        return idata;
    }
    
    numer = d1343 * d4321 - d1321 * d4343;
    mua = numer / denom;
    mub = (d1343 + d4321 * (mua)) / d4343;
    
    pa.x = p1.x + mua * p21.x;
    pa.y = p1.y + mua * p21.y;
    pa.z = p1.z + mua * p21.z;
    pb.x = p3.x + mub * p43.x;
    pb.y = p3.y + mub * p43.y;
    pb.z = p3.z + mub * p43.z;
    
    idata.isIntersection=true;
    idata.pos = pa;
    idata.dir = pb-pa;
    return idata;
    
}


IntersectionData PointLineDistance(const glm::vec3 &_point, const glmLine &_line){
    float u;
    glm::vec3 dist;
    IntersectionData idata;
    
    u = ( ( ( _point.x - _line.getP0().x ) * ( _line.getVec().x ) +
           ( ( _point.y - _line.getP0().y ) * ( _line.getVec().y) ) +
           ( ( _point.z - _line.getP0().z ) * ( _line.getVec().z) ) ) /
         ( glm::length2( _line.getVec() ) ) );
    
    if( u < 0.0f || u > 1.0f ){
        idata.isIntersection=false;
        return idata;
    }
    
    idata.isIntersection=true;
    idata.pos.x = _line.getP0().x + u * ( _line.getVec().x );
    idata.pos.y = _line.getP0().y + u * ( _line.getVec().y );
    idata.pos.z = _line.getP0().z + u * ( _line.getVec().z );
    idata.dir = idata.pos - _point;
    
    return idata;
    
}





/************** Plane  ************/


IntersectionData PlanePlaneIntersection(const glmPlane &_plane1, const glmPlane &_plane2){
    IntersectionData idata;
    
    glm::vec3 n1 = _plane1.getNormal();
    glm::vec3 n2 = _plane2.getNormal();
    float d1 = _plane1.getDCoeff();
    float d2 = _plane2.getDCoeff();
    
    // Check if planes are parallel, if so return false:
    glm::vec3 dir= glm::cross(_plane1.getNormal(),_plane2.getNormal());
    
    if(dir.length() < EPS){
        idata.isIntersection=false;
        return idata;
    }
    
    idata.isIntersection=true;
    // Direction of intersection is the cross product of the two normals:
    dir = glm::normalize(dir);
    
    // Thank you Toxi!
    float offDiagonal = glm::dot(n1,n2);
    double det = 1.0 / (1 - offDiagonal * offDiagonal);
    double a = (d1 - d2 * offDiagonal) * det;
    double b = (d2 - d1 * offDiagonal) * det;
    glm::vec3 anchor = getScaled(n1,(float)a)+ getScaled(n2,(float)b);
    
    idata.pos=anchor;
    idata.dir=dir;
    return idata;
}



IntersectionData PlanePlanePlaneIntersection(const glmPlane &_plane1, const glmPlane &_plane2, const glmPlane &_plane3){

    IntersectionData idata;
    
    float d1 = _plane1.getDCoeff();
    float d2 = _plane2.getDCoeff();
    float d3 = _plane3.getDCoeff();
    
    glm::vec3 n1 = _plane1.getNormal();
    glm::vec3 n2 = _plane2.getNormal();
    glm::vec3 n3 = _plane3.getNormal();
    
    glm::vec3 cross1 = glm::cross(n2,n3);
    glm::vec3 cross2 = glm::cross(n3,n1);
    glm::vec3 cross3 = glm::cross(n1,n2);
    
    float det = glm::dot(n1,cross1);
    
    if(fabs(det)<EPS){
        idata.isIntersection=false;
        return idata;
    }
    
    idata.isIntersection=true;
    idata.pos = (d1*cross1+d2*cross2+d3*cross3)/det;
    
    return idata;
}

IntersectionData PlaneTriangleIntersection(const glmPlane &_plane, const glmTriangle &_triangle){
    
    IntersectionData idata;
    glm::vec3 tp0 = _triangle.getP0();
    glm::vec3 tp1 = _triangle.getP1();
    glm::vec3 tp2 = _triangle.getP2();
    
    float dist1 = PointPlaneDistance(tp0, _plane);
    float dist2 = PointPlaneDistance(tp1, _plane);
    float dist3 = PointPlaneDistance(tp2, _plane);
    
    int pos1 = signValue(dist1);
    int pos2 = signValue(dist2);
    int pos3 = signValue(dist3);
    
    if(pos1==pos2 && pos1==pos3){
        idata.isIntersection=false;
        return idata;
    };
    
    std::vector<glm::vec3>ispoints;
    bool bintersects=false;
    glm::vec3 ip;
    
    if(pos1!=pos2){
        ip = LinePlaneIntersectionFast(tp0, tp1, _plane);
        if(!containsValue(&ispoints, ip)){
            ispoints.push_back(ip);
        };
    }
    if(pos2!=pos3){
        ip = LinePlaneIntersectionFast(tp1, tp2, _plane);
        if(!containsValue(&ispoints, ip)){
            ispoints.push_back(ip);
        };
    }
    if(pos3!=pos1){
        ip = LinePlaneIntersectionFast(tp2, tp0, _plane);
        if(!containsValue(&ispoints, ip)){
            ispoints.push_back(ip);
        };
    }
    
    idata.isIntersection=true;
    idata.pos = ispoints.at(0);
    
    if(ispoints.size()==2){
        idata.dir = ispoints.at(1);
        idata.dir-=idata.pos;
        idata.dist=idata.dir.length();
    }
    
    return idata;
}