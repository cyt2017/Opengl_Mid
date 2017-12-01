#ifndef QMYSHADERLIGHT_H
#define QMYSHADERLIGHT_H

#include "tool/programid.h"

class QMyShaderLight : public ProgramId
{
public:
    typedef int uniform;
    typedef int attribute;
public:
    uniform     _MVP;
    uniform     _matMVT;
    uniform     _lightDir;
    uniform     _texture;
    attribute   _position;
    attribute   _normal;
    attribute   _uv;
public:
    QMyShaderLight();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();

};

#endif // QMYSHADERLIGHT_H
