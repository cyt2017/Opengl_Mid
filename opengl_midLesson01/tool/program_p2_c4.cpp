#include "program_p2_c4.h"

PROGRAM_P2_C4::PROGRAM_P2_C4()
{
    _positionAttr   =   -1;
    _colorAttr      =   -1;
    _uvAttr         =   -1;
    _MVP            =   -1;
    _texture        =   -1;
}

bool PROGRAM_P2_C4::initialize()
{
    //!使用（varying），是表示需要在（vs）和（ps）之间进行传递的变量，把需要传递的变量在（vs）中赋值，并在（ps）中重复使用（varying）语句即可
    const char* vs  =
    {
        "uniform    mat4    _MVP;"
        "attribute  vec3    _positionAttr;"
        "attribute  vec4    _colorAttr;"
        "attribute  vec2    _uvAttr;"

        "varying    vec4    _outColor;"
        "varying    vec2    _outUV;"

        "void main()"
        "{"
        "   _outUV      =   _uvAttr;"
        "   _outColor   =   _colorAttr;"
        "   gl_Position =   _MVP * vec4(_positionAttr,1.0);"
        "}"
    };
    const char* ps  =
    {
        "precision  lowp    float; "
        "uniform    sampler2D   _texture;"

        "varying    vec4    _outColor;"
        "varying    vec2    _outUV;"

        "void main()"
        "{"
        "   vec4 color  =   texture2D(_texture,_outUV);"
        "   gl_FragColor=   _outColor * color;"
        "}"
    };
    //!上面的if语句，表示如果颜色中的alpha值<0.2,那么就不绘制（discard）出来。
    //!在上面的（"gl_FragColor=tcolor0 + tcolor1;"）中，用（+）图像变亮；用（*）图像变暗。
    bool    res =   createProgram(vs,ps);
    if(res)
    {
        //!~需要注意变量的类型
        _positionAttr   =   glGetAttribLocation(_programId,"_positionAttr");
        _colorAttr      =   glGetAttribLocation(_programId,"_colorAttr");
        _uvAttr         =   glGetUniformLocation(_programId,"_uvAttr");
        _MVP            =   glGetUniformLocation(_programId,"_MVP");
        _texture        =   glGetUniformLocation(_programId,"_texture");
    }
    return  res;
}

void PROGRAM_P2_C4::begin()
{
    glUseProgram(_programId);
    //!在显卡里面使用的局部变量，在使用时是需要进行使能和关闭的
    glEnableVertexAttribArray(_positionAttr);
    glEnableVertexAttribArray(_colorAttr);
    glEnableVertexAttribArray(_uvAttr);
}

void PROGRAM_P2_C4::end()
{
    glDisableVertexAttribArray(_positionAttr);
    glDisableVertexAttribArray(_colorAttr);
    glDisableVertexAttribArray(_uvAttr);
    glUseProgram(0);
}
