#include "qmyshadernormalimage.h"

QMyShaderNormalImage::QMyShaderNormalImage()
{
    _position   =   -1;
    _normal     =   -1;
    _uv         =   -1;
    _tangent    =   -1;

    _MVP        =   -1;
    _lightPos   =   -1;
    _texNor     =   -1;
    _texNor1    =   -1;
}

bool QMyShaderNormalImage::initialize()
{
    const char* vs  =
    {
        "attribute  vec3  inVertex;"
        "attribute  vec3  inNormal;"
        "attribute vec2  inTexCoord;"
        "attribute vec3  inTangent;"

        "uniform mat4  MVPMatrix;"
        // Light position (point light) in model space
        "uniform vec3  LightPosModel;"

        "varying  vec3  LightVec;"
        "varying  vec2  TexCoord;"

        "void main()"
        "{"
            "gl_Position = MVPMatrix * vec4(inVertex,1);"

            //! 计算灯光的方向，在模型空间中
            "highp vec3 lightDirection = normalize(LightPosModel - vec3(inVertex));"

            //! 将灯的方向从模型空间，转化到tangent空间
            "highp vec3 bitangent = cross(inNormal, inTangent);"
            "highp mat3 tangentSpaceXform = mat3(inTangent, bitangent, inNormal);"
            "LightVec = lightDirection * tangentSpaceXform;"

            "TexCoord = inTexCoord;"
        "}"

    };
    const char* ps  =
    {
        "precision  lowp float; "
        "uniform sampler2D  sBaseTex;"
        "uniform sampler2D  sNormalMap;"

        "varying lowp    vec3  LightVec;"
        "varying mediump vec2  TexCoord;"

        "void main()"
        "{"
            // 将像素值映射到 [-1, 1]
            "vec3 normal = texture2D(sNormalMap, TexCoord).rgb * 2.0 - 1.0;"

            "float lightIntensity = dot(LightVec, normal);"

            "vec3 texColor = texture2D(sBaseTex, TexCoord).rgb;	"
            "gl_FragColor = vec4(texColor * lightIntensity, 1.0);"
        "}"
    };

    bool res = createProgram(vs,ps);
    if(res)
    {
        _position   = glGetAttribLocation(_programId,"inVertex");
        _normal     = glGetAttribLocation(_programId,"inNormal");
        _uv         = glGetAttribLocation(_programId,"inTexCoord");
        _tangent    = glGetAttribLocation(_programId,"inTangent");

        _MVP        = glGetUniformLocation(_programId,"MVPMatrix");
        _lightPos   = glGetUniformLocation(_programId,"LightPosModel");
        _texNor     = glGetUniformLocation(_programId,"sTexture");
        _texNor1    = glGetUniformLocation(_programId,"sNormalMap");
    }
    return res;
}

void QMyShaderNormalImage::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_tangent);
    glEnableVertexAttribArray(_normal);
    glEnableVertexAttribArray(_uv);
}

void QMyShaderNormalImage::end()
{
    glDisableVertexAttribArray(_position);
    glDisableVertexAttribArray(_tangent);
    glDisableVertexAttribArray(_normal);
    glDisableVertexAttribArray(_uv);
    glUseProgram(0);;
}
