//
//  glmGL.h
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include "glmRectangle.h"
#include "glmPolyline.h"
#include "glmMesh.h"

std::vector<glm::vec3> project(const std::vector<glm::vec3> &_pts);
std::vector<glm::vec3> project(const std::vector<glm::vec3> &_pts, const glm::ivec4 &_viewport,const glm::mat4x4 &_mvmatrix, const  glm::mat4x4 &_projmatrix);

void drawCross(const glm::vec3 &_pos, const float &_width = 3.5);

void drawArrow(const glm::vec3 &_pos, const float &_angle, const float &_width = 6.0);
void drawLine(const glm::vec3 &_a, const glm::vec3 &_b);
void drawStippleLine(const glm::vec3 &_a, const glm::vec3 &_b, const unsigned short &_pattern = 0x00FF);

void drawBorders( const glmRectangle &_rect );
void drawCorners(const glmRectangle &_rect, const float &_width = 4.);

void drawPoints(const std::vector<glm::vec3> &_pts );
void drawPolyline(const std::vector<glm::vec3> &_pts );
inline void drawPolyline(const glmPolyline &_poly){ drawPolyline( _poly.getVertices() ); }

void drawMesh(const glmMesh &_mesh);

inline void draw(const glm::vec3 &_a){drawCross(_a);}
inline void draw(const glm::vec3 &_a, float _width){drawCross(_a,_width);}
inline void draw(const glm::vec3 &_a, const glm::vec3 &_b,const unsigned short &_pattern){drawStippleLine(_a, _b, _pattern);}
inline void draw(const glm::vec3 &_a, const glm::vec3 &_b){drawLine(_a, _b);}
inline void draw(const glmRectangle &_rect,const float &_width){drawCorners(_rect,_width);}
inline void draw(const glmRectangle &_rect){drawBorders(_rect);}
inline void draw(const glmPolyline &_poly){drawPolyline(_poly);}
inline void draw(const std::vector<glm::vec3> &_pts){ drawPolyline(_pts); }
inline void draw(const glmMesh &_mesh){ drawMesh(_mesh); }