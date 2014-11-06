//
//  glmGL.h
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include <OpenGL/gl.h>

#include "glmGeom.h"

#include "glmRectangle.h"
#include "glmPolyline.h"

void drawCross(const glm::vec3 &_pos, const float &_width = 3.5);

void drawArrow(const glm::vec3 &_pos, const float &_angle, const float &_width = 6.0);
void drawLine(const glm::vec3 &_a, const glm::vec3 &_b);
void drawStippleLine(const glm::vec3 &_a, const glm::vec3 &_b, const GLushort &_pattern = 0x00FF);

void drawBorders( const glmRectangle &_rect );
void drawCorners(const glmRectangle &_rect, const float &_width = 4.);

void drawPoints(const std::vector<glm::vec3> &_pts );
void drawPolyline(const std::vector<glm::vec3> &_pts );
void drawPolyline(const glmPolyline &_poly){drawPolyline(_poly.getVertices());}

//void draw(const glm::vec3 &_a){drawCross(_a);}
//void draw(const glm::vec3 &_a, const float &_width){drawCross(_a,_width);}
//void draw(const glm::vec3 &_a, const glm::vec3 &_b,const GLushort &_pattern){drawStippleLine(_a, _b, _pattern);}
//void draw(const glm::vec3 &_a, const glm::vec3 &_b){drawLine(_a, _b);}
//void draw(const glmRectangle &_rect,const float &_width){drawCorners(_rect,_width);}
//void draw(const glmRectangle &_rect){drawBorders(_rect);}
//void draw(const glmPolyline &_poly){drawPolyline(_poly);}
//void draw(const std::vector<glm::vec3> &_pts){ drawPolyline(_pts); }