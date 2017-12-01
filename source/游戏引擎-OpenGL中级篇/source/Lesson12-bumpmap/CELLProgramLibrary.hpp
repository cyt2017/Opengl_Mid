#pragma once


namespace   CELL
{

    
    class   PROGRAM_P2_T2_C3 :public ProgramId
    {
    public:
        typedef int location; 
    public:
        CELLOpenGL& _device;

        location    _positionAttr;
        location    _colorAttr;
        location    _uvAttr;
        location    _MVP;
        location    _texture;
    public:
        PROGRAM_P2_T2_C3(CELLOpenGL& device)
            :_device(device)
        {
            _positionAttr   =   -1;
            _colorAttr      =   -1;
            _uvAttr         =   -1;
            _MVP            =   -1;
        }
        ~PROGRAM_P2_T2_C3()
        {
        }
       
        /// 初始化函数
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "uniform   mat4 _MVP;"
                "attribute highp vec3 _position;"
                "attribute highp vec4 _color;"
                "attribute highp vec2 _uv;"

                "varying   vec4 _outColor;"
                "varying   vec2 _outUV;"
                "void main()"
                "{"
                "   vec4    pos =   vec4(_position.x,_position.y,_position.z,1);"
                "   gl_Position =   _MVP * pos;"
                "   _outColor   =   _color;"
                "   _outUV      =   _uv;"
                "}"
            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform    sampler2D   _texture;\n"
                "varying    vec4        _outColor;\n"
                "varying    vec2        _outUV;"
                "void main()"
                "{"
                "   vec4   color   =   texture2D(_texture,_outUV);"
                "   gl_FragColor   =   color * _outColor;"
                "}"
            };
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            _positionAttr   =   _device.getAttribLocation(prgId, "_position");
            _colorAttr      =   _device.getAttribLocation(prgId, "_color");
            _uvAttr         =   _device.getAttribLocation(prgId, "_uv");
            _MVP            =   _device.getUniformLocation(prgId,"_MVP");
            _texture        =   _device.getUniformLocation(prgId,"_texture");
            
            return  true;
        }

        /**
        *   使用程序
        */
        virtual void    begin()
        {
            _device.useProgram(this);
            _device.enableVertexAttributeArray(_positionAttr);
            _device.enableVertexAttributeArray(_uvAttr);
            _device.enableVertexAttributeArray(_colorAttr);
            
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            
            _device.disableVertexAttributeArray(_positionAttr);
            _device.disableVertexAttributeArray(_uvAttr);
            _device.disableVertexAttributeArray(_colorAttr);
            _device.useProgram(0);
        }
    };



    class   Program_BumpMap :public ProgramId
    {
    public:
        typedef int location; 
    public:
        CELLOpenGL& _device;

        location    inVertex;
        location    inNormal;
        location    inTexCoord;
        location    inTangent;

        location    MVPMatrix;
        location    LightPosModel;
        location    sBaseTex;
        location    sNormalMap;
    public:
        Program_BumpMap(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_BumpMap()
        {
        }
       
        /// 初始化函数
        virtual bool    initialize()
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
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");
            inTexCoord  =   _device.getAttribLocation(prgId, "inTexCoord");
            inTangent   =   _device.getAttribLocation(prgId,"inTangent");



            MVPMatrix       =   _device.getUniformLocation(prgId,"MVPMatrix");
            LightPosModel   =   _device.getUniformLocation(prgId,"LightPosModel");
            sBaseTex        =   _device.getUniformLocation(prgId,"sBaseTex");
            sNormalMap      =   _device.getUniformLocation(prgId,"sNormalMap");
            
            return  true;
        }

        /**
        *   使用程序
        */
        virtual void    begin()
        {

            _device.useProgram(this);
            _device.enableVertexAttributeArray(inVertex);
            _device.enableVertexAttributeArray(inNormal);
            _device.enableVertexAttributeArray(inTexCoord);
            _device.enableVertexAttributeArray(inTangent);
            
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            _device.disableVertexAttributeArray(inVertex);
            _device.disableVertexAttributeArray(inNormal);
            _device.disableVertexAttributeArray(inTexCoord);
            _device.disableVertexAttributeArray(inTangent);
            _device.useProgram(0);
        }
    };
}