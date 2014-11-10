//
//  glmString.h
//
//  Created by Patricio Gonzalez Vivo on 11/6/14.
//
//

#pragma once

#include "glm.hpp"

#include <iostream>
#include <sstream>

inline std::string toString(const int &_int){
    std::ostringstream strStream;
    strStream<<_int;
    return strStream.str();
}

inline std::string toString(const float &_float){
    std::ostringstream strStream;
    strStream<<_float;
    return strStream.str();
}

inline std::string toString(const double &_double){
    std::ostringstream strStream;
    strStream<<_double;
    return strStream.str();
}

inline std::string toString(const char &_char){
    std::stringstream strStream;
    strStream<<_char;
    return strStream.str();
}

inline void stringPurifier ( std::string& s ){
    for ( std::string::iterator it = s.begin(), itEnd = s.end(); it!=itEnd; ++it){
        if ( static_cast<unsigned int>(*it) < 32 || static_cast<unsigned int>(*it) > 127 ){
            (*it) = ' ';
        }
    }
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << vec.x << ", " << vec.y << ", " << vec.z;
    return os;
}

inline std::istream& operator>>(std::istream& is, glm::vec3& vec) {
    is >> vec.x;
    is.ignore(2);
    is >> vec.y;
    is.ignore(2);
    is >> vec.z;
    return is;
}