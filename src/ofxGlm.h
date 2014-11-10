//
//  ofxGlm.h
//
//  A wrapper for OpenFrameworks
//
//  Created by Patricio Gonzalez Vivo on 8/25/14.
//

#pragma once

#include "ofMain.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

inline ofIndexType toOf(const uint16_t &_i){
    return ofIndexType(_i);
}

inline uint16_t toGlm(const ofIndexType &_i){
    return uint16_t(_i);
}

inline ofVec2f toOf(const glm::vec2 &_v){
    return ofVec2f(_v.x,_v.y);
}

inline glm::vec2 toGlm(const ofVec2f &_v){
    return glm::vec2(_v.x,_v.y);
}

inline ofVec3f toOf(const glm::vec3 &_v){
    return ofVec3f(_v.x,_v.y,_v.z);
}

inline glm::vec3 toGlm(const ofVec3f &_v){
    return glm::vec3(_v.x,_v.y,_v.z);
}

inline ofFloatColor toOf(const glm::vec4 &_v){
    return ofFloatColor(_v.x,_v.y,_v.z,_v.w);
}

inline glm::vec4 toGlm(const ofVec4f &_v){
    return glm::vec4(_v.x,_v.y,_v.z,_v.w);
}

inline glm::vec4 toGlm(const ofFloatColor &_v){
    return glm::vec4(_v.r,_v.g,_v.b,_v.a);
}

inline ofQuaternion toOf(const glm::quat &_q){
    return ofQuaternion(_q[0],_q[1],_q[2],_q[3]);
}

inline glm::quat toGlm(const ofQuaternion &_q){
    return glm::quat(_q.x(),_q.y(),_q.z(),_q.w());
}

inline ofMatrix4x4 toOf(const glm::mat4 &_m){
    return ofMatrix4x4(&_m[0][0]);
}

//  NOT tested
//
inline glm::mat4 toGlm(const ofMatrix4x4 &_m){
    return glm::mat4(1.0);
}
