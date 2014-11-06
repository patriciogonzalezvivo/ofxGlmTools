//
//  ofxGlmTools.cpp
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#include "ofxGlmTools.h"

#include "ofxGlm.h"

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
    
    mesh.setMode(ofGetOFPrimitiveMode(_mesh.getDrawMode()));
    
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
    
    mesh.setDrawMode(ofGetGLPrimitiveMode(_mesh.getMode()));
    
    return mesh;
}