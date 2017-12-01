#ifndef QMYSHADERANIMA_H
#define QMYSHADERANIMA_H

#include "tool/programid.h"

class QMyShaderAnima : public ProgramId
{
public:
    typedef int location;
public:
    location    inVertex;
    location    inNormal;

    location    MVPMatrix;
    location    myAnim;
    location    LightDirection;
    location    EyePosition;
    location    sTexture;
public:
    QMyShaderAnima();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // QMYSHADERANIMA_H
