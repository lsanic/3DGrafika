#pragma once
#include "geometry.h"

struct Material
{  
    Vec3f diffuse_color;
    
    float diffuse_coef = 1;
    float specular_coef = 0;
    float phong_exp = 1;
    
    Material(const Vec3f &color) : diffuse_color(color) {}
    Material() : diffuse_color(Vec3f(0, 0, 0)) {}
};