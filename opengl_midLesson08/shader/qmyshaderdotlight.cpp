#include "qmyshaderdotlight.h"

QMyShaderDotlight::QMyShaderDotlight()
{
    _MVP        =   -1;
    _matMVT     =   -1;
    _matMV      =   -1;
    _lightPos   =   -1;
    _lightDir   =   -1;
    _lightColor =   -1;
    _texture    =   -1;
    _position   =   -1;
    _uv         =   -1;
    _normal     =   -1;
}

bool QMyShaderDotlight::initialize()
{
    const char* vs =
    {
        "attribute vec3  _position; "
        "attribute vec3  _normal;"
        "attribute vec2  _uv;"

        "uniform mat4  _MVP;"
        "uniform mat4  _matMV;"
        "uniform mat4  _matMVT;"
        "uniform vec3  _lightPos;"
        "uniform vec3  _lightDir;"
        "uniform vec3  _lightColor;"

        "varying vec2  _outUV;"
        "varying vec3  DiffuseLight;"
        "varying vec3  SpecularLight;"
        "const  float  cShininess = 16.0;"

        "void main()"
        "{"
            // transform normal to eye space
            "vec3 normal = normalize((_matMVT * vec4(_normal,0.0)).xyz);"

            // transform vertex position to eye space
            "vec3 ecPosition = (_matMV * vec4(_position, 1.0)).xyz;"

            // initalize light intensity varyings
            "DiffuseLight   =   vec3(0.0,0.0,0.0);"
            "SpecularLight  =   vec3(0.0,0.0,0.0);"

            "vec3    eyeDir =   -normalize(ecPosition);"
            "float   NdotL  =   max(dot(normal, _lightDir), 0.0);"
            "DiffuseLight   +=  NdotL * _lightColor;"

            "if (NdotL > 0.0)"
            "{"
                "vec3 halfVector= normalize(_lightDir + eyeDir);"
                "float NdotH    = max(dot(normal, halfVector), 0.0);	"
                "float specular = pow(NdotH, cShininess);"
                "SpecularLight  += specular * _lightColor;"
            "}	"

            "gl_Position    =   _MVP * vec4(_position,1.0);"

            "_outUV         =   _uv;"
        "}"
    };
    const char* ps  =
    {
       "precision  lowp float; "
       "uniform sampler2D  sTexture;"

       "varying vec2  _outUV;"
       "varying vec3  DiffuseLight;"
       "varying vec3  SpecularLight;"

       "void main()"
       "{"
           "vec3 texColor  = (texture2D(sTexture, _outUV)).rgb;"
           "vec3 color = (texColor * DiffuseLight) + SpecularLight;"
           "gl_FragColor = vec4(color, 1.0);"
       "}"
    };
    bool res = createProgram(vs,ps);
    if(res)
    {
        _MVP        =   glGetUniformLocation(_programId,"_MVP");
        _matMVT     =   glGetUniformLocation(_programId,"_matMVT");
        _matMV      =   glGetUniformLocation(_programId,"_matMV");
        _lightDir   =   glGetUniformLocation(_programId,"_lightDir");
        _lightPos   =   glGetUniformLocation(_programId,"_lightPos");
        _lightColor =   glGetUniformLocation(_programId,"_lightColor");
        _texture    =   glGetUniformLocation(_programId,"_texture");
        _position   =   glGetAttribLocation(_programId, "_position");
        _normal     =   glGetAttribLocation(_programId, "_normal");
        _uv         =   glGetAttribLocation(_programId, "_uv");
    }
    return res;
}

void QMyShaderDotlight::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_normal);
    glEnableVertexAttribArray(_uv);
}

void QMyShaderDotlight::end()
{
    glDisableVertexAttribArray(_position);
    glDisableVertexAttribArray(_normal);
    glDisableVertexAttribArray(_uv);
    glUseProgram(0);
}
