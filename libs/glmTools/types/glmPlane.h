//
//  glmPlane.h
//
//  Created by Patricio Gonzalez Vivo on 11/12/14.
//
//

#pragma once

#include "glm.hpp"

class glmPlane {
public:
    glmPlane():m_p0(0.0,0.0,0.0),m_normal(0.0,0.0,1.0),m_d(0.0){};
    
    glmPlane(glm::vec3 _p0, glm::vec3 _normal){
        set(_normal, _p0);
    };
    
    void set(glm::vec3 _p0, glm::vec3 _p1, glm::vec3 _p2){
        glm::vec3 seg0, seg1;
        seg0 = _p1-_p0;
        seg1 = _p2-_p0;
        m_normal = glm::normalize(glm::cross(seg0,seg1));
        set(m_normal,_p0);
    }
    
    void set(glm::vec3 _p0, glm::vec3 _normal){
        m_p0=_p0;
        setNormal(_normal);
    }
    
    void setNormal(glm::vec3 _normal){
        m_normal = glm::normalize(_normal);
        updateDCoeff();
    }
    
    void setPosition(glm::vec3 _p0){
        m_p0=_p0;
        updateDCoeff();
    }
    
    glm::vec3 getP0() const {return m_p0;};
    glm::vec3 getNormal() const {return m_normal;};
    float getDCoeff() const {return m_d;};
    
protected:
    float updateDCoeff(){
        m_d=glm::dot(m_normal,m_p0);
    }
    
    glm::vec3 m_p0;
    glm::vec3 m_normal;
    float m_d;
};