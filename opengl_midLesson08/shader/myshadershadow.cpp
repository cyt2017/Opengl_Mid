#include "myshadershadow.h"

MyShaderShadow::MyShaderShadow()
{
    _MVP        =   -1;
    _matMV      =   -1;
    _matMVT     =   -1;
    _lightPos   =   -1;
    _lightDir   =   -1;
    _lightColor =   -1;
    _texture    =   -1;

    _position   =   -1;
    _normal     =   -1;
    _uv         =   -1;
}

bool MyShaderShadow::initialize()
{
    const char* vs  =
    {
        "attribute vec3  inVertex; "
        "attribute vec3  inNormal;"
        "attribute vec2  inTexCoord;"

        "uniform mat4  MVPMatrix;"
        "uniform mat4  ModelView;"
        "uniform mat3  ModelViewIT;"
        "uniform vec3  LightPosition;"
        "uniform vec3  LightDirection;"
        "uniform vec3  LightColor;"

        "varying vec2  TexCoord;"
        "varying vec3  DiffuseLight;"
        "varying vec3  SpecularLight;"
        "const float  cShininess = 16.0;"
        "const float  cSpotCutoff = 0.9; "
        "const float  cSpotExp = 40.0;"

        "void main(void)"
        "{"
          // transform normal to eye space
          "vec3 normal = normalize(ModelViewIT * inNormal);"

          // transform vertex position to eye space
          "vec3 ecPosition = vec3(ModelView * vec4(inVertex, 1.0));"

          // initalize light intensity varyings
          "DiffuseLight   =   vec3(0.0);"
          "SpecularLight  =   vec3(0.0);"
          "highp vec3   lightDir=   -normalize(ecPosition - LightPosition);"
          "highp vec3   eyeDir  =   -normalize(ecPosition);"

          "highp float  spotDot     =   dot(lightDir, LightDirection);"
          "highp float  attenuation = 0.0;"
          "if (spotDot > cSpotCutoff)"
          "{"
              "attenuation = pow(spotDot, cSpotExp);"
          "}"
          "vec3   lightColor  =   attenuation * LightColor;"


          "float  NdotL   =   max(dot(normal, lightDir), 0.0);"
          "DiffuseLight   +=  NdotL * lightColor;"

          "if (NdotL > 0.0)"
          "{"
              "vec3 halfVector = normalize(lightDir + eyeDir);"
              "float NdotH = max(dot(normal, halfVector), 0.0);	"
              "float specular = pow(NdotH, cShininess);"
              "SpecularLight += specular * lightColor;"
          "}	"

          "gl_Position    =   MVPMatrix * vec4(inVertex,1);"

          "TexCoord       =   inTexCoord;"
        "}"

    };
    const char* ps  =
    {
        "precision  lowp float; "
        "uniform sampler2D  sTexture;"

        "varying vec2  TexCoord;"
        "varying vec3  DiffuseLight;"
        "varying vec3  SpecularLight;"

        "void main()"
        "{"
            "vec3 texColor  = vec3(texture2D(sTexture, TexCoord));"
            "vec3 color = (texColor * DiffuseLight) + SpecularLight;"
            "gl_FragColor = vec4(color, 1.0);"
        "}"
    };
    bool    res =   createProgram(vs,ps);
    if(res)
    {
        _position   =   glGetAttribLocation(_programId,"inVertex");
        _normal     =   glGetAttribLocation(_programId,"inNormal");
        _uv         =   glGetAttribLocation(_programId,"inTexCoord");

        _matMV      =   glGetUniformLocation(_programId,"ModelView");
        _MVP        =   glGetUniformLocation(_programId,"MVPMatrix");
        _matMVT     =   glGetUniformLocation(_programId,"ModelViewIT");
        _lightPos   =   glGetUniformLocation(_programId,"LightPosition");
        _lightDir   =   glGetUniformLocation(_programId,"LightDirection");
        _lightColor =   glGetUniformLocation(_programId,"LightColor");
        _texture    =   glGetUniformLocation(_programId,"sTexture");
    }
    return  res;
}

void MyShaderShadow::begin()
{
    glUseProgram(_programId);
    //!在显卡里面使用的局部变量，在使用时是需要进行使能和关闭的
    glEnableVertexAttribArray(_position );
    glEnableVertexAttribArray(_normal);
    glEnableVertexAttribArray(_uv);
}

void MyShaderShadow::end()
{
    glDisableVertexAttribArray(_position );
    glDisableVertexAttribArray(_normal);
    glDisableVertexAttribArray(_uv);
    glUseProgram(0);
}
