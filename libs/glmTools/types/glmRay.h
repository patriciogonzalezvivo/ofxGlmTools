//
//  glmRay.h
//
//  Created by Patricio Gonzalez Vivo on 11/12/14.
//
//

#include "glmGeom.h"

class glmRay{
public:
    
    glmRay():m_p0(0,0,0),m_vec(1,0,0){};
    glmRay(glm::vec3 _p0, glm::vec3 _vec):m_p0(_p0),m_vec(_vec){};
    
    void set(glm::vec3 _p0, glm::vec3 _vec){
        m_p0=_p0;
        m_vec=_vec;
    }
    
    glm::vec3 getP0() const {return m_p0;}
    glm::vec3 getVec() const { return m_vec;}
    
    glm::vec3 getPointAtDistance(float _dist) const { return m_p0 + getScaled(m_vec,_dist); }
    
private:
    glm::vec3 m_p0;
    glm::vec3 m_vec;
};