//
//  glmTools.cpp
//
//  Created by Patricio Gonzalez Vivo on 9/4/14.
//
//

#include "glmGeom.h"

void wrapRad(double &_angle){
    if (_angle < -PI) _angle += PI*2.;
    if (_angle > PI) _angle -= PI*2.;
}

float mapValue(const float &value, const float &inputMin, const float &inputMax, const float &outputMin, const float &outputMax, bool clamp ) {
    if (fabs(inputMin - inputMax) < FLT_EPSILON){
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
        
        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
}

float lerpValue(const float &_start, const float &_stop, float const &_amt) {
    if(_start!=_stop){
        return _start + (_stop-_start) * _amt;
    } else {
        return _stop;
    }
}

float getArea(const std::vector<glm::vec3> &_pts){
    float area;
    
    for(int i=0;i<(int)_pts.size()-1;i++){
        area += _pts[i].x * _pts[i+1].y - _pts[i+1].x * _pts[i].y;
    }
    area += _pts[_pts.size()-1].x * _pts[0].y - _pts[0].x * _pts[_pts.size()-1].y;
    area *= 0.5;
    
    return area;
}

glm::vec3 getCentroid(const std::vector<glm::vec3> &_pts){
    glm::vec3 centroid;
    for (int i = 0; i < _pts.size(); i++) {
        centroid += _pts[i] / (float)_pts.size();
    }
    return centroid;
}

std::vector<glm::vec3> getSimplify(const std::vector<glm::vec3> &_pts){
    
}

bool lexicalComparison(const glm::vec3& v1, const glm::vec3& v2) {
    if (v1.x > v2.x) return true;
    else if (v1.x < v2.x) return false;
    else if (v1.y > v2.y) return true;
    else return false;
}

bool isRightTurn(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
    // use the cross product to determin if we have a right turn
    return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) > 0;
}

std::vector<glm::vec3> getConvexHull(const std::vector<glm::vec3> &_pts){
    std::vector<glm::vec3> pts;
    pts.assign(_pts.begin(),_pts.end());
    
    return getConvexHull(pts);
}

std::vector<glm::vec3> getConvexHull(std::vector<glm::vec3> &pts){
    std::vector<glm::vec3> hull;
    glm::vec3 h1,h2,h3;
    
    if (pts.size() < 3) {
        std::cout << "Error: you need at least three points to calculate the convex hull" << std::endl;
        return hull;
    }
    
    std::sort(pts.begin(), pts.end(), &lexicalComparison);
    
    hull.push_back(pts.at(0));
    hull.push_back(pts.at(1));
    
    int currentPoint = 2;
    int direction = 1;
    
    for (int i=0; i<2000; i++) { //max 1000 tries
        
        hull.push_back(pts.at(currentPoint));
        
        // look at the turn direction in the last three points
        h1 = hull.at(hull.size()-3);
        h2 = hull.at(hull.size()-2);
        h3 = hull.at(hull.size()-1);
        
        // while there are more than two points in the hull
        // and the last three points do not make a right turn
        while (!isRightTurn(h1, h2, h3) && hull.size() > 2) {
            
            // remove the middle of the last three points
            hull.erase(hull.end() - 2);
            
            if (hull.size() >= 3) {
                h1 = hull.at(hull.size()-3);
            }
            h2 = hull.at(hull.size()-2);
            h3 = hull.at(hull.size()-1);
        }
        
        // going through left-to-right calculates the top hull
        // when we get to the end, we reverse direction
        // and go back again right-to-left to calculate the bottom hull
        if (currentPoint == pts.size() -1 || currentPoint == 0) {
            direction = direction * -1;
        }
        
        currentPoint+=direction;
        
        if (hull.front()==hull.back()) {
            if(currentPoint == 3 && direction == 1){
                currentPoint = 4;
            } else {
                break;
            }
        }
    }
    
    return hull;
}

bool lineSegmentIntersection(const glm::vec3 &_line1Start, const glm::vec3 &_line1End,
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

//This is for polygon/contour simplification - we use it to reduce the number of m_points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
    glm::vec3 P0, P1;
} Segment;

#define norm2(v)   glm::dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

//--------------------------------------------------
static void simplifyDP(float tol, glm::vec3* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;
    
    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    glm::vec3 u;
    u				= S.P1 - S.P0;   // segment direction vector
    float  cu		= glm::dot(u,u);     // segment length squared
    
    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    glm::vec3  w;
    glm::vec3   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared
    
    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = glm::dot(w,u);
        if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
        else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2) continue;
        
        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}

std::vector<glm::vec3> getSimplify(std::vector<glm::vec3> &_pts, float _tolerance){
    std::vector<glm::vec3> rta;
    rta.assign(_pts.begin(),_pts.end());
    simplify(rta);
    return rta;
}

void simplify(std::vector<glm::vec3> &_pts, float _tolerance){
    
    if(_pts.size() < 2) return;
    
    int n = _pts.size();
    
    if(n == 0) {
        return;
    }
    
    std::vector<glm::vec3> sV;
    sV.resize(n);
    
    int    i, k, m, pv;            // misc counters
    float  tol2 = _tolerance * _tolerance;       // tolerance squared
    std::vector<glm::vec3> vt;
    std::vector<int> mk;
    vt.resize(n);
    mk.resize(n,0);
    
    
    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = _pts[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(_pts[i], _pts[pv]) < tol2) continue;
        
        vt[k++] = _pts[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = _pts[n-1];      // finish at the end
    
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( _tolerance, &vt[0], 0, k-1, &mk[0] );
    
    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }
    
    //get rid of the unused points
    if( m < (int)sV.size() ){
        _pts.assign( sV.begin(),sV.begin()+m );
    }else{
        _pts = sV;
    }
    
    
}