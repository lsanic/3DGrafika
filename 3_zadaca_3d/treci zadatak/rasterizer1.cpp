#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "tgaimage.cpp"
#include "tgaimage.h"
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

float formula(float xa,float ya,float xc,float yc,float x,float y)
{  
   float s = ((ya-yc)*x+(xc-xa)*y+xa*yc-xc*ya);
   return s;
}

void draw_triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2,TGAImage &slika,TGAColor boja){
   // homogene -> klasicne koord 
   // prvo za x i njegovu pripadajucu z koord
    x0 = x0 / z0;
    x1 = x1 / z1;
    x2 = x2 / z2;
   
    //// a zatim i za y
    y0 = y0 / z0;
    y1 = y1 / z1;
    y2 = y2 / z2;

    //zatim radimo transformacije nad x-evima
    x0 = (1 + x0) * width * 0.5;
    x1 = (1 + x1) * width * 0.5;
    x2 = (1 + x2) * width * 0.5;
    //// potom i y-ima
    y0 = (1 + y0) * height * 0.5;
    y1 = (1 + y1) * height * 0.5;
    y2 = (1 + y2) * height * 0.5;

    //prema formuli s predavanja:
    int minx = floor (min(x0,(min(x1,x2)))); 
    int maxx = ceil(max(x0,(max(x1,x2))));
    int miny = floor(min(y0,(min(y1,y2))));
    int maxy = ceil(max(y0,(max(y1,y2))));

    for(float y = miny; y <= maxy; y++){
        for(float x = minx; x <= maxx; x++){
            float alpha = formula(x1, y1, x2, y2, x, y) / formula(x1, y1, x2, y2, x0, y0);
            float beta = formula(x2, y2, x0, y0, x, y) / formula(x2, y2, x0, y0, x1, y1);
            float gamma = formula(x0, y0, x1, y1, x, y) / formula(x0, y0, x1, y1, x2, y2);

            if((alpha >= 0 && beta >= 0 && gamma >= 0) && (alpha <= 1 && beta <= 1 && gamma <= 1))
            {
                
                    set_color(x, y, slika, boja);
                
            }
        }
    }
}

int main()
{
    TGAImage image3(width, height, TGAImage::RGB);

    draw_triangle(-48, -10, 82, 29, -15, 44, 13, 34, 114, image3, blue);

    image3.flip_vertically();
    image3.write_tga_file("trecizadatak.tga");
     


    return 0;
}