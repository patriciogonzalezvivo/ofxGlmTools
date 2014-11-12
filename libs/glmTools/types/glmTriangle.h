//
//  glmTriangle.h
//
//  Created by Patricio Gonzalez Vivo on 11/12/14.
//
//

#pragma once

#include "glmLine.h"

class glmTriangle{
public :
    
    glmTriangle(){};
    glmTriangle(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2){
        set(_p0,_p1, _p2);
    }
    
    void set(const glm::vec3 &_p0, const glm::vec3 &_p1, const glm::vec3 &_p2){
        m_p0 = _p0;
        m_p1 = _p1;
        m_p2 = _p2;
        m_seg0.set(m_p0, m_p1);
        m_seg1.set(m_p1, m_p2);
        m_seg2.set(m_p2, m_p0);
        m_segments[0] = &m_seg0;
        m_segments[1] = &m_seg1;
        m_segments[2] = &m_seg2;
        computeNormal();
    }
    
    bool containsPoint(const glm::vec3 &_p){
        glm::vec3 v0 = m_p2 - m_p0;
        glm::vec3 v1 = m_p1 - m_p0;
        glm::vec3 v2 = _p - m_p0;
        
        // Compute dot products
        float dot00 = glm::dot(v0,v0);
        float dot01 = glm::dot(v0,v1);
        float dot02 = glm::dot(v0,v2);
        float dot11 = glm::dot(v1,v1);
        float dot12 = glm::dot(v1,v2);
        
        // Compute barycentric coordinates
        float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
        
        // Check if point is in triangle
        return (u >= 0.0) && (v >= 0.0) && (u + v <= 1.0);
    }
    
    // Getters:
    glm::vec3 getP0() const {return m_p0;}
    glm::vec3 getP1() const {return m_p1;}
    glm::vec3 getP2() const {return m_p2;}
    glmLine getSeg0() const {return m_seg0;}
    glmLine getSeg1() const {return m_seg1;}
    glmLine getSeg2() const {return m_seg2;}
    glm::vec3 getNormal() const {return m_normal;}
    
private:
    glm::vec3 computeNormal(){
        m_normal = glm::normalize(glm::cross(m_seg2.getVec(),m_seg0.getVec()));
        return m_normal;
    }
    
    glm::vec3 m_p0;
    glm::vec3 m_p1;
    glm::vec3 m_p2;
    
    glm::vec3 m_normal;
    
    glmLine m_seg0;
    glmLine m_seg1;
    glmLine m_seg2;
    
    glmLine *m_segments[3];
};