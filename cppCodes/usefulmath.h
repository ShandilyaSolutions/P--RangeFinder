/*Library holding all the necessary functions for mathematics*/
#include <cmath>


float distance2d (float x1, float y1, float x2, float y2)
{
    /*Square root for 2d cordinate system*/
    float dist;
    dist = (x2-x1)+(y2-y1);
    return (sqrt(dist));
}


float distance3d(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dist;
    dist = (x2-x1)+(y2-y1)+(z2-z1);
    return (sqrtf(dist));
}
