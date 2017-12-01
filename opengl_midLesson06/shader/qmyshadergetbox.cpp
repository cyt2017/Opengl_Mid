#include "qmyshadergetbox.h"

QMyShaderGetBox::QMyShaderGetBox()
{
    _position       =   -1;
    _normal         =   -1;

    _MVP            =   -1;
    _matMVT         =   -1;
    _eyePos         =   -1;
    _lightVecter    =   -1;
}

bool QMyShaderGetBox::initialize()
{
    const char* vs  =
    {
        "attribute   vec3 _position;"
        "attribute   vec3 _normal;"
        "varying     vec4 _outColor;"

        "uniform mat4 modelViewProjection;"
        "uniform mat4 modelViewInverse;"
        "uniform vec3 eyePosition;"
        "uniform vec3 lightVector;"

        "void main(){"

        "gl_Position =  modelViewProjection * vec4(_position,1);"


        "vec4 normal = modelViewInverse * vec4(_normal,1);"
        "normal = normalize(normal);"

        "vec3 light = normalize( lightVector );"

        "vec3 eye = eyePosition;"
        "vec3 v = normalize( _position);"
        "vec3 halfs = normalize( light  + eye);"

        // 使用兰伯特余弦定律（Lambert' cosine law）计算漫反射

        "float diffuse = dot( normal.xyz, light );"

        // 使用比林 - 冯着色模型（Blinn - Phong shading model）来计算镜面反射
        "float specular = dot( normal.xyz, halfs );"
        "specular = pow( specular, 32.0 );"


        "vec4 ambientColor = vec4( 0.4, 0.4, 0.4, 0.4 );"
        "vec4 diffuseColor = vec4( 1, 1, 1, 1.0 );"

        "vec4 specularMaterial = vec4( 1.0, 1.0, 1.0, 1.0 );"

        "_outColor = diffuse * diffuseColor + specular * specularMaterial + ambientColor;"
        "}"
    };
    const char* ps  =
    {
        "precision  lowp float; "
        "varying     vec4 _outColor;"
        "void main(){"
            "gl_FragColor    =   _outColor;"
        "}"
    };

    bool res = createProgram(vs,ps);
    if(res)
    {
        _position       = glGetAttribLocation(_programId,"_position");
        _normal         = glGetAttribLocation(_programId,"_normal");

        _MVP            = glGetUniformLocation(_programId,"modelViewProjection");
        _matMVT         = glGetUniformLocation(_programId,"modelViewInverse");
        _eyePos         = glGetUniformLocation(_programId,"eyePosition");
        _lightVecter    = glGetUniformLocation(_programId,"lightVector");
    }
    return res;
}

void QMyShaderGetBox::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(_position);
    glEnableVertexAttribArray(_normal);
}

void QMyShaderGetBox::end()
{
    glDisableVertexAttribArray(_position);
    glDisableVertexAttribArray(_normal);
    glUseProgram(0);
}
