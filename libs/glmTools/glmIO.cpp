//
//  glmIO.cpp
//
//  Created by Patricio Gonzalez Vivo on 11/13/14.
//
//

#include "glmIO.h"

#include <fstream>
#include <iostream>

#include "glmString.h"

bool loadPLY(glmMesh &_mesh, const std::string &_path){
    std::fstream is(_path.c_str(), std::ios::in);
    if(is.is_open()){
        
        std::string line;
        std::string error;
        
        int orderVertices=-1;
        int orderIndices=-1;
        
        int vertexCoordsFound=0;
        int colorCompsFound=0;
        int texCoordsFound=0;
        int normalsCoordsFound=0;
        
        int currentVertex = 0;
        int currentFace = 0;
        
        bool floatColor = false;
        
        enum State{
            Header,
            VertexDef,
            FaceDef,
            Vertices,
            Normals,
            Faces
        };
        
        State state = Header;
        
        int lineNum = 0;
        
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoord;
        std::vector<uint16_t> indices;

        std::getline(is,line);
        lineNum++;
        if(line!="ply"){
            error = "wrong format, expecting 'ply'";
            goto clean;
        }
        
        std::getline(is,line);
        lineNum++;
        if(line!="format ascii 1.0"){
            error = "wrong format, expecting 'format ascii 1.0'";
            goto clean;
        }
        
        while(std::getline(is,line)){
            lineNum++;
            if(line.find("comment")==0){
                continue;
            }
            
            if((state==Header || state==FaceDef) && line.find("element vertex")==0){
                state = VertexDef;
                orderVertices = MAX(orderIndices, 0)+1;
                vertices.resize(toInt(line.substr(15)));
                continue;
            }
            
            if((state==Header || state==VertexDef) && line.find("element face")==0){
                state = FaceDef;
                orderIndices = MAX(orderVertices, 0)+1;
                indices.resize(toInt(line.substr(13))*3);
                continue;
            }
            
            if(state==VertexDef && (line.find("property float x")==0 || line.find("property float y")==0 || line.find("property float z")==0)){
                vertexCoordsFound++;
                continue;
            }
            
            if(state==VertexDef && (line.find("property float r")==0 || line.find("property float g")==0 || line.find("property float b")==0 || line.find("property float a")==0)){
                colorCompsFound++;
                colors.resize(vertices.size());
                floatColor = true;
                continue;
            }
            
            if(state==VertexDef && (line.find("property uchar red")==0 || line.find("property uchar green")==0 || line.find("property uchar blue")==0 || line.find("property uchar alpha")==0)){
                colorCompsFound++;
                colors.resize(vertices.size());
                floatColor = false;
                continue;
            }
            
            if(state==VertexDef && (line.find("property float u")==0 || line.find("property float v")==0)){
                texCoordsFound++;
                texcoord.resize(vertices.size());
                continue;
            }
            
            if(state==VertexDef && (line.find("property float nx")==0 || line.find("property float ny")==0 || line.find("property float nz")==0)){
                normalsCoordsFound++;
                if (normalsCoordsFound==3) normals.resize(vertices.size());
                continue;
            }
            
            if(state==FaceDef && line.find("property list")!=0 && line!="end_header"){
                error = "wrong face definition";
                goto clean;
            }
            
            if(line=="end_header"){
                if(colors.size() && colorCompsFound!=3 && colorCompsFound!=4){
                    error =  "data has color coordiantes but not correct number of components. Found " + toString(colorCompsFound) + " expecting 3 or 4";
                    goto clean;
                }
                if(normals.size() && normalsCoordsFound!=3){
                    error = "data has normal coordiantes but not correct number of components. Found " + toString(normalsCoordsFound) + " expecting 3";
                    goto clean;
                }
                if(!vertices.size()){
                    std::cout << "ERROR glMesh, load(): mesh loaded from \"" << _path << "\" has no vertices" << std::endl;
                }
                if(orderVertices==-1) orderVertices=9999;
                if(orderIndices==-1) orderIndices=9999;
                
                if(orderVertices < orderIndices){
                    state = Vertices;
                }else {
                    state = Faces;
                }
                continue;
            }
            
            if(state==Vertices){
                std::stringstream sline;
                sline.str(line);
                glm::vec3 v;
                sline >> v.x;
                sline >> v.y;
                if(vertexCoordsFound>2) sline >> v.z;
                vertices[currentVertex] = v;
                
                if(colorCompsFound>0){
                    if (floatColor){
                        glm::vec4 c;
                        sline >> c.r;
                        sline >> c.g;
                        sline >> c.b;
                        if(colorCompsFound>3) sline >> c.a;
                        colors[currentVertex] = c;
                    }else{
                        float r, g, b, a = 255;
                        sline >> r;
                        sline >> g;
                        sline >> b;
                        if(colorCompsFound>3) sline >> a;
                        colors[currentVertex] = glm::vec4(r/255.0, g/255.0, b/255.0, a/255.0);
                    }
                }
                
                if(texCoordsFound>0){
                    glm::vec2 uv;
                    sline >> uv.x;
                    sline >> uv.y;
                    texcoord[currentVertex] = uv;
                }
                
                if (normalsCoordsFound>0){
                    glm::vec3 n;
                    sline >> n.x;
                    sline >> n.y;
                    sline >> n.z;
                    normals[currentVertex] = n;
                }
                
                currentVertex++;
                if(currentVertex==vertices.size()){
                    if(orderVertices<orderIndices){
                        state = Faces;
                    }else{
                        state = Vertices;
                    }
                }
                continue;
            }
            
            if(state==Faces){
                std::stringstream sline;
                sline.str(line);
                int numV;
                sline >> numV;
                if(numV!=3){
                    error = "face not a triangle";
                    goto clean;
                }
                int i;
                sline >> i;
                indices[currentFace*3] = i;
                sline >> i;
                indices[currentFace*3+1] = i;
                sline >> i;
                indices[currentFace*3+2] = i;
                
                currentFace++;
                if(currentFace==indices.size()/3){
                    if(orderVertices<orderIndices){
                        state = Vertices;
                    }else{
                        state = Faces;
                    }
                }
                continue;
            }
        }
        is.close();
        
        //  Succed loading the PLY data
        //  (proceed replacing the data on mesh)
        //
        _mesh.clear();
        _mesh.addColors(colors);
        _mesh.addVertices(vertices);
        _mesh.addTexCoords(texcoord);
        _mesh.addIndices(indices);
        
        if(normals.size()>0){
            _mesh.addNormals(normals);
        } else {
            std::cout << "Computing normals" << std::endl;
            _mesh.computeNormals();
        }
        
        return true;
        
    clean:
        std::cout << "ERROR glMesh, load(): " << lineNum << ":" << error << std::endl;
        std::cout << "ERROR glMesh, load(): \"" << line << "\"" << std::endl;
    
    }
    
    is.close();
    std::cout << "ERROR glMesh, can not load  " << _path << std::endl;
    return false;
}

bool savePLY(const glmMesh &_mesh, const std::string &_path, bool _useBinary){
    std::ios_base::openmode binary_mode = _useBinary ? std::ios::binary : (std::ios_base::openmode)0;
    std::fstream os(_path.c_str(), std::ios::out | binary_mode);
    
    os << "ply" << std::endl;
    if(_useBinary) {
        os << "format binary_little_endian 1.0" << std::endl;
    } else {
        os << "format ascii 1.0" << std::endl;
    }
    
    if(_mesh.getVertices().size()){
        os << "element vertex " << _mesh.getVertices().size() << std::endl;
        os << "property float x" << std::endl;
        os << "property float y" << std::endl;
        os << "property float z" << std::endl;
        if(_mesh.getColors().size()){
            os << "property uchar red" << std::endl;
            os << "property uchar green" << std::endl;
            os << "property uchar blue" << std::endl;
            os << "property uchar alpha" << std::endl;
        }
        if(_mesh.getTexCoords().size()){
            os << "property float u" << std::endl;
            os << "property float v" << std::endl;
        }
        if(_mesh.getNormals().size()){
            os << "property float nx" << std::endl;
            os << "property float ny" << std::endl;
            os << "property float nz" << std::endl;
        }
    }
    
    unsigned char faceSize = 3;
    if(_mesh.getIndices().size()){
        os << "element face " << _mesh.getIndices().size() / faceSize << std::endl;
        os << "property list uchar int vertex_indices" << std::endl;
    } else if(_mesh.getDrawMode() == TRIANGLES) {
        os << "element face " << _mesh.getIndices().size() / faceSize << std::endl;
        os << "property list uchar int vertex_indices" << std::endl;
    }
    
    os << "end_header" << std::endl;
    
    for(int i = 0; i < _mesh.getVertices().size(); i++){
        if(_useBinary) {
            os.write((char*) &_mesh.getVertices()[i], sizeof(glm::vec3));
        } else {
            os << _mesh.getVertices()[i].x << " " << _mesh.getVertices()[i].y << " " << _mesh.getVertices()[i].z;
        }
        if(_mesh.getColors().size()){
            // VCG lib / MeshLab don't support float colors, so we have to cast
            glm::vec4 c = _mesh.getColors()[i] * glm::vec4(255,255,255,255);
            glm::ivec4 cur = glm::ivec4(c.r,c.g,c.b,c.a);
            if(_useBinary) {
                os.write((char*) &cur, sizeof(glm::ivec4));
            } else {
                os << " " << (int) cur.r << " " << (int) cur.g << " " << (int) cur.b << " " << (int) cur.a;
            }
        }
        if(_mesh.getTexCoords().size()){
            if(_useBinary) {
                os.write((char*) &_mesh.getTexCoords()[i], sizeof(glm::vec2));
            } else {
                os << " " << _mesh.getTexCoords()[i].x << " " << _mesh.getTexCoords()[i].y;
            }
        }
        if(_mesh.getNormals().size()){
            glm::vec3 norm = glm::normalize(_mesh.getNormals()[i]);
            if(_useBinary) {
                os.write((char*) &norm, sizeof(glm::vec3));
            } else {
                os << " " << norm.x << " " << norm.y << " " << norm.z;
            }
        }
        if(!_useBinary) {
            os << std::endl;
        }
    }
    
    if(_mesh.getIndices().size()) {
        for(int i = 0; i < _mesh.getIndices().size(); i += faceSize) {
            if(_useBinary) {
                os.write((char*) &faceSize, sizeof(unsigned char));
                for(int j = 0; j < faceSize; j++) {
                    int curIndex = _mesh.getIndices()[i + j];
                    os.write((char*) &curIndex, sizeof(int));
                }
            } else {
                os << (int) faceSize << " " << _mesh.getIndices()[i] << " " << _mesh.getIndices()[i+1] << " " << _mesh.getIndices()[i+2] << std::endl;
            }
        }
    } else if(_mesh.getDrawMode() == TRIANGLES) {
        for(int i = 0; i < _mesh.getVertices().size(); i += faceSize) {
            int indices[] = {i, i + 1, i + 2};
            if(_useBinary) {
                os.write((char*) &faceSize, sizeof(unsigned char));
                for(int j = 0; j < faceSize; j++) {
                    os.write((char*) &indices[j], sizeof(int));
                }
            } else {
                os << (int) faceSize << " " << indices[0] << " " << indices[1] << " " << indices[2] << std::endl;
            }
        }
    }
    
    os.close();
    return true;
}

bool loadXYZ(std::vector<glm::vec3> &_points, const std::string &_path){
    std::fstream is(_path.c_str(), std::ios::in);
    if(is.is_open()){
        std::string line;
        
        while (std::getline(is,line)) {
            std::vector<std::string> data = splitString(line," ");
            _points.push_back(glm::vec3(toFloat(data[0]),
                                        toFloat(data[1]),
                                        toFloat(data[2])));
        }
        
        is.close();
        return false;
    }
    is.close();
    return false;
}

void saveXYZ(const std::vector<glm::vec3> &_points, std::string _path){
    std::fstream os(_path.c_str(), std::ios::out);
    for (int i = 0; i < _points.size(); i++) {
        os << _points[i].x << " " << _points[i].y << " " << _points[i].z << std::endl;
    }
    os.close();
}
