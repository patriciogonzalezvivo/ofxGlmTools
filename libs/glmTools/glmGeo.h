//
//  glmGeom.cpp
//  05-mapBundler
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include "glm.hpp"

#ifndef R_EARTH
#define R_EARTH 6378137
#endif

#include <math.h>

inline double y2lat(double y) { return glm::degrees(2.0 * atan(exp(glm::radians(y))) - M_PI / 2.0); }
inline double x2lon(double x) { return glm::degrees(x / R_EARTH); }
inline double lat2y(double lat) { return R_EARTH * std::log(tan(M_PI/ 4.0 + glm::radians(lat) / 2.0)); }
inline double lon2x(double lon) { return glm::radians(lon) * R_EARTH; }

inline int long2tilex(double lon, int z){ return (int)(std::floor((lon + 180.0) / 360.0 * std::pow(2.0, z)));}
inline int lat2tiley(double lat, int z){ return (int)(std::floor((1.0 - std::log( std::tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * std::pow(2.0, z))); }

inline glm::vec3 tile2xy(int _tileX, int _tileY, int _zoom ){
    float n = powf(2.0f, _zoom);
    glm::vec3 rta;
    rta.x = lon2x((_tileX + 0.5) / n * 360.0f - 180.0f);
    rta.y = lat2y(atanf(sinhf(M_PI*(1-2*(_tileY+0.5)/n))) * 180.0f / M_PI);
    return rta;
}