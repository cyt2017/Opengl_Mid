#include "qmyshaderlight.h"

QMyShaderLight::QMyShaderLight()
{
    _MVP        =   -1;
    _matMVT     =   -1;
    _lightDir   =   -1;
    _texture    =   -1;
    _position   =   -1;
    _normal     =   -1;
    _uv         =   -1;
}

bool QMyShaderLight::initialize()
{
    char *vs =
    {
        "uniform    mat4    _MVP;"
        "uniform    mat4    _matMVT;"
        "uniform    vec3    _lightDir;"
        "attribute  vec3    _position;"
        "attribute  vec3    _normal;"
        "attribute  vec2    _uv;"

        "varying    vec2    _outUV;"
        "varying    float   _outNDotV;"
        "void main()"
        "{"
            "gl_Position    =   _MVP * vec4(_position,1.0);"
            "_outUV         =   _uv;"
            "vec3 normalT   =   mat3(_matMVT) * _normal;"
            "_outNDotV      =   max(dot(normalT,_lightDir),0.0);"
        "}"

    };
    char* ps =
    {
        "precision  lowp        float;"
        "uniform    sampler2D   _texture;"

        "varying    float       _outNDotV;"
        "varying    vec2        _outUV;"
        "void main (void)"
        "{"
            "gl_FragColor.rgb    =   texture2D(_texture,_outUV).rgb * _outNDotV;"
            "gl_FragColor.a      =   1.0; "
        "}"
    };
    bool res = createProgram(vs,ps);
    if(res)
    {
        _MVP        =   glGetUniformLocation(_programId,"_MVP");
        _matMVT     =   glGetUniformLocation(_programId,"_matMVT");
        _lightDir   =   glGetUniformLocation(_programId,"_lightDir");
        _texture    =   glGetUniformLocation(_programId,"_texture");
        _position   =   glGetAttribLocation(_programId, "_position");
        _normal     =   glGetAttribLocation(_programId, "_normal");
        _uv         =   glGetAttribLocation(_programId, "_uv");
    }
    return res;
}

void QMyShaderLight::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_normal);
    glEnableVertexAttribArray(_uv);
}

void QMyShaderLight::end()
{
    glDisableVertexAttribArray(_position);
    glDisableVertexAttribArray(_normal);
    glDisableVertexAttribArray(_uv);
    glUseProgram(0);
}
