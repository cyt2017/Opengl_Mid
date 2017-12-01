#include "qmyshadercartoon.h"

QMyShaderCartoon::QMyShaderCartoon()
{
    inVertex    =   -1;
    inNormal    =   -1;

    MVPMatrix   =   -1;
    LightDirection  =   -1;
    EyePosition =   -1;
    sTexture    =   -1;
}

bool QMyShaderCartoon::initialize()
{
    //!利用眼睛方向和灯光方向与法线的点积获得文理输出的坐标
    const char* vs  =
    {
        "attribute vec3  inVertex;"
        "attribute vec3  inNormal;"

        "uniform mat4  MVPMatrix;"
        "uniform vec3  LightDirection;"
        "uniform vec3  EyePosition;"

        "varying vec2  TexCoord;"

        "void main()"
        "{"
            "gl_Position = MVPMatrix * vec4(inVertex,1.0);"

            "vec3 eyeDirection = normalize(EyePosition - inVertex);"

            "TexCoord.x = dot(LightDirection, inNormal);"
            "TexCoord.y = dot(eyeDirection, inNormal);"
        "}"

    };
    const char* ps  =
    {
        "precision  lowp float; "
        "uniform sampler2D  sTexture;"

        "varying vec2  TexCoord;"

        "const vec3  cBaseColor = vec3(0.2, 1.0, 0.7);"

        "void main()"
        "{"
            "gl_FragColor = vec4(cBaseColor * texture2D(sTexture, TexCoord).rgb, 1.0);"
        "}"
    };

    bool res = createProgram(vs,ps);
    if(res)
    {
        inVertex    = glGetAttribLocation(_programId, "inVertex");
        inNormal    = glGetAttribLocation(_programId, "inNormal");

        MVPMatrix       = glGetUniformLocation(_programId,"MVPMatrix");
        LightDirection  = glGetUniformLocation(_programId,"LightDirection");
        EyePosition     = glGetUniformLocation(_programId,"EyePosition");
        sTexture        = glGetUniformLocation(_programId,"sTexture");
    }
    return  true;
}

void QMyShaderCartoon::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(inVertex);
    glEnableVertexAttribArray(inNormal);
}

void QMyShaderCartoon::end()
{
    glDisableVertexAttribArray(inVertex);
    glDisableVertexAttribArray(inNormal);
    glUseProgram(0);
}
