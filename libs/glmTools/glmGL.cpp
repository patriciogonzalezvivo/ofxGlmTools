//
//  glmGL.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#include "glmGL.h"

#include <OpenGL/gl.h>

std::vector<glm::vec3> project(const std::vector<glm::vec3> &_pts){
    glm::ivec4 viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    glm::mat4x4 mvmatrix, projmatrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, &mvmatrix[0][0]);
    glGetFloatv(GL_PROJECTION_MATRIX, &projmatrix[0][0]);
    
    return project(_pts,viewport,mvmatrix,projmatrix);
}

std::vector<glm::vec3> project(const std::vector<glm::vec3> &_pts, const glm::ivec4 &_viewport,const glm::mat4x4 &_mvmatrix, const  glm::mat4x4 &_projmatrix){
    std::vector<glm::vec3> rta;
    for (int i = 0; i < _pts.size(); i++) {
        rta.push_back(glm::project(_pts[i], _mvmatrix, _projmatrix, _viewport));
    }
    return rta;
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

void drawLine(const glm::vec3 &_a, const glm::vec3 &_b){
    glm::ivec3 linePoints[2];
    linePoints[0] = glm::ivec3(_a.x,_a.y,_a.z);
    linePoints[1] = glm::ivec3(_b.x,_b.y,_b.z);;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, sizeof(glm::ivec3), &linePoints[0].x);
    glDrawArrays(GL_LINES, 0, 2);
};

void drawStippleLine(const glm::vec3 &_a, const glm::vec3 &_b, const unsigned short &_pattern ){
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

void drawPlane( const glmPlane &_plane ){
    //  TODO
    //
}

void drawTriangle( const glmTriangle &_tri ){
    glm::vec3 A = _tri.getP0();
    glm::vec3 B = _tri.getP1();
    glm::vec3 C = _tri.getP2();
    glm::vec3 normal = _tri.getNormal();
    
    glBegin(GL_TRIANGLES);
    glNormal3fv(&normal.x);
    glVertex3fv(&A.x);
    glNormal3fv(&normal.x);
    glVertex3fv(&B.x);
    glNormal3fv(&normal.x);
    glVertex3fv(&C.x);
    glEnd();
}

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

void drawMesh(const glmMesh &_mesh){
    if(_mesh.getVertices().size()){
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), &_mesh.getVertices()[0].x);
    }
    if(_mesh.getNormals().size()){
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(glm::vec3), &_mesh.getNormals()[0].x);
    }
    if(_mesh.getColors().size()){
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4,GL_FLOAT, sizeof(glm::vec4), &_mesh.getColors()[0].x);
    }
    if(_mesh.getTexCoords().size()){
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(glm::vec2), &_mesh.getTexCoords()[0].x);
    }
    
    GLenum drawMode = GL_TRIANGLES;
    
    if (_mesh.getDrawMode() == POINTS) {
        drawMode = GL_POINT;
    } else if (_mesh.getDrawMode() == LINES) {
        drawMode = GL_LINES;
    } else if (_mesh.getDrawMode() == LINE_STRIP) {
        drawMode = GL_LINE_STRIP;
    } else if (_mesh.getDrawMode() == TRIANGLES) {
        drawMode = GL_TRIANGLES;
    } else if (_mesh.getDrawMode() == TRIANGLE_STRIP) {
        drawMode = GL_TRIANGLE_STRIP;
    }
    
    if(_mesh.getIndices().size()){
        glDrawElements(drawMode, _mesh.getIndices().size(),GL_UNSIGNED_SHORT,&_mesh.getIndices()[0]);
    }else{
        glDrawArrays(drawMode, 0, _mesh.getVertices().size());
    }
    
    if(_mesh.getTexCoords().size()){
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if(_mesh.getColors().size()){
        glDisableClientState(GL_COLOR_ARRAY);
    }
    if(_mesh.getNormals().size()){
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}