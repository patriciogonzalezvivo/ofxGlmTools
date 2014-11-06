//
//  glmTransform.h
//
//  Created by Patricio Gonzalez Vivo on 11/5/14.
//
//

#pragma once

#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

//  In glm think on using translate, rotate and scale:
//
//  mat4 Projection = perspective(45.0f, 1.0f, 0.1f, 100.0f);
//
//  mat4 View = mat4(1.);
//  View = translate(View, vec3(2.0f,4.0f, -25.0f));
//
//  mat4 ViewRotateX:
//  ViewRotateX = rotate( View, 0.23 , vec3(-1.0f, 0.0f, 0.0f);
//
//  glm::mat4 Model = mat4(1.0);
//  Model = scale(mat4(1.0f),vec3(0.5f));
//

//  The following functions works for both GLSL and GLM,
//  so you can copy&pase this into your shaders :)
//

mat3 mat3RotateX(float _phi){
    return mat3(vec3(1.,0.,0.),
                vec3(0.,cos(_phi),sin(_phi)),
                vec3(0.,-sin(_phi),cos(_phi)));
}

mat3 mat3RotateY(float _theta){
    return mat3(vec3(cos(_theta),0.,sin(_theta)),
                vec3(0.,1.,0.),
                vec3(-sin(_theta),0.,cos(_theta)));
}

mat3 mat3RotateZ(float _psi){
    return mat3(vec3(cos(_psi),sin(_psi),0.),
                vec3(-sin(_psi),cos(_psi),0.),
                vec3(0.,0.,1.));
}

mat3 mat3Scale(float _x, float _y, float _z){
    return mat3(vec3(_x, 0.0, 0.0),
                vec3(0.0, _y, 0.0),
                vec3(0.0, 0.0, _z));
}

mat4 mat4Translate(float _x, float _y, float _z){
    return mat4(vec4(1.0, 0.0, 0.0, 0.0),
                vec4(0.0, 1.0, 0.0, 0.0),
                vec4(0.0, 0.0, 1.0, 0.0),
                vec4( _x,  _y,  _z, 1.0));
}

mat4 mat4Translate(vec3 _v){
    return mat4Translate(_v.x,_v.y,_v.z);
}

mat4 mat4RotateX(float _phi){
    return mat4(vec4(1.,0.,0.,0),
                vec4(0.,cos(_phi),-sin(_phi),0.),
                vec4(0.,sin(_phi),cos(_phi),0.),
                vec4(0.,0.,0.,1.));
}

mat4 mat4RotateY(float _theta){
    return mat4(vec4(cos(_theta),0.,-sin(_theta),0),
                vec4(0.,1.,0.,0.),
                vec4(sin(_theta),0.,cos(_theta),0.),
                vec4(0.,0.,0.,1.));
}

mat4 mat4RotateZ(float _psi){
    return mat4(vec4(cos(_psi),-sin(_psi),0.,0),
                vec4(sin(_psi),cos(_psi),0.,0.),
                vec4(0.,0.,1.,0.),
                vec4(0.,0.,0.,1.));
}

mat4 mat4Scale(float _x, float _y, float _z){
    return mat4(
                vec4(_x, 0.0, 0.0, 0.0),
                vec4(0.0, _y, 0.0, 0.0),
                vec4(0.0, 0.0, _z,  0.0),
                vec4(0.0, 0.0, 0.0, 1.0)
                );
}

mat4 mat4Scale(float _s){
    return mat4Scale(_s,_s,_s);
}