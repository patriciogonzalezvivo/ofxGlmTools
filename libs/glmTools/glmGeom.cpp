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

glm::vec3 getCentroid(const std::vector<glm::vec3> &_pts){
    glm::vec3 centroid;
    for (int i = 0; i < _pts.size(); i++) {
        centroid += _pts[i] / (float)_pts.size();
    }
    return centroid;
}

bool lexicalComparison(const glm::vec3& v1, const glm::vec3& v2) {
    if (v1.x > v2.x) return true;
    else if (v1.x < v2.x) return false;
    else if (v1.y > v2.y) return true;
    else return false;
}

bool isRightTurn(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
    // use the cross product to determin if we have a right turn
    return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x)) >= 0;
}

glm::vec3 h1,h2,h3;

std::vector<glm::vec3> getConvexHull(std::vector<glm::vec3> &pts){
    std::vector<glm::vec3> hull;
    
//    std::vector<glm::vec3> pts;
//    pts.assign(_pts.begin(),_pts.end());
    
    if (pts.size() < 3) {
        std::cout << "Error: you need at least three points to calculate the convex hull" << std::endl;
        return hull;
    }
    
    std::sort(pts.begin(), pts.end(), &lexicalComparison);
    
    hull.push_back(pts.at(0));
    hull.push_back(pts.at(1));
    
    int currentPoint = 2;
    int direction = 1;
    
    for (int i=0; i<1000; i++) { //max 1000 tries
        
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
        
        currentPoint+= direction;
        
        if (hull.front()==hull.back()) break;
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

//-------------------------------------------------------------------------------------

//  C++ implementation of Kabsch Algorithm ( http://en.wikipedia.org/wiki/Kabsch_algorithm )
//  from http://boscoh.com/code/rmsd.c
//  More resources http://nghiaho.com/?page_id=671
//

/*
 * setup_rotation()
 *
 *      given two lists of x,y,z coordinates, constructs
 * the correlation R matrix and the E value needed to calculate the
 * least-squares rotation matrix.
 */
void setup_rotation(std::vector<glm::vec3> &_ref_xlist, std::vector<glm::vec3> &_mov_xlist, glm::vec3 &mov_com, glm::vec3 &mov_to_ref, glm::mat3 &R, double* E0){
    
    int i, j, n;
    glm::vec3 ref_com;
    
    /* calculate the centre of mass */
    mov_com = glm::vec3(0.0);
    ref_com = glm::vec3(0.0);
    
    for (n=0; n<_ref_xlist.size(); n++){
        mov_com += _mov_xlist[n];
        ref_com += _ref_xlist[n];
    }
    
    mov_com /= _ref_xlist.size();
    ref_com /= _ref_xlist.size();
    mov_to_ref= ref_com - mov_com;
    
    /* shift mov_xlist and ref_xlist to centre of mass */
    for (n=0; n<_ref_xlist.size(); n++){
        _mov_xlist[n] -= mov_com;
        _ref_xlist[n] -= ref_com;
    }
    
    /* initialize */
    R = glm::mat3(0.0);
    *E0 = 0.0;
    
    for (n=0; n<_ref_xlist.size(); n++){
        /*
         * E0 = 1/2 * sum(over n): y(n)*y(n) + x(n)*x(n)
         */
        for (i=0; i<3; i++)
            *E0 +=  _mov_xlist[n][i] * _mov_xlist[n][i]
            + _ref_xlist[n][i] * _ref_xlist[n][i];
        
        /*
         * correlation matrix R:
         *   R[i,j) = sum(over n): y(n,i) * x(n,j)
         *   where x(n) and y(n) are two vector sets
         */
        for (i=0; i<3; i++){
            for (j=0; j<3; j++)
                R[i][j] += _mov_xlist[n][i] * _ref_xlist[n][j];
        }
    }
    *E0 *= 0.5;
}

#define ROTATE(a,i,j,k,l) { g = a[i][j]; \
h = a[k][l]; \
a[i][j] = g-s*(h+g*tau); \
a[k][l] = h+s*(g-h*tau); }
/*
 * jacobi3
 *
 *    computes eigenval and eigen_vec of a real 3x3
 * symmetric matrix. On output, elements of a that are above
 * the diagonal are destroyed. d[1..3] returns the
 * eigenval of a. v[1..3][1..3] is a matrix whose
 * columns contain, on output, the normalized eigen_vec of
 * a. n_rot returns the number of Jacobi rotations that were required.
 */
int jacobi3(glm::mat3 &a, glm::vec3 &d, glm::mat3 &v, int* n_rot){
    
    int count, k, i, j;
    double tresh, theta, tau, t, sum, s, h, g, c;
    glm::vec3 b, z;
    
    /*Initialize v to the identity matrix.*/
    v = glm::mat3(1.0);
    
    /* Initialize b and d to the diagonal of a */
    for (i=0; i<3; i++)
        b[i] = d[i] = a[i][i];
    
    /* z will accumulate terms */
    z = glm::vec3(0.0);
    *n_rot = 0;
    
    /* 50 tries */
    for (count=0; count<50; count++){
        
        /* sum off-diagonal elements */
        sum = 0.0;
        for (i=0; i<2; i++){
            for (j=i+1; j<3; j++)
                sum += fabs(a[i][j]);
        }
        
        /* if converged to machine underflow */
        if (sum == 0.0)
            return(1);
        
        /* on 1st three sweeps... */
        if (count < 3)
            tresh = sum * 0.2 / 9.0;
        else
            tresh = 0.0;
        
        for (i=0; i<2; i++){
            for (j=i+1; j<3; j++){
                g = 100.0 * fabs(a[i][j]);
                
                /*  after four sweeps, skip the rotation if
                 *   the off-diagonal element is small
                 */
                if ( count > 3  &&  fabs(d[i])+g == fabs(d[i]) &&  fabs(d[j])+g == fabs(d[j]) ){
                    a[i][j] = 0.0;
                } else if (fabs(a[i][j]) > tresh) {
                    h = d[j] - d[i];
                    
                    if (fabs(h)+g == fabs(h)) {
                        t = a[i][j] / h;
                    } else {
                        theta = 0.5 * h / (a[i][j]);
                        t = 1.0 / ( fabs(theta) +
                                   (double)sqrt(1.0 + theta*theta) );
                        if (theta < 0.0)
                            t = -t;
                    }
                    
                    c = 1.0 / (double) sqrt(1 + t*t);
                    s = t * c;
                    tau = s / (1.0 + c);
                    h = t * a[i][j];
                    
                    z[i] -= h;
                    z[j] += h;
                    d[i] -= h;
                    d[j] += h;
                    
                    a[i][j] = 0.0;
                    
                    for (k=0; k<=i-1; k++)
                        ROTATE(a, k, i, k, j)
                        
                        for (k=i+1; k<=j-1; k++)
                            ROTATE(a, i, k, k, j)
                            
                            for (k=j+1; k<3; k++)
                                ROTATE(a, i, k, j, k)
                                
                                for (k=0; k<3; k++)
                                    ROTATE(v, k, i, k, j)
                                    
                                    ++(*n_rot);
                }
            }
        }
        
        b += z;
        d = b;
        z = glm::vec3(0.0);
        
        //        for (i=0; i<3; i++){
        //            b[i] += z[i];
        //            d[i] = b[i];
        //            z[i] = 0.0;
        //        }
    }
    
    printf("Too many iterations in jacobi3\n");
    return (0);
}



/*
 * diagonalize_symmetric
 *
 *    Diagonalize a 3x3 matrix & sort eigenval by size
 */
int diagonalize_symmetric(glm::mat3 &matrix, glm::mat3 &eigen_vec, glm::vec3 &eigenval){
    int n_rot, i, j, k;
    glm::mat3 vec;
    double val;
    
    if (!jacobi3(matrix, eigenval, vec, &n_rot)){
        printf("convergence failed\n");
        return (0);
    }
    
    /* sort solutions by eigenval */
    for (i=0; i<3; i++){
        k = i;
        val = eigenval[i];
        
        for (j=i+1; j<3; j++)
            if (eigenval[j] >= val){
                k = j;
                val = eigenval[k];
            }
        
        if (k != i){
            eigenval[k] = eigenval[i];
            eigenval[i] = val;
            for (j=0; j<3; j++){
                val = vec[j][i];
                vec[j][i] = vec[j][k];
                vec[j][k] = val;
            }
        }
    }
    
    /* transpose such that first index refers to solution index */
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            eigen_vec[i][j] = vec[j][i];
    
    return (1);
}



/*
 * calculate_rotation_matrix()
 *
 *   calculates the rotation matrix U and the
 * rmsd from the R matrix and E0:
 */

int calculate_rotation_matrix(glm::mat3 &R, glm::mat3 &U, double E0, double* residual){
    int i, j, k;
    glm::mat3 Rt, RtR;
    glm::mat3 left_eigenvec, right_eigenvec;
    glm::vec3 eigenval;
    glm::vec3 v;
    double sigma;
    
    /* build Rt, transpose of R  */
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            Rt[i][j] = R[j][i];
    
    /* make symmetric RtR = Rt X R */
    for (i=0; i<3; i++)
        for (j=0; j<3; j++){
            RtR[i][j] = 0.0;
            for (k = 0; k<3; k++)
                RtR[i][j] += Rt[k][i] * R[j][k];
        }
    
    if (!diagonalize_symmetric(RtR, right_eigenvec, eigenval))
        return(0);
    
    /* right_eigenvec's should be an orthogonal system but could be left
     * or right-handed. Let's force into right-handed system.
     */
    //    cross(&right_eigenvec[2][0], &right_eigenvec[0][0], &right_eigenvec[1][0]);
    right_eigenvec[2] = glm::cross(right_eigenvec[0], right_eigenvec[1]);
    
    /* From the Kabsch algorithm, the eigenvec's of RtR
     * are identical to the right_eigenvec's of R.
     * This means that left_eigenvec = R x right_eigenvec
     */
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
            left_eigenvec[i][j] = glm::dot(right_eigenvec[i], Rt[j]);
    
    for (i=0; i<3; i++)
        glm::normalize( left_eigenvec[i] );
    
    /*
     * Force left_eigenvec[2] to be orthogonal to the other vectors.
     * First check if the rotational matrices generated from the
     * orthogonal eigenvectors are in a right-handed or left-handed
     * co-ordinate system - given by sigma. Sigma is needed to
     * resolve this ambiguity in calculating the RMSD.
     */
    v = glm::cross(left_eigenvec[0], left_eigenvec[1]);
    if (glm::dot(v, left_eigenvec[2]) < 0.0)
        sigma = -1.0;
    else
        sigma = 1.0;
    
    //    for (i=0; i<3; i++)
    //        left_eigenvec[2][i] = v[i];
    
    left_eigenvec[2] = v;
    
    /* calc optimal rotation matrix U that minimises residual */
    for (i=0;i<3; i++)
        for (j=0; j<3; j++){
            U[i][j] = 0.0;
            for (k=0; k<3; k++)
                U[i][j] += left_eigenvec[k][i] * right_eigenvec[k][j];
        }
    
    *residual = E0 - (double) sqrt(fabs(eigenval[0])) - (double) sqrt(fabs(eigenval[1])) - sigma * (double) sqrt(fabs(eigenval[2]));
    
    return (1);
}



void calculate_rotation_rmsd(std::vector<glm::vec3> &_ref_xlist, std::vector<glm::vec3> &_mov_xlist, glm::vec3 &mov_com, glm::vec3 &mov_to_ref, glm::mat3 &U, double* rmsd){
    double Eo, residual;
    glm::mat3 R;
    
    setup_rotation(_ref_xlist, _mov_xlist, mov_com, mov_to_ref, R, &Eo);
    
    calculate_rotation_matrix(R, U, Eo, &residual);
    
    residual = fabs(residual); /* avoids the awkward case of -0.0 */
    *rmsd = sqrt( fabs((double) (residual)*2.0/((double)_ref_xlist.size())) );
}



/*
 * Fast calculation of rmsd w/o calculating a rotation matrix.
 *
 *   Chris Saunders 11/2002 - Fast rmsd calculation by the method of
 * Kabsch 1978, where the required eigenvalues are found by an
 * analytical, rather than iterative, method to save time.
 * The cubic factorization used to accomplish this only produces
 * stable eigenvalues for the transpose(R]*R matrix of a typical
 * protein after the whole matrix has been normalized. Note that
 * the normalization process used here is completely empirical
 * and that, at the present time, there are **no checks** or
 * warnings on the quality of the (potentially unstable) cubic
 * factorization.
 *
 */
#define PI 3.14159265358979323846
void fast_rmsd(std::vector<glm::vec3> _ref_xlist, std::vector<glm::vec3> _mov_xlist, double* rmsd){
    glm::mat3 R;
    double d0,d1,d2,e0,e1,f0;
    double omega;
    glm::vec3 mov_com;
    glm::vec3 mov_to_ref;
    
    /* cubic roots */
    double r1,r2,r3;
    double rlow;
    
    glm::vec3 v;
    double Eo, residual;
    
    setup_rotation(_ref_xlist, _mov_xlist, mov_com, mov_to_ref, R, &Eo);
    
    /*
     * check if the determinant is greater than 0 to
     * see if R produces a right-handed or left-handed
     * co-ordinate system.
     */
    v = glm::cross(R[1], R[2]);
    if (glm::dot(R[0], v) > 0.0)
        omega = 1.0;
    else
        omega = -1.0;
    
    /*
     * get elements we need from tran(R) x R
     *  (funky matrix naming relic of first attempt using pivots)
     *          matrix = d0 e0 f0
     *                      d1 e1
     *                         d2
     * divide matrix by d0, so that cubic root algorithm can handle it
     */
    
    d0 =  R[0][0]*R[0][0] + R[1][0]*R[1][0] + R[2][0]*R[2][0];
    
    d1 = (R[0][1]*R[0][1] + R[1][1]*R[1][1] + R[2][1]*R[2][1])/d0;
    d2 = (R[0][2]*R[0][2] + R[1][2]*R[1][2] + R[2][2]*R[2][2])/d0;
    
    e0 = (R[0][0]*R[0][1] + R[1][0]*R[1][1] + R[2][0]*R[2][1])/d0;
    e1 = (R[0][1]*R[0][2] + R[1][1]*R[1][2] + R[2][1]*R[2][2])/d0;
    
    f0 = (R[0][0]*R[0][2] + R[1][0]*R[1][2] + R[2][0]*R[2][2])/d0;
    
    /* cubic roots */
    {
        double B, C, D, q, q3, r, theta;
        /*
         * solving for eigenvalues as det(A-I*lambda) = 0
         * yeilds the values below corresponding to:
         * lambda**3 + B*lambda**2 + C*lambda + D = 0
         *   (given that d0=1.)
         */
        B = -1.0 - d1 - d2;
        C = d1 + d2 + d1*d2 - e0*e0 - f0*f0 - e1*e1;
        D = e0*e0*d2 + e1*e1 + f0*f0*d1 - d1*d2 - 2*e0*f0*e1;
        
        /* cubic root method of Viete with all safety belts off */
        q = (B*B - 3.0*C) / 9.0;
        q3 = q*q*q;
        r = (2.0*B*B*B - 9.0*B*C + 27.0*D) / 54.0;
        theta = acos(r/sqrt(q3));
        r1 = r2 = r3 = -2.0*sqrt(q);
        r1 *= cos(theta/3.0);
        r2 *= cos((theta + 2.0*PI) / 3.0);
        r3 *= cos((theta - 2.0*PI) / 3.0);
        r1 -= B / 3.0;
        r2 -= B / 3.0;
        r3 -= B / 3.0;
    }
    
    /* undo the d0 norm to get eigenvalues */
    r1 = r1*d0;
    r2 = r2*d0;
    r3 = r3*d0;
    
    /* set rlow to lowest eigenval; set other two to r1,r2 */
    if (r3<r1 && r3<r2){
        rlow = r3;
    } else if (r2<r1 && r2<r3) {
        rlow = r2;
        r2 = r3;
    } else {
        rlow = r1;
        r1 = r3;
    }
    
    residual = Eo - sqrt(r1) - sqrt(r2) - omega*sqrt(rlow);
    *rmsd = sqrt( (double) residual*2.0 / ((double) _ref_xlist.size()) );
}