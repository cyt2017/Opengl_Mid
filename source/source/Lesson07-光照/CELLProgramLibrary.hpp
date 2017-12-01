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




    class   Program_BasicLight :public ProgramId
    {
    public:
        typedef int atrribute; 
        typedef int uniform;
    public:
        CELLOpenGL& _device;

        atrribute   inVertex;
        atrribute   inNormal;
        atrribute   inTexCoord;

        uniform     MVPMatrix;
        uniform     LightDirection;
        uniform     ModelViewIT;
        uniform     sTexture;

    public:
        Program_BasicLight(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_BasicLight()
        {
        }
       
        /// 初始化函数
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute vec3	inVertex;"
                "attribute vec3	inNormal;"
                "attribute vec2	inTexCoord;"

                "uniform mat4	MVPMatrix;"
                "uniform mat4	ModelViewIT;"
                "uniform vec3	LightDirection;"
                "varying float	varDot;"
                "varying vec2	varCoord;"
                "void main(void)"
                "{"
                    "gl_Position    =   MVPMatrix * vec4(inVertex,1.0);"
                    "varCoord       =   inTexCoord;"
                    "vec3 transNormal = mat3(ModelViewIT) * inNormal;"
                    "varDot = max( dot(transNormal, LightDirection), 0.0 );"
                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform sampler2D sTexture;"
                "varying float	varDot;"
                "varying vec2	varCoord;"
                "void main (void)"
                "{"
                    "gl_FragColor.rgb    =   texture2D(sTexture,varCoord).rgb * varDot;"
                    "gl_FragColor.a      =   1.0; "
                "}"
            };
            
            ProgramId&  prgId   =   *this;
            prgId       =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");
            inTexCoord  =   _device.getAttribLocation(prgId, "inTexCoord");


            MVPMatrix       =   _device.getUniformLocation(prgId,"MVPMatrix");
            LightDirection  =   _device.getUniformLocation(prgId,"LightDirection");
            ModelViewIT     =   _device.getUniformLocation(prgId,"ModelViewIT");
            sTexture        =   _device.getUniformLocation(prgId,"sTexture");
            
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
            
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            _device.disableVertexAttributeArray(inVertex);
            _device.disableVertexAttributeArray(inNormal);
            _device.disableVertexAttributeArray(inTexCoord);
            _device.useProgram(0);
        }
    };



}