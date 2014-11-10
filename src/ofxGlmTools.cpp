//
//  ofxGlmTools.cpp
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#include "ofxGlmTools.h"

ofRectangle toOf(const glmRectangle &_rect){
    return ofRectangle(_rect.x,_rect.y,_rect.width,_rect.height);
}

glmRectangle toGlm(const ofRectangle &_rect){
    return glmRectangle(_rect.x,_rect.y,_rect.width,_rect.height);
}

ofPolyline toOf(const glmPolyline &_poly){
    ofPolyline poly;
    for (int i = 0; i < _poly.size(); i++) {
        poly.addVertex(toOf(_poly[i]));
    }
    return poly;
}

glmPolyline toGlm(const ofPolyline &_poly){
    glmPolyline poly;
    for (int i = 0; i < _poly.size(); i++) {
        poly.add(toGlm(_poly[i]));
    }
    return poly;
}

ofMesh toOf(const glmMesh &_mesh){
    ofMesh mesh;
    
    for (auto &it : _mesh.getColors()) {
        mesh.addColor(toOf(it));
    }
    
    for (auto &it : _mesh.getVertices()) {
        mesh.addVertex(toOf(it));
    }
    
    for (auto &it : _mesh.getNormals()) {
        mesh.addNormal(toOf(it));
    }
    
    for (auto &it : _mesh.getTexCoords()) {
        mesh.addTexCoord(toOf(it));
    }
    
    for (auto &it : _mesh.getIndices()) {
        mesh.addIndex(toOf(it));
    }
    
    DrawMode drawMode = _mesh.getDrawMode();
    
    if(drawMode == POINTS){
        mesh.setMode(OF_PRIMITIVE_POINTS);
    } else if(drawMode == LINES){
        mesh.setMode(OF_PRIMITIVE_LINES);
    } else if(drawMode == LINE_STRIP){
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    } else if(drawMode == TRIANGLES){
        mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    } else if(drawMode == TRIANGLE_STRIP){
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    }
    
    return mesh;
}

glmMesh toGlm(const ofMesh &_mesh){
    glmMesh mesh;
    
    for (auto &it : _mesh.getColors()) {
        mesh.addColor(toGlm(it));
    }
    
    for (auto &it : _mesh.getVertices()) {
        mesh.addVertex(toGlm(it));
    }
    
    for (auto &it : _mesh.getNormals()) {
        mesh.addNormal(toGlm(it));
    }
    
    for (auto &it : _mesh.getTexCoords()) {
        mesh.addTexCoord(toGlm(it));
    }
    
    for (auto &it : _mesh.getIndices()) {
        mesh.addIndex(toGlm(it));
    }
    
    GLenum drawMode = ofGetGLPrimitiveMode(_mesh.getMode());
    if (drawMode == GL_POINTS) {
        mesh.setDrawMode(POINTS);
    } else if (drawMode == GL_LINES){
        mesh.setDrawMode(LINES);
    } else if (drawMode == GL_LINE_STRIP){
        mesh.setDrawMode(LINE_STRIP);
    } else if (drawMode == GL_TRIANGLES){
        mesh.setDrawMode(TRIANGLES);
    } else if (drawMode == GL_TRIANGLE_STRIP){
        mesh.setDrawMode(TRIANGLE_STRIP);
    }
    
    
    
    return mesh;
}