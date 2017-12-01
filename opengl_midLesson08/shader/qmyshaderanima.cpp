#include "qmyshaderanima.h"

QMyShaderAnima::QMyShaderAnima()
{
    inVertex        =   -1;
    inNormal        =   -1;

    MVPMatrix       =   -1;
    myAnim          =   -1;
    LightDirection  =   -1;
    EyePosition     =   -1;
    sTexture        =   -1;
}

bool QMyShaderAnima::initialize()
{
    //!根据变量（myAnim）的变化，调整物体坐标的值，来控制物体的位置
#if 1
    const char* vs  =
    {
        "attribute vec3  inVertex;"
        "attribute vec3  inNormal;"

        "uniform mat4  MVPMatrix;"		// model view projection transformation
        "uniform vec3  LightDirection;"	// light direction in model space
        "uniform vec3  EyePosition;"	    // eye position in model space
        "uniform float  myAnim;"
        "varying vec2  TexCoord;"

        "void main()"
        "{"
            "float ripple = 2.0*cos(0.2*inVertex.y + (radians(8.0*myAnim*360.0)));"
            "gl_Position = MVPMatrix * vec4(inVertex.x,inVertex.y,inVertex.z + ripple,1.0);"

            //"gl_Position = MVPMatrix * vec4(inVertex,1.0);"

            "vec3 eyeDirection = normalize(EyePosition - inVertex);"

            "TexCoord.x = dot(LightDirection, inNormal);"
            "TexCoord.y = dot(eyeDirection, inNormal);"
        "}"

    };
#else
    const char* vs  =
    {
        "attribute vec3  inVertex;"
        "attribute vec3  inNormal;"

        "uniform mat4  MVPMatrix;"		// model view projection transformation
        "uniform vec3  LightDirection;"	// light direction in model space
        "uniform vec3  EyePosition;"	    // eye position in model space
        "uniform float  myAnim;"
        "varying vec2  TexCoord;"

        "void main()"
        "{"
            "gl_Position = MVPMatrix * vec4(inVertex,1.0);"

            "vec3 eyeDirection = normalize(EyePosition - inVertex);"

            "TexCoord.x = dot(LightDirection, inNormal);"
            "TexCoord.y = dot(eyeDirection, inNormal);"
        "}"

    };
#endif
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
        myAnim          = glGetUniformLocation(_programId,"myAnim");
    }
    return  true;
}

void QMyShaderAnima::begin()
{
    glUseProgram(_programId);
    glEnableVertexAttribArray(inVertex);
    glEnableVertexAttribArray(inNormal);
}

void QMyShaderAnima::end()
{
    glDisableVertexAttribArray(inVertex);
    glDisableVertexAttribArray(inNormal);
    glUseProgram(0);
}
