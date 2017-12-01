#ifndef QMYSHADERNORMALIMAGE_H
#define QMYSHADERNORMALIMAGE_H

#include "tool/programid.h"

class QMyShaderNormalImage : public ProgramId
{
public:
    typedef int location;
public:
    location    _position;
    location    _normal;
    location    _uv;
    location    _tangent;

    location    _MVP;
    location    _lightPos;
    location    _texNor;
    location    _texNor1;
public:
    QMyShaderNormalImage();

    virtual bool initialize();
    virtual void begin();
    virtual void end();
};

#endif // QMYSHADERNORMALIMAGE_H
