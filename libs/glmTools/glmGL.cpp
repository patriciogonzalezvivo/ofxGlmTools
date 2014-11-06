//
//  glmGL.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#include "glmGL.h"

void drawArrow(const glm::vec3 &_pos, const float &_angle, const float &_width ){
    glm::ivec3 linePoints[4] = {    glm::ivec3(_pos.x,_pos.y,_pos.z),
        glm::ivec3(_pos.x,_pos.y,_pos.z),
        glm::ivec3(_pos.x,_pos.y,_pos.z),
        glm::ivec3(_pos.x,_pos.y,_pos.z)};
    
    linePoints[0].x += (int)_width*cos(_angle+M_PI_4*3.);
    linePoints[0].y += (int)_width*sin(_angle+M_PI_4*3.);
    
    linePoints[3].x += (int)_width*cos(_angle-M_PI_4*3.);
    linePoints[3].y += (int)_width*sin(_angle-M_PI_4*3.);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, sizeof(glm::ivec3), &linePoints[0].x);
    glDrawArrays(GL_LINES, 0, 4);
}

void drawCross(const glm::vec3 &_pos, const float &_width ){
    glm::ivec3 linePoints[4] = {glm::ivec3(_pos.x,_pos.y,_pos.z),
        glm::ivec3(_pos.x,_pos.y,_pos.z),
        glm::ivec3(_pos.x,_pos.y,_pos.z),
        glm::ivec3(_pos.x,_pos.y,_pos.z) };
    linePoints[0].x -= _width;
    linePoints[1].x += _width;
    linePoints[2].y -= _width;
    linePoints[3].y += _width;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, sizeof(glm::ivec3), &linePoints[0].x);
    glDrawArrays(GL_LINES, 0, 4);
}

void drawLine(const glm::vec3 &_a, const glm::vec3 &_b){
    glm::ivec3 linePoints[2];
    linePoints[0] = glm::ivec3(_a.x,_a.y,_a.z);
    linePoints[1] = glm::ivec3(_b.x,_b.y,_b.z);;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, sizeof(glm::ivec3), &linePoints[0].x);
    glDrawArrays(GL_LINES, 0, 2);
};

void drawStippleLine(const glm::vec3 &_a, const glm::vec3 &_b, const GLushort &_pattern ){
    glm::vec3 linePoints[2];
    linePoints[0] = _a;
    linePoints[1] = _b;
    
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, _pattern);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(glm::vec3), &linePoints[0].x);
    glDrawArrays(GL_LINES, 0, 2);
    glDisable(GL_LINE_STIPPLE);
};

void drawBorders( const glmRectangle &_rect ){
    glm::vec3 linePoints[5] = {_rect.getTopLeft(), _rect.getTopRight(), _rect.getBottomRight(), _rect.getBottomLeft(), _rect.getTopLeft()};
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(glm::vec3), &linePoints[0].x);
    glDrawArrays(GL_LINE_STRIP, 0, 5);
}

void drawCorners(const glmRectangle &_rect, const float &_width){
    glm::vec3 linePoints[16] = {_rect.getTopLeft(), _rect.getTopLeft(),_rect.getTopLeft(), _rect.getTopLeft(),
                                _rect.getTopRight(), _rect.getTopRight(),_rect.getTopRight(), _rect.getTopRight(),
                                _rect.getBottomRight(), _rect.getBottomRight(), _rect.getBottomRight(), _rect.getBottomRight(),
                                _rect.getBottomLeft(), _rect.getBottomLeft(),_rect.getBottomLeft(), _rect.getBottomLeft() };
    linePoints[0].x += _width;
    linePoints[3].y += _width;
    
    linePoints[4].x -= _width;
    linePoints[7].y += _width;
    
    linePoints[8].x -= _width;
    linePoints[11].y -= _width;
    
    linePoints[12].x += _width;
    linePoints[15].y -= _width;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(glm::vec3), &linePoints[0].x);
    glDrawArrays(GL_LINES, 0, 16);
}

void drawPoints(const std::vector<glm::vec3> &_pts ){
    glBegin(GL_POINTS);
    for (int i = 0; i < _pts.size(); i++) {
        glVertex3fv(&_pts[i].x);
    }
    glEnd();
}

void drawPolyline(const std::vector<glm::vec3> &_pts ){
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < _pts.size(); i++) {
        glVertex3fv(&_pts[i].x);
    }
    glEnd();
}