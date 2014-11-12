//
//  glmLine.h
//
//  Created by Patricio Gonzalez Vivo on 11/12/14.
//
//

#pragma once 

#include "glm.hpp"

class glmLine{
public:
    glmLine():m_p0(0,0,0),m_p1(100,0,0),m_vec(100,0,0){};
    glmLine(glm::vec3 _p0, glm::vec3 _p1){
        set(_p0,_p1);
    };
    
    void set(glm::vec3 _p0, glm::vec3 _p1){
        m_p0=_p0;
        m_p1=_p1;
        m_vec = m_p1 - m_p0;
    }
    
    float magintude() const {return glm::length(m_vec);}
    
    glm::vec3 getP0() const {return m_p0;}
    glm::vec3 getP1() const {return m_p1;}
    glm::vec3 getVec() const {return m_vec;}
    
private:
    glm::vec3 m_p0;
    glm::vec3 m_p1;
    glm::vec3 m_vec;
    
};