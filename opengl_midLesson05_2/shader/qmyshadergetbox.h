#ifndef QMYSHADERGETBOX_H
#define QMYSHADERGETBOX_H

#include "tool/programid.h"

class QMyShaderGetBox : public ProgramId
{
public:
        typedef int uniform;
        typedef int attribute;
public:
    attribute   _position;
    attribute   _normal;

    uniform     _MVP;
    uniform     _matMVT;
    uniform     _eyePos;
    uniform     _lightVecter;
public:
    QMyShaderGetBox();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // QMYSHADERGETBOX_H
