//
//  ofxGlmMesh.h
//  05-mapBundler
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGlm.h"

#include "glmRectangle.h"
#include "glmPolyline.h"
#include "glmMesh.h"

ofRectangle toOf(const glmRectangle &_rect);
glmRectangle toGlm(const ofRectangle &_rect);

ofPolyline toOf(const glmPolyline &_poly);
glmPolyline toGlm(const ofPolyline &_poly);

ofMesh toOf(const glmMesh &_mesh);
glmMesh toGlm(const ofMesh &_mesh);