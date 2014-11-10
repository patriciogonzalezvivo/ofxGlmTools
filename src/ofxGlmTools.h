//
//  ofxGlmMesh.h
//  05-mapBundler
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include "ofMain.h"
#include "glmMesh.h"

ofMesh toOf(const glmMesh &_mesh);
glmMesh toGlm(const ofMesh &_mesh);