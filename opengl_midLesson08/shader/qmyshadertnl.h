#ifndef QMYSHADERTNL_H
#define QMYSHADERTNL_H

#include "tool/programid.h"

class QMyShaderTNL : public ProgramId
{
public:
    typedef int uniform;
    typedef int attribute;
public:
    attribute   inVertex;
    attribute   inNormal;
    attribute   inTexCoord;

    uniform     MVPMatrix;
    uniform     LightDirection;
    uniform     MaterialBias;
    uniform     MaterialScale;
    uniform     sTexture;
public:
    QMyShaderTNL();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // QMYSHADERTNL_H
