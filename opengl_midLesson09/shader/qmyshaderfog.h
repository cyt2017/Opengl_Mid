#ifndef QMYSHADERFOG_H
#define QMYSHADERFOG_H

#include "tool/programid.h"

class QMyShaderFog : public ProgramId
{
public:
    typedef int attribute;
    typedef int uniform;
public:
    attribute   inVertex;
    attribute   inNormal;
    attribute   inTexCoord;

    uniform     MVPMatrix;
    uniform     ModelViewMatrix;
    uniform     LightDirection;
    uniform     iFogMode;
    uniform     EyePositon;
    uniform     FogDensity;
    uniform     FogEnd;
    uniform     FogRcpEndStartDiff;
    uniform     sTexture;
    uniform     FogColor;
public:
    QMyShaderFog();

    virtual bool    initialize();
    virtual void    begin();
    virtual void    end();
};

#endif // QMYSHADERFOG_H
