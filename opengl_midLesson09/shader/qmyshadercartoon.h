#ifndef QMYSHADERCARTOON_H
#define QMYSHADERCARTOON_H

#include "tool/programid.h"
class QMyShaderCartoon : public ProgramId
{
public:
    typedef int location;
public:
    location    inVertex;
    location    inNormal;

    location    MVPMatrix;
    location    LightDirection;
    location    EyePosition;
    location    sTexture;
public:
    QMyShaderCartoon();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // QMYSHADERCARTOON_H
