#include "qmyshadertnl.h"

QMyShaderTNL::QMyShaderTNL()
{
    inVertex        =   -1;
    inNormal        =   -1;
    inTexCoord      =   -1;

    MVPMatrix       =   -1;
    LightDirection  =   -1;
    MaterialBias    =   -1;
    MaterialScale   =   -1;
    sTexture        =   -1;
}

bool QMyShaderTNL::initialize()
{
    const char* vs =
    {
        "attribute vec3 inVertex;"
        "attribute vec3 inNormal;"
        "attribute vec2 inTexCoord;"

        "uniform mat4   MVPMatrix;"
        "uniform vec3   LightDirection;"
        "uniform float  MaterialBias;"
        "uniform float  MaterialScale;"

        "varying vec3   DiffuseLight;"
        "varying vec3   SpecularLight;"
        "varying vec2   TexCoord;"

        "void main()"
        "{"
            "gl_Position    =   MVPMatrix * vec4(inVertex,1);"

            "DiffuseLight   =   vec3(max(dot(inNormal, LightDirection), 0.0));"
            "SpecularLight  =   vec3(max((DiffuseLight.x - MaterialBias) * MaterialScale, 0.0));"

            "TexCoord       =   inTexCoord;"
        "}"
    };
    const char* ps =
    {
        "precision  lowp float; "
        "uniform sampler2D sTexture;"

        "varying vec3  DiffuseLight;"
        "varying vec3  SpecularLight;"
        "varying vec2  TexCoord;"

        "void main()"
        "{"
            "vec3 texColor  =   texture2D(sTexture, TexCoord).rgb;"
            "vec3 color     =   (texColor * DiffuseLight) + SpecularLight;"
            "gl_FragColor   =   vec4(color, 1.0);"
        "}"
    };
    bool res = createProgram(vs,ps);
    if(res)
    {
        inVertex        = glGetAttribLocation(_programId,"inVertex");
        inNormal        = glGetAttribLocation(_programId,"inNormal");
        inTexCoord      = glGetAttribLocation(_programId,"inTexCoord");

        MVPMatrix       = glGetUniformLocation(_programId,"MVPMatrix");
        LightDirection  = glGetUniformLocation(_programId,"LightDirection");
        MaterialBias    = glGetUniformLocation(_programId,"MaterialBias");
        MaterialScale   = glGetUniformLocation(_programId,"MaterialScale");
        sTexture        = glGetUniformLocation(_programId,"sTexture");
    }
}

void QMyShaderTNL::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(inVertex);
    glEnableVertexAttribArray(inNormal);
    glEnableVertexAttribArray(inTexCoord);
}

void QMyShaderTNL::end()
{
    glDisableVertexAttribArray(inVertex);
    glDisableVertexAttribArray(inNormal);
    glDisableVertexAttribArray(inTexCoord);
    glUseProgram(0);
}
