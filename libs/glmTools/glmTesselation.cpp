//
//  glmTessalation.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/13/14.
//
//

#include "glmTesselation.h"

#include "tesselator.h"

//  http://artgrammer.blogspot.co.uk/2011/07/drawing-polylines-by-tessellation.html
//  https://www.mapbox.com/blog/drawing-antialiased-lines/
//

void flatLine(glmMesh &_mesh, const glmPolyline &_polyline, float _lineWidth){
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();
    
    glm::vec3 normi;             // Right normal to segment between previous and current m_points
    glm::vec3 normip1;           // Right normal to segment between current and next m_points
    glm::vec3 rightNorm;         // Right "normal" at current point, scaled for miter joint
    
    glm::vec3 im1;                   // Previous point coordinates
    glm::vec3 i0 = _polyline[0];    // Current point coordinates
    glm::vec3 ip1 = _polyline[1];   // Next point coordinates
    
    normip1.x = ip1.y - i0.y;
    normip1.y = i0.x - ip1.x;
    normip1.z = 0.;
    
    normip1 = glm::normalize(normip1);
    
    rightNorm = glm::vec3(normip1.x*_lineWidth,normip1.y*_lineWidth,normip1.z*_lineWidth);
    
    _mesh.addVertex(i0 + rightNorm);
    _mesh.addNormal(UP_NORMAL);
    _mesh.addTexCoord(glm::vec2(1.0,0.0));
    
    _mesh.addVertex(i0 - rightNorm);
    _mesh.addNormal(UP_NORMAL);
    _mesh.addTexCoord(glm::vec2(0.0,0.0));
    
    // Loop over intermediate m_points in the polyline
    //
    for (int i = 1; i < _polyline.size() - 1; i++) {
        im1 = i0;
        i0 = ip1;
        ip1 = _polyline[i+1];
        
        normi = normip1;
        normip1.x = ip1.y - i0.y;
        normip1.y = i0.x - ip1.x;
        normip1.z = 0.0f;
        normip1 = glm::normalize(normip1);
        
        rightNorm = normi + normip1;
        float scale = sqrtf(2. / (1. + glm::dot(normi,normip1) )) * _lineWidth / 2.;
        rightNorm *= scale;
        
        _mesh.addVertex(i0+rightNorm);
        _mesh.addNormal(UP_NORMAL);
        _mesh.addTexCoord(glm::vec2(1.0,(float)i/(float)_polyline.size()));
        
        _mesh.addVertex(i0-rightNorm);
        _mesh.addNormal(UP_NORMAL);
        _mesh.addTexCoord(glm::vec2(0.0,(float)i/(float)_polyline.size()));
        
    }
    
    normip1 *= _lineWidth;
    
    _mesh.addVertex(ip1 + normip1);
    _mesh.addNormal(UP_NORMAL);
    _mesh.addTexCoord(glm::vec2(1.0,1.0));
    
    _mesh.addVertex(ip1 - normip1);
    _mesh.addNormal(UP_NORMAL);
    _mesh.addTexCoord(glm::vec2(0.0,1.0));
    
    if (_mesh.getDrawMode() == TRIANGLES){
        for (int i = 0; i < _polyline.size() - 1; i++) {
            _mesh.addIndex(indexOffset + 2*i+3);
            _mesh.addIndex(indexOffset + 2*i+2);
            _mesh.addIndex(indexOffset + 2*i);
            
            _mesh.addIndex(indexOffset + 2*i);
            _mesh.addIndex(indexOffset + 2*i+1);
            _mesh.addIndex(indexOffset + 2*i+3);
        }
    } else if(_mesh.getDrawMode() == TRIANGLE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else if(_mesh.getDrawMode() == LINES){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }  else if(_mesh.getDrawMode() == LINE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else if(_mesh.getDrawMode() == POINTS){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else {
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }
}

void tesselate(glmMesh &_mesh, const glmPolyline &_polyline){
    TESStesselator *m_tess = tessNewTess(NULL);
    
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();
    glmRectangle bBox = _polyline.getBoundingBox();
    
    for (int i = 0; i < _polyline.size(); i++) {
        // Add contour to tesselator
        tessAddContour(m_tess, 3, &_polyline.getVertices()[0].x, sizeof(glm::vec3), _polyline.size());
    }
    
    if (_mesh.getDrawMode() == TRIANGLES){
        // Tessellate polygon into triangles
        tessTesselate(m_tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 3, NULL);
        
        // Extract triangle elements from tessellator
        
        const int numIndices = tessGetElementCount(m_tess);
        const TESSindex* indices = tessGetElements(m_tess);
        
        for (int i = 0; i < numIndices; i++) {
            const TESSindex* poly = &indices[i*3];
            for (int j = 0; j < 3; j++) {
                _mesh.addIndex(poly[j] + indexOffset);
            }
        }
        
        const int numVertices = tessGetVertexCount(m_tess);
        const float* vertices = tessGetVertices(m_tess);
        for (int i = 0; i < numVertices; i++) {
            
            _mesh.addTexCoord(glm::vec2(mapValue(vertices[3*i],bBox.getMinX(),bBox.getMaxX(),0.,1.),
                                        mapValue(vertices[3*i+1],bBox.getMinY(),bBox.getMaxY(),0.,1.)));
            _mesh.addNormal(UP_NORMAL);
            _mesh.addVertex(glm::vec3(vertices[3*i], vertices[3*i + 1], vertices[3*i + 2]));
        }
    } else if(_mesh.getDrawMode() == TRIANGLE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else if(_mesh.getDrawMode() == LINES){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }  else if(_mesh.getDrawMode() == LINE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else if(_mesh.getDrawMode() == POINTS){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else {
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }
    
    tessDeleteTess(m_tess);
}

void tesselate(glmMesh &_mesh, const std::vector<glmPolyline> &_polylines){
    TESStesselator  *m_tess = tessNewTess(NULL);            // Tesselator instance
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();    // track indices
    
    glmRectangle bBox;  // this will calculate the total bounding box to compute a UV for top the top view
    
    //  Go through the Json polygons making walls and adding it to the tessalator
    //
    for (int i = 0; i < _polylines.size(); i++) {
        
        //  Grow  bounding box
        //
        bBox.growToInclude(_polylines[i].getVertices());
        //  Add to tesselator
        tessAddContour(m_tess, 3, &_polylines[i].getVertices()[0].x, sizeof(glm::vec3), _polylines[i].size());
    }
    
    if (_mesh.getDrawMode() == TRIANGLES){
        // Tessellate polygon into triangles
        tessTesselate(m_tess, TESS_WINDING_NONZERO, TESS_POLYGONS, 3, 3, NULL);
        
        // Extract triangle elements from tessellator
        const int numIndices = tessGetElementCount(m_tess);
        const TESSindex* indices = tessGetElements(m_tess);
        for (int i = 0; i < numIndices; i++) {
            const TESSindex* poly = &indices[i*3];
            for (int j = 0; j < 3; j++) {
                _mesh.addIndex(poly[j] + indexOffset);
            }
        }
        
        //  Add vertexes from tessellator
        //
        const int numVertices = tessGetVertexCount(m_tess);
        const float* vertices = tessGetVertices(m_tess);
        for (int i = 0; i < numVertices; i++) {
            _mesh.addTexCoord(glm::vec2(mapValue(vertices[3*i],bBox.getMinX(),bBox.getMaxX(),0.,1.),
                                        mapValue(vertices[3*i+1],bBox.getMinY(),bBox.getMaxY(),0.,1.)));
            _mesh.addNormal(UP_NORMAL);
            _mesh.addVertex(glm::vec3(vertices[3*i], vertices[3*i + 1], vertices[3*i + 2]));
        }
    } else if(_mesh.getDrawMode() == TRIANGLE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else if(_mesh.getDrawMode() == LINES){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }  else if(_mesh.getDrawMode() == LINE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else if(_mesh.getDrawMode() == POINTS){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    } else {
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }
    
    tessDeleteTess(m_tess);
}

void extrude(glmMesh &_mesh, const glmPolyline &_polyline, float _min, float _max ){
    
    //  Build top and buttom
    //
    glmPolyline top, buttom;
    for (int i = 0; i < _polyline.size(); i++) {
        top.add(glm::vec3(_polyline[i].x,_polyline[i].y, _max));
        buttom.add(glm::vec3(_polyline[i].x,_polyline[i].y, _min));
    }
    tesselate(_mesh, top);
    tesselate(_mesh, buttom);
    
    //  Build walls
    //
    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();
    glm::vec3 tan, nor;
    for (int i = 0; i < _polyline.size() - 1; i++) {
        
        //For each vertex in the polygon, make two triangles to form a quad
        //
        glm::vec3 ip0 = glm::vec3(_polyline[i].x,_polyline[i].y,0);
        glm::vec3 ip1 = glm::vec3(_polyline[i+1].x,_polyline[i+1].y,0);
        
        tan = ip1 - ip0;
        nor = glm::cross(-UP_NORMAL, tan);
        nor = nor;
        
        _mesh.addTexCoord(glm::vec2(1.,0.));
        _mesh.addVertex(ip0 + glm::vec3(0,0, _min));
        _mesh.addNormal(nor);
        _mesh.addTexCoord(glm::vec2(0.,0.));
        _mesh.addVertex(ip1 + glm::vec3(0,0, _min));
        _mesh.addNormal(nor);
        
        _mesh.addTexCoord(glm::vec2(1.,1.));
        _mesh.addVertex(ip0 + glm::vec3(0,0, _max));
        _mesh.addNormal(nor);
        _mesh.addTexCoord(glm::vec2(0.,1.));
        _mesh.addVertex(ip1 + glm::vec3(0,0, _max));
        _mesh.addNormal(nor);
        
        _mesh.addIndex(indexOffset);
        _mesh.addIndex(indexOffset + 2);
        _mesh.addIndex(indexOffset + 1);
        
        _mesh.addIndex(indexOffset + 1);
        _mesh.addIndex(indexOffset + 2);
        _mesh.addIndex(indexOffset + 3);
        indexOffset += 4;
    }
    
    return _mesh;
}

void extrude(glmMesh &_mesh, const std::vector<glmPolyline> &_polylines, float _min, float _max ){
//    
//    //  Build top and buttom
//    //
//    glmPolyline top, buttom;
//    for (int i = 0; i < _polyline.size(); i++) {
//        top.add(glm::vec3(_polyline[i].x,_polyline[i].y, _max));
//        buttom.add(glm::vec3(_polyline[i].x,_polyline[i].y, _min));
//    }
//    tessalate(_mesh, top);
//    tessalate(_mesh, buttom);
//    
//    //  Build walls
//    //
//    uint16_t indexOffset = (uint16_t)_mesh.getVertices().size();
//    glm::vec3 tan, nor;
//    for (int i = 0; i < _polyline.size() - 1; i++) {
//        
//        //For each vertex in the polygon, make two triangles to form a quad
//        //
//        glm::vec3 ip0 = glm::vec3(_polyline[i].x,_polyline[i].y,0);
//        glm::vec3 ip1 = glm::vec3(_polyline[i+1].x,_polyline[i+1].y,0);
//        
//        tan = ip1 - ip0;
//        nor = glm::cross(UP_NORMAL, tan);
//        nor = nor;
//        
//        _mesh.addTexCoord(glm::vec2(1.,0.));
//        _mesh.addVertex(ip0 + glm::vec3(0,0, _min));
//        _mesh.addNormal(nor);
//        _mesh.addTexCoord(glm::vec2(0.,0.));
//        _mesh.addVertex(ip1 + glm::vec3(0,0, _min));
//        _mesh.addNormal(nor);
//        
//        _mesh.addTexCoord(glm::vec2(1.,1.));
//        _mesh.addVertex(ip0 + glm::vec3(0,0, _max));
//        _mesh.addNormal(nor);
//        _mesh.addTexCoord(glm::vec2(0.,1.));
//        _mesh.addVertex(ip1 + glm::vec3(0,0, _max));
//        _mesh.addNormal(nor);
//        
//        _mesh.addIndex(indexOffset);
//        _mesh.addIndex(indexOffset + 2);
//        _mesh.addIndex(indexOffset + 1);
//        
//        _mesh.addIndex(indexOffset + 1);
//        _mesh.addIndex(indexOffset + 2);
//        _mesh.addIndex(indexOffset + 3);
//        indexOffset += 4;
//    }
//    
//    return _mesh;
}