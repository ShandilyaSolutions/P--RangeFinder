/*While calling this function pass 3 float pointers too for getting back the values.
    float X,Y,Z;
    scalarMulti(x,y,z,c,X,Y,Z)*/
float scalarMulti(float x, float y, float z, float c, float *X, float *Y, float *Z)
{
    //This function performs the scalar multiplications and return the new vector
    *X = c*x;
    *Y = c*y;
    *Z = c*z;
}
