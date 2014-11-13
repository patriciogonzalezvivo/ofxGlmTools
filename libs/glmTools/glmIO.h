//
//  glmIO.h
//
//  Created by Patricio Gonzalez Vivo on 11/13/14.
//
//

#pragma once

#include "glmMesh.h"

bool loadPLY(glmMesh &_mesh, const std::string &_path);
bool savePLY(const glmMesh &_mesh, const std::string &_path, bool _useBinary = false);

bool loadXYZ(std::vector<glm::vec3> &_points, const std::string &_path);
inline bool loadXYZ(glmMesh &_mesh, const std::string &_path){
    std::vector<glm::vec3> points;
    loadXYZ(points,_path);
    _mesh.addVertices(points);
}

bool saveXYZ(const std::vector<glm::vec3> &_points, const std::string &_path);
bool saveXYZ(const glmMesh &_mesh, const std::string &_path){
    saveXYZ(_mesh.getVertices(),_path);
}

