#ifndef QMYRAPIDXML_FACEANDVERTEX_H
#define QMYRAPIDXML_FACEANDVERTEX_H

#include "rapidxml.hpp"
#include "tool/CELLMath.hpp"

class QMyRapidXML_FaceAndVertex
{
public:
    struct  Face
    {
        short   x,y,z;
    };
    struct Vertex
    {
        float x, y, z;
        float u,v;
        float r, g, b,a;
        float nx,ny,nz;
    };

    std::vector<Face>   _arFaces;
    std::vector<Vertex> _arVertex;
public:
    QMyRapidXML_FaceAndVertex();

    char *readFile(const char *fileName);
    bool load(const char *fileName);

};

#endif // QMYRAPIDXML_FACEANDVERTEX_H
