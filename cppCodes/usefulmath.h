/*Library holding all the necessary functions for mathematics*/
#include <cmath>


float distance2d (float x1, float y1, float x2, float y2)
{
    /*Distance formula for 2d cordinate system*/
    float dist;
    dist = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1));
    return (sqrt(dist));
}


float distance3d(float x1, float y1, float z1, float x2, float y2, float z2)
{
    //Distance formula for 3d cordinate system
    float dist;
    dist = ((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1))+((z2-z1)*(z2-z1));
    return (sqrtf(dist));
}


float distanceOrigin(float x, float y, float z)
{
    //Distance of a point from Origin of cordinate system
    float dist;
    dist = (x*x)+(y*y)+(z*z);
    return dist;
}
