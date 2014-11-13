//
//  glmTessalation.h
//
//  Created by Patricio Gonzalez Vivo on 11/13/14.
//
//

#pragma once

#include "glmMesh.h"
#include "glmPolyline.h"

void flatLine(glmMesh &_mesh, const glmPolyline &_polyline, float _lineWidth);
inline glmMesh getFlatLine(const glmPolyline &_polyline, float _lineWidth){
    glmMesh mesh;
    mesh.setDrawMode(TRIANGLES);
    flatLine(mesh, _polyline,_lineWidth);
    return mesh;
}

// Close Shape
void tesselate(glmMesh &_mesh, const glmPolyline &_polyline);
inline glmMesh getMesh(const glmPolyline &_polyline){
    glmMesh mesh;
    mesh.setDrawMode(TRIANGLES);
    tesselate(mesh, _polyline);
    return mesh;
}

// Close Shape with holes
void tesselate(glmMesh &_mesh, const std::vector<glmPolyline> &_polylines);
inline glmMesh getMesh(const std::vector<glmPolyline> &_polylines){
    glmMesh mesh;
    mesh.setDrawMode(TRIANGLES);
    tesselate(mesh, _polylines);
    return mesh;
}

void extrude(glmMesh &_mesh, const glmPolyline &_polyline, float _min, float _max );
inline glmMesh getExtrude(const glmPolyline &_polyline, float _min, float _max ){
    glmMesh mesh;
    mesh.setDrawMode(TRIANGLES);
    extrude(mesh, _polyline,_min,_max);
    return mesh;
}

void extrude(glmMesh &_mesh, const std::vector<glmPolyline> &_polylines, float _min, float _max );
inline glmMesh getExtrude(const std::vector<glmPolyline> &_polylines, float _min, float _max ){
    glmMesh mesh;
    mesh.setDrawMode(TRIANGLES);
    extrude(mesh, _polylines,_min,_max);
    return mesh;
}