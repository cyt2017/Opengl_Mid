#include "qmyshaderfog.h"

QMyShaderFog::QMyShaderFog()
{
    inVertex    =   -1;
    inNormal    =   -1;
    inTexCoord  =   -1;

    MVPMatrix           =   -1;
    ModelViewMatrix     =   -1;
    LightDirection      =   -1;
    iFogMode            =   -1;
    EyePositon          =   -1;
    FogDensity          =   -1;
    FogEnd              =   -1;
    FogRcpEndStartDiff  =   -1;
    sTexture            =   -1;
    FogColor            =   -1;
}

bool QMyShaderFog::initialize()
{
    const char* vs  =
    {
        "attribute vec3  inVertex;"
        "attribute vec3  inNormal;"
        "attribute vec2  inTexCoord;"

        "uniform mat4  MVPMatrix;"
        "uniform mat4  ModelViewMatrix;"
        "uniform vec3  LightDirection;"
        // fog uniforms
        "uniform float  FogDensity;"
        "uniform float  FogEnd;"

        "varying vec2  TexCoord;"
        "varying vec3  DiffuseLight;"
        "varying vec3  FogIntensity;"

        "void main()"
        "{"
            //! 这里需要将顶点转化到摄像机空间，目的是用来计算摄像机与顶点之间的距离
            "vec3   viewPos = vec3(ModelViewMatrix * vec4(inVertex,1));"
            "float  eyeDist = length(viewPos);"


            // transform vertex position
            "gl_Position = MVPMatrix * vec4(inVertex,1);"

            "TexCoord = inTexCoord;"

            "float DiffuseIntensity = dot(inNormal, normalize(LightDirection));"

            // clamp negative values and add some ambient light
            "DiffuseLight = vec3(max(DiffuseIntensity, 0.0) * 0.5 + 0.5);"

            "float fogIntensity = 1.0;"

            "fogIntensity   =   (FogEnd - eyeDist) * FogDensity;"
            "fogIntensity   =   clamp(fogIntensity, 0.0, 1.0);"
            "FogIntensity   =   vec3(fogIntensity,fogIntensity,fogIntensity);"
        "}"


    };
    const char* ps  =
    {
        "precision  lowp float; "
        "uniform    sampler2D  sTexture;"

        "uniform    vec3  FogColor;"
        "varying    vec2  TexCoord;"
        "varying    vec3  DiffuseLight;"
        "varying    vec3  FogIntensity;"

        "void main()"
        "{"
            "vec3 texColor  = texture2D(sTexture, TexCoord).rgb;"
            "vec3 color = texColor * DiffuseLight;"

            "color = mix(FogColor, color, FogIntensity);"
            "gl_FragColor = vec4(color, 1.0);"
        "}"

    };

    bool res = createProgram(vs,ps);
    if(res)
    {
        inVertex    =   glGetAttribLocation(_programId, "inVertex");
        inNormal    =   glGetAttribLocation(_programId, "inNormal");
        inTexCoord  =   glGetAttribLocation(_programId, "inTexCoord");

        EyePositon          =   glGetUniformLocation(_programId,"EyePositon");
        MVPMatrix           =   glGetUniformLocation(_programId,"MVPMatrix");
        ModelViewMatrix     =   glGetUniformLocation(_programId,"ModelViewMatrix");
        LightDirection      =   glGetUniformLocation(_programId,"LightDirection");
        iFogMode            =   glGetUniformLocation(_programId,"iFogMode");
        FogDensity          =   glGetUniformLocation(_programId,"FogDensity");
        FogEnd              =   glGetUniformLocation(_programId,"FogEnd");
        FogRcpEndStartDiff  =   glGetUniformLocation(_programId,"FogRcpEndStartDiff");
        sTexture            =   glGetUniformLocation(_programId,"sTexture");
        FogColor            =   glGetUniformLocation(_programId,"FogColor");
    }

    return  true;
}

void QMyShaderFog::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(inVertex);
    glEnableVertexAttribArray(inNormal);
    glEnableVertexAttribArray(inTexCoord);
}

void QMyShaderFog::end()
{
    glDisableVertexAttribArray(inVertex);
    glDisableVertexAttribArray(inNormal);
    glDisableVertexAttribArray(inTexCoord);
    glUseProgram(0);
}
