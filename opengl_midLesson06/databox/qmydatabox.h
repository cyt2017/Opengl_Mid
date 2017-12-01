#ifndef QMYDATABOX_H
#define QMYDATABOX_H
#include <string.h>

class QMyDataBox
{
 public:
    struct Vertex
    {
        float x, y, z;
        float u,v;
        float r, g, b,a;
        float nx,ny,nz;
    };
public:
    QMyDataBox();
    Vertex  _boxVertex[36];

};

#endif // QMYDATABOX_H
