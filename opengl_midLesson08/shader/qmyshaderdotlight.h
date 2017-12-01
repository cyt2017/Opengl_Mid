#ifndef QMYSHADERDOTLIGHT_H
#define QMYSHADERDOTLIGHT_H

#include "tool/programid.h"
class QMyShaderDotlight : public ProgramId
{
public:
    typedef int     uniform;
    typedef int     attribute;
public:
    uniform     _MVP;
    uniform     _matMVT;
    uniform     _matMV;
    uniform     _lightPos;
    uniform     _lightDir;
    uniform     _lightColor;
    uniform     _texture;
    attribute   _position;
    attribute   _uv;
    attribute   _normal;
public:
    QMyShaderDotlight();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // QMYSHADERDOTLIGHT_H
