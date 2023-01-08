#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "tgaimage.h"
#include "tgaimage.cpp"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 1);

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);    
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void line_naive(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0; t < 1; t += 0.01)
    {
        int x = x0 * (1.0f - t) + x1 * t;
        int y = x0 * (1.0f - t) + y1 * t;
        set_color(x, y, image, color);
    }
}

void line_midpoint(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, bool invert)
{
    int y = y0;
    int d = line(x0, y0, x1, y1, x0 + 1, y0 + 0.5);
    
    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int increment = 1;
    
    if (dy < 0)
    {
        // pravac ide od gore prema dolje
        dy = -dy;
        increment = -increment;
    }
    
    for (int x = x0; x <= x1; ++x)
    {
        if (invert)
        {
            set_color(x, y, image, color);
        }
        else
        {
            set_color(y, x, image, color);       
        }
        
        if (d < 0)
        {
            y = y + increment;
            d = d + dx - dy;
        }
        else
        {
            d = d - dy;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // 'transponiraj' duzinu ako je veci od 1
    bool invert = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        invert = true;
    }
    
    // zamijeni tocke ako pravac ide s desna na lijevo
    if (x1 < x0)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    
    // nacrtaj duzinu
    line_midpoint(x0, y0, x1, y1, image, color, invert);
}


//////// ZADACA ////////
/// izracun baricentricnih koord uz pomoc pravaca
float formula(float xa,float ya,float xc,float yc,float x,float y)
{  
   float s = ((ya-yc)*x+(xc-xa)*y+xa*yc-xc*ya);
   return s;
}

////// PRVI ZADATAK
void draw_triangle_2d( float x0, float y0, float x1, float y1, float x2, float y2, TGAImage &slika, TGAColor boja)
{
    //prema formuli s predavanja:
    int minx = floor (min(x0,(min(x1,x2)))); 
    int maxx = ceil(max(x0,(max(x1,x2))));
    int miny = floor(min(y0,(min(y1,y2))));
    int maxy = ceil(max(y0,(max(y1,y2))));
    for(int  y = miny ; y <= maxy ; y++)
    {
        for(int x = minx ; x <= maxx ; x++)
        {
            float alfa=formula(x1,y1,x2,y2,x,y)/formula(x1,y1,x2,y2,x0,y0); 
            float beta=formula(x2,y2,x0,y0,x,y)/formula(x2,y2,x0,y0,x1,y1); 
            float gama=formula(x0,y0,x1,y1,x,y)/formula(x0,y0,x1,y1,x2,y2); 

            if((alfa >= 0 && alfa <= 1) && (beta >= 0 && beta <= 1) && (gama >= 0 && gama <= 1))
            {
               if(((alfa > 0) || (formula(x1,y1,x2,y2, x0,y0)*formula(x1,y1, x2,y2, -1,-1))>=0)  &&
                 (beta > 0) || (formula(x2,y2,x0,y0, x1,y1)*formula(x2,y2, x0,y0, -1,-1)>=0) &&
                 (gama > 0) || (formula(x0,y0,x1,y1, x2,y2)*formula(x0,y0, x1,y1, -1,-1)>=0))
                slika.set(x,y, boja);
            }
        }
    }
}

////// DRUGI ZADATAK
void draw_triangle_2d_gouraurd(float x0,float y0, float x1, float y1, float x2, float y2, TGAImage &slika, TGAColor c0, TGAColor c1, TGAColor c2)
{
  //prema formuli s predavanja:
    int minx = floor (min(x0,(min(x1,x2)))); 
    int maxx = ceil(max(x0,(max(x1,x2))));
    int miny = floor(min(y0,(min(y1,y2))));
    int maxy = ceil(max(y0,(max(y1,y2))));
    TGAColor boja;

    for(int  y = miny ; y <= maxy ; y++)
    {
        for(int x = minx ; x <= maxx ; x++)
        {   
            float alfa=formula(x1,y1,x2,y2,x,y)/formula(x1,y1,x2,y2,x0,y0); 
            float beta=formula(x2,y2,x0,y0,x,y)/formula(x2,y2,x0,y0,x1,y1); 
            float gama=formula(x0,y0,x1,y1,x,y)/formula(x0,y0,x1,y1,x2,y2); 

            if((alfa >= 0 && alfa <= 1) && (beta >= 0 && beta <= 1) && (gama >= 0 && gama <= 1))
            {
               if(((alfa > 0) || (formula(x1,y1,x2,y2, x0,y0)*formula(x1,y1, x2,y2, -1,-1))>=0)  &&
                 (beta > 0) || (formula(x2,y2,x0,y0, x1,y1)*formula(x2,y2, x0,y0, -1,-1)>=0) &&
                 (gama > 0) || (formula(x0,y0,x1,y1, x2,y2)*formula(x0,y0, x1,y1, -1,-1)>=0))
                {
                    boja.r=alfa*c0.r+beta*c1.r+gama*c2.r;
                    boja.g=alfa*c0.g+beta*c1.g+gama*c2.g;
                    boja.b=alfa*c0.b+beta*c1.b+gama*c2.b;
                    slika.set(x,y,boja);
                }
            }
        }
    }
}



int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);
    
   
    draw_triangle_2d(12,14,50,60,70,80, image, blue);
    draw_triangle_2d(70,80,100,120,50,80, image, red);
    draw_triangle_2d_gouraurd(88, 92, 44, 55, 70,80, image, red,green,blue);

     
    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("prvadvazadatka.tga");


    return 0;
}