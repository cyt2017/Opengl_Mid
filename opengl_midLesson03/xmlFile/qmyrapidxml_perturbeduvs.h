#ifndef QMYRAPIDXML_PERTURBEDUVS_H
#define QMYRAPIDXML_PERTURBEDUVS_H

#include "tool/CELLMath.hpp"
#include "rapidxml.hpp"

class QMyRapidXML_PerturbedUvs
{
public:
    struct  VertexBumpMap
    {
        CELL::float3  _pos;
        CELL::float3  _normal;
        CELL::float2  _uv;
        CELL::float3  _tangent;
    };
    struct  Face
    {
        short   x,y,z;
    };
public:
    std::vector<VertexBumpMap>  _arVertexs;
    std::vector<Face>           _arFaces;
public:
    QMyRapidXML_PerturbedUvs();

    /**
    *   读文件函数，返回文件的内容，需要自己释放内存
    */

    virtual char*   readFile(const char* fileName);

    /**
    *   加载模型文件函数
    */

    virtual bool    load(const char* fileName);

};

#endif // QMYRAPIDXML_PERTURBEDUVS_H
