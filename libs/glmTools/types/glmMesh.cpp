//
//  glmMesh.cpp
//
//  Created by Patricio Gonzalez Vivo on 8/22/14.
//
//

#include "glmMesh.h"

glmMesh::glmMesh():m_drawMode(TRIANGLES){
    
}

glmMesh::glmMesh(const glmMesh &_mother):m_drawMode(_mother.getDrawMode()){
    add(_mother);
}

glmMesh::~glmMesh(){
    
}

void glmMesh::setDrawMode(DrawMode _drawMode){
    m_drawMode = _drawMode;
}

void glmMesh::setColor(const glm::vec4 &_color){
    m_colors.clear();
    for (int i = 0; i < m_vertices.size(); i++) {
        m_colors.push_back(_color);
    }
}

void glmMesh::addColor(const glm::vec4 &_color){
    m_colors.push_back(_color);
}

void glmMesh::addColors(const std::vector<glm::vec4> &_colors){
    m_colors.insert(m_colors.end(), _colors.begin(), _colors.end());
}

void glmMesh::addVertex(const glm::vec3 &_point){
   m_vertices.push_back(_point);
}

void glmMesh::addVertices(const std::vector<glm::vec3>& _verts){
   m_vertices.insert(m_vertices.end(),_verts.begin(),_verts.end());
}

void glmMesh::addVertices(const glm::vec3* verts, int amt){
   m_vertices.insert(m_vertices.end(),verts,verts+amt);
}

void glmMesh::addNormal(const glm::vec3 &_normal){
    m_normals.push_back(_normal);
}

void glmMesh::addNormals(const std::vector<glm::vec3> &_normals ){
    m_normals.insert(m_normals.end(), _normals.begin(), _normals.end());
}

void glmMesh::addTexCoord(const glm::vec2 &_uv){
    m_texCoords.push_back(_uv);
}

void glmMesh::addTexCoords(const std::vector<glm::vec2> &_uvs){
    m_texCoords.insert(m_texCoords.end(), _uvs.begin(), _uvs.end());
}

void glmMesh::addIndex(uint16_t _i){
    m_indices.push_back(_i);
}

void glmMesh::addIndices(const std::vector<uint16_t>& inds){
	m_indices.insert(m_indices.end(),inds.begin(),inds.end());
}

void glmMesh::addIndices(const uint16_t* inds, int amt){
	m_indices.insert(m_indices.end(),inds,inds+amt);
}

void glmMesh::addTriangle(uint16_t index1, uint16_t index2, uint16_t index3){
    addIndex(index1);
    addIndex(index2);
    addIndex(index3);
}

void glmMesh::add(const glmMesh &_mesh){
    
    if(_mesh.getDrawMode() != m_drawMode){
        std::cout << "INCOMPATIBLE DRAW MODES" << std::endl;
        return;
    }
    
    uint16_t indexOffset = (uint16_t)getVertices().size();
    
    addColors(_mesh.getColors());
    addVertices(_mesh.getVertices());
    addNormals(_mesh.getNormals());
    addTexCoords(_mesh.getTexCoords());
    
    for (int i = 0; i < _mesh.getIndices().size(); i++) {
        addIndex(indexOffset+_mesh.getIndices()[i]);
    }
}

DrawMode glmMesh::getDrawMode() const{
    return m_drawMode;
}

const std::vector<glm::vec4> & glmMesh::getColors() const{
    return m_colors;
}

const std::vector<glm::vec3> & glmMesh::getVertices() const{
	return m_vertices;
}

const std::vector<glm::vec3> & glmMesh::getNormals() const{
    return m_normals;
}

const std::vector<glm::vec2> & glmMesh::getTexCoords() const{
    return m_texCoords;
}

const std::vector<glm::uint16_t> & glmMesh::getIndices() const{
    return m_indices;
}

std::vector<glm::ivec3> glmMesh::getTriangles() const {
    std::vector<glm::ivec3> faces;
    
    if(getDrawMode() == TRIANGLES) {
        if(m_indices.size()>0){
            for(unsigned int j = 0; j < m_indices.size(); j += 3) {
                glm::ivec3 tri;
                for(int k = 0; k < 3; k++) {
                    tri[k] = m_indices[j+k];
                }
                faces.push_back(tri);
            }
        } else {
            for(unsigned int j = 0; j < m_vertices.size(); j += 3) {
                glm::ivec3 tri;
                for(int k = 0; k < 3; k++) {
                    tri[k] = j+k;
                }
                faces.push_back(tri);
            }
        }
    } else if (getDrawMode() == TRIANGLE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }
    
    return faces;
}

void glmMesh::clear(){
    if(!m_vertices.empty()){
		m_vertices.clear();
	}
	if(!m_colors.empty()){
		m_colors.clear();
	}
	if(!m_normals.empty()){
		m_normals.clear();
	}
    if(!m_indices.empty()){
		m_indices.clear();
	}
}

void glmMesh::computeNormals(){
    
    if(getDrawMode() == TRIANGLES){
        //The number of the vertices
        int nV = m_vertices.size();
        
        //The number of the triangles
        int nT = m_indices.size() / 3;
        
        std::vector<glm::vec3> norm( nV ); //Array for the normals
        
        //Scan all the triangles. For each triangle add its
        //normal to norm's vectors of triangle's vertices
        for (int t=0; t<nT; t++) {
            
            //Get indices of the triangle t
            int i1 = m_indices[ 3 * t ];
            int i2 = m_indices[ 3 * t + 1 ];
            int i3 = m_indices[ 3 * t + 2 ];
            
            //Get vertices of the triangle
            const glm::vec3 &v1 = m_vertices[ i1 ];
            const glm::vec3 &v2 = m_vertices[ i2 ];
            const glm::vec3 &v3 = m_vertices[ i3 ];
            
            //Compute the triangle's normal
            glm::vec3 dir = glm::normalize(glm::cross(v2-v1,v3-v1));
            
            //Accumulate it to norm array for i1, i2, i3
            norm[ i1 ] += dir;
            norm[ i2 ] += dir;
            norm[ i3 ] += dir;
        }
        
        //Normalize the normal's length and add it.
        m_normals.clear();
        for (int i=0; i<nV; i++) {
            addNormal( glm::normalize(norm[i]) );
        }
        
    } else if ( getDrawMode() == TRIANGLE_STRIP){
        //  TODO
        //
        std::cout << "ERROR: glmMesh only add GL_TRIANGLES for NOW !!" << std::endl;
    }
}