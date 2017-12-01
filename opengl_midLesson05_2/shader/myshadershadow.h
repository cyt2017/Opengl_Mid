#ifndef MYSHADERSHADOW_H
#define MYSHADERSHADOW_H

#include "tool/programid.h"

class MyShaderShadow : public ProgramId
{
public:
    typedef int attribute;
    typedef int uniform;
public:
    uniform     _MVP;
    uniform     _matMV;
    uniform     _matMVT;
    uniform     _lightPos;
    uniform     _lightDir;
    uniform     _lightColor;
    uniform     _texture;
    attribute   _position;
    attribute   _normal;
    attribute   _uv;
public:
    MyShaderShadow();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // MYSHADERSHADOW_H
