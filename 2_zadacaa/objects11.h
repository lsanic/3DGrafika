#pragma once
#include <cmath>
#include "geometry.h"
#include "material.h"
#include <iostream>
using namespace std;
///iz nekog razloga ne radi ray trace iako objects i objects1 rade
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

struct Cuboid : Object{
    Vec3f T1;
    Vec3f T2;

    Cuboid(const Vec3f &T1, const Vec3f &T2, const Material &materijal): T1(T1), T2(T2) {
        Object::material = materijal;
    }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normala) const{
        /// najveci i najmanji float
        float mint = numeric_limits<float>::min(); 
        float maxt = numeric_limits<float>::max(); 

        ///// cilj nam je pronaci "okvir" naseg kvadra tj pravce paralelne s osima te provjeriti sijece li njih nas zadani pravac
        ////// buduci da pravac definiramo pomocu e + dt umjesto toga uvrstimo npr. za x komponentu naseg okvira : e + dt = T1.x
        float minx = min(T1.x, T2.x);
        float maxx = max(T1.x, T2.x);
        float miny = min(T1.y, T2.y);
        float maxy = max(T1.y, T2.y);
        float minz = min(T1.z, T2.z);
        float maxz = max(T1.z, T2.z);
        
        //ako nam je d = 0 onda promatramo e tj izvor pogleda nase zrake, ukoliko je za x veca od najveceg x-a i manja od najmanjeg=> ne sijece promatrani "okvir"
        if(ray.direction.x == 0){
            if(ray.origin.x < min_x || ray.origin.x > max_x){
                return false; 
            }
        }
        
        /// promatramo e + dt = promatrana komponenta (moze biti min il max), kada to podijelimo s d dobijemo : t = ((komponenta)-e )/ d
        else
        {
            float t1 = (minx - ray.origin.x) / ray.direction.x;
            float t2 = (maxx - ray.origin.x) / ray.direction.x;

            if(t1 > t2){
                swap(t1, t2);
            }

            t_min = max(mint, t1);
            t_max = min(maxt, t2); 

            if(mint > maxt || maxt < 0){
                return false;
            }
        }

        t = mint;

        // isto radimo za y pomocu formule t = (komponenta od y - e)/ d
        if(ray.direction.y == 0){
            if(ray.origin.y < miny || ray.origin.y > maxy){
                return false;
            }
        }else{
            float t1 = (miny - ray.origin.y) / ray.direction.y;
            float t2 = (maxy - ray.origin.y) / ray.direction.y;

            if(t1 > t2){
                swap(t1,t2);
            }

            mint = max(mint, t1);
            maxt = min(maxt, t2);

            if(mint > maxt || maxt < 0){
                return false;
            }
        }

        t = mint;

        //z
        if(ray.direction.z == 0){
            if(ray.origin.z < minz || ray.origin.z > maxz){
                return false;
            }
        }else{
            float t1 = (minz - ray.origin.z) / ray.direction.z;
            float t2 = (maxz - ray.origin.z) / ray.direction.z;

            if(t1 > t2){
                swap(t1, t2);
            }

            mint = max(mint, t1);
            maxt = min(maxt, t2);

            if(mint > maxt || maxt < 0){
                return false;
            }
        }

        t = mint;

        //izracun normale

        if(abs(ray.origin.x - T1.x) < 0.01){
            normala = Vec3f(1, 0, 0);
        }else if(abs(ray.origin.x - T2.x) < 0.01){
            normala = Vec3f(-1, 0, 0);
        }else if(abs(ray.origin.y - T1.y) < 0.01){
            normala = Vec3f(0, 1, 0);
        }else if(abs(ray.origin.y - T2.y) < 0.01){
            normala = Vec3f(0, -1, 0);
        }else if(abs(ray.origin.z - T1.z) < 0.01){
            normala = Vec3f(0, 0, 1);
        }else if(abs(ray.origin.z - T2.z) < 0.01){
            normala = Vec3f(0, 0, -1);
        }


        return true;
    }
};
