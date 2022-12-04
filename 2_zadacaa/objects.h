#pragma once
#include <cmath>
#include "geometry.h"
#include "material.h"
#include "ray.h"

#include <iostream>
using namespace std;

struct Object
{
    Material material;
    
    virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;    
};

struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius
    
    Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r)
    {
        Object::material = mat;
    }
    
    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {
        float d2 = ray.direction * ray.direction;
        Vec3f e_minus_c = ray.origin - c;
        
        float disc = pow(ray.direction * e_minus_c, 2) - d2 * (e_minus_c * e_minus_c - r * r);
        
        bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
        
        if (disc < 0)
        {
            return false;            
        }
        else
        {
            if (ray_inside_sphere)
            {
                t = (-ray.direction * e_minus_c + sqrt(disc))/d2;
            }
            else
            {
                t = (-ray.direction * e_minus_c - sqrt(disc))/d2;                
            }
            
            Vec3f normal_origin = ray.origin + ray.direction * t;
            normal = (normal_origin - c).normalize();
            
            return t > 0;
        }
    }
};

struct Cuboid : Object
{
    Vec3f T1; /// tocka (x min,y min,z min)
    
    Vec3f T2;  /// tocka (x'max,y'max,z'max)

    Cuboid(const Vec3f &T1, const Vec3f &T2, const Material &materijal) : T1(T1), T2(T2)
    {
        Object::material = materijal;
    }

    bool ray_intersect(const Ray &ray, const Vec3f &normala) const
    {
   
        //// ray.direction  / smjer pogleda tj d s predavanja
        ////  ray.origin  / izvor pogleda, e
        
        ///// cilj nam je pronaci "okvir" naseg kvadra tj pravce paralelne s osima te provjeriti sijece li njih nas zadani pravac
        ////// buduci da pravac definiramo pomocu e + dt umjesto toga uvrstimo npr. za x komponentu naseg okvira : e + dt = T1.x
        float minX = min(T1.x, T2.x);  /// trazimo manju x komponentu
        float maxX = max(T1.x, T2.x);  /// trazimo vecu x komponentu
        /// jednak postupak za y te za z
        float minY = min(T1.y, T2.y);
        float maxY = max(T1.y, T2.y);

        float minZ = min(T1.z, T2.z);
        float maxZ = max(T1.z, T2.z); 
    
    ///racunamo pomocu formule e + dt = (odsjecak na osi koju promatramo) tada jednadzbu podijelimo s d 
    /// sto je nas smjer pogleda ili ray.direction i dobijemo x(min/max) = (((min/max)odsjecak na osi) - e)/ d, x0 uzimamo za min, a x1 za max sto imamo ovdje:  
    float x0, x1, y0, y1, z0, z1; 
 
    x0 = ((minX - ray.origin.x) * (1/ray.direction.x)); /// u slucaju da dijelimo s nulom
    x1 = (maxX - ray.origin.x) *(1/ray.direction.x) ; 

    y0 = (minY - ray.origin.y) *(1/ray.direction.y); 
    y1 = (maxY - ray.origin.y) * (1/ray.direction.y);
    
    z0 = (minZ - ray.origin.z) * (1/ray.direction.z); 
    z1 = (maxZ - ray.origin.z) * (1/ray.direction.z); 
 
   
    if ((x0 > y1) || (y0 > x1)) 
        return false; 
 
    if (y0 > x0) 
        x0 = y0; //// minimum postavi na veci minimum
    if (y1 < x1) 
        x1 = y1; 
 
    if ((x0 > z1) || (z0 > x1)) 
        return false; 
    //// postavi minimum na onaj veci
    if (z0 > x0) 
        x0 = z0; 
    /// postavi maksimum na onaj manji
    if (z1 < x1) 
        x1 = z1; 
 
    
}

   Vec3f normala(const Vec3f &n) const
   {
    Vec3f normala;

    if(abs(n.x-T1.x)<0.01)
       normala = Vec3f(-1,0,0);
    else if(abs(n.x-T2.x)<0.01)
       normala = Vec3f(1,0,0);
    else if(abs(n.y-T1.y)<0.01)
       normala = Vec3f(0,-1,0);
    else if(abs(n.y-T2.y)<0.01)
       normala = Vec3f(0,1,0);
    else if(abs(n.z-T1.z)<0.01)
       normala = Vec3f(0,0,-1);
    else if(abs(n.z-T2.z)<0.01)
       normala = Vec3f(0,0,1);
    return normala;
    
   }
};
