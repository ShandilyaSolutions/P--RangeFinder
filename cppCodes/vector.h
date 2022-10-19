/*This is the header file for vector library written by Amritanshu Shiv Shandilya*/


void scalarMulti(float x, float y, float z, float c, float *X, float *Y, float *Z)
{
    /*This function does scalar multiplication.
        Note : while calling this function pass three float pointers as arguments : 
        scalarMulti(x,y,z,c,&X,&Y,&Z)*/
    *X = c*x;
    *Y = c*y;
    *Z = c*Z;
}

void display(float x, float y, float z)
{
    /*This function displays the vector in easily readable format : 
        xi^+yj^+zk^  with proper signs.
    Syntax : display(x,y,z)*/
    if (y < 0.0 && z < 0.0)
        cout<<x<<"i^ - "<<y<<"j^ - "<<z<<"k^"<<endl;
    else if (y < 0.0)
        cout<<x<<"i^ - "<<y<<"j^ + "<<z<<"k^"<<endl;
    else if (z < 0.0)
        cout<<x<<"i^ + "<<y<<"j^ - "<<z<<"k^"<<endl;
}
