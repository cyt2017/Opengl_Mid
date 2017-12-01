#ifndef QMYRAPIDXML_H
#define QMYRAPIDXML_H

#include "rapidxml.hpp"
#include "tool/CELLMath.hpp"

class QMyRapidXML
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

    class   Model
    {
    public:
        std::string         _name;
        std::vector<Face>   _arFaces;
        std::vector<Vertex> _arVertex;

    };

    class   Node
    {
    public:
        std::string     _name;
        std::string     _fileName;
        CELL::float3    _pos;
        CELL::float3    _scale;
        CELL::float3    _rot;
    };


    std::map<std::string,Model> _map;

    std::vector<Node>           _arNode;
public:
    QMyRapidXML();

    virtual char*   readFile(const char* fileName);
    virtual bool    loadScene(const char* fileScene);
    virtual bool    load(const char* fileName,Model& mode);
};

#endif // QMYRAPIDXML_H
