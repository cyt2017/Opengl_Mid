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
       
        /// ��ʼ������
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
        *   ʹ�ó���
        */
        virtual void    begin()
        {
            _device.useProgram(this);
            _device.enableVertexAttributeArray(_positionAttr);
            _device.enableVertexAttributeArray(_uvAttr);
            _device.enableVertexAttributeArray(_colorAttr);
            
        }
        /**
        *   ʹ�����
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
       
        /// ��ʼ������
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute highp vec3  inVertex;"
                "attribute highp vec3  inNormal;"
                "attribute highp vec2  inTexCoord;"
                "attribute vec3  inTangent;"

                "uniform mat4  MVPMatrix;"
                // Light position (point light) in model space
                "uniform vec3  LightPosModel;"

                "varying lowp vec3  LightVec;"
                "varying mediump vec2  TexCoord;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex,1);"

                    //! ����ƹ�ķ�����ģ�Ϳռ���
                    "highp vec3 lightDirection = normalize(LightPosModel - vec3(inVertex));"

                    //! ���Ƶķ����ģ�Ϳռ䣬ת����tangent�ռ�
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
                    // read the per-pixel normal from the normal map and expand to [-1, 1]
                    "vec3 normal = texture2D(sNormalMap, TexCoord).rgb * 2.0 - 1.0;"

                    // linear interpolations of normals may cause shortened normals and thus
                    // visible artifacts on low-poly models.
                    // We omit the normalization here for performance reasons

                    // calculate diffuse lighting as the cosine of the angle between light
                    // direction and surface normal (both in surface local/tangent space)
                    // We don't have to clamp to 0 here because the framebuffer write will be clamped
                    "float lightIntensity = dot(LightVec, normal);"

                    // read base texture and modulate with light intensity
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
        *   ʹ�ó���
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
        *   ʹ�����
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


    class   Program_CELL :public ProgramId
    {
    public:
        typedef int location; 
    public:
        CELLOpenGL& _device;

        location    inVertex;
        location    inNormal;

        location    MVPMatrix;
        location    LightDirection;
        location    EyePosition;
        location    sTexture;
    public:
        Program_CELL(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_CELL()
        {
        }
       
        /// ��ʼ������
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute highp   vec3  inVertex;"
                "attribute mediump vec3  inNormal;"

                "uniform highp mat4  MVPMatrix;"		// model view projection transformation
                "uniform highp vec3  LightDirection;"	// light direction in model space
                "uniform highp vec3  EyePosition;"	    // eye position in model space

                "varying mediump vec2  TexCoord;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex,1.0);"

                    "mediump vec3 eyeDirection = normalize(EyePosition - inVertex);"

                    "TexCoord.x = dot(LightDirection, inNormal);"
                    "TexCoord.y = dot(eyeDirection, inNormal);"
                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform sampler2D  sTexture;"

                "varying mediump vec2  TexCoord;"

                "const lowp vec3  cBaseColor = vec3(0.2, 1.0, 0.7);"

                "void main()"
                "{"
                    "gl_FragColor = vec4(cBaseColor * texture2D(sTexture, TexCoord).rgb, 1.0);"
                "}"
            };
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");

            MVPMatrix       =   _device.getUniformLocation(prgId,"MVPMatrix");
            LightDirection  =   _device.getUniformLocation(prgId,"LightDirection");
            EyePosition     =   _device.getUniformLocation(prgId,"EyePosition");
            sTexture        =   _device.getUniformLocation(prgId,"sTexture");
            
            return  true;
        }

        /**
        *   ʹ�ó���
        */
        virtual void    begin()
        {

            _device.useProgram(this);
            _device.enableVertexAttributeArray(inVertex);
            _device.enableVertexAttributeArray(inNormal);
            
        }
        /**
        *   ʹ�����
        */
        virtual void    end()
        {
            _device.disableVertexAttributeArray(inVertex);
            _device.disableVertexAttributeArray(inNormal);
            _device.useProgram(0);
        }
    };

    class   Program_SINE :public ProgramId
    {
    public:
        typedef int location; 
    public:
        CELLOpenGL& _device;

        location    inVertex;
        location    inNormal;
        
        location    MVPMatrix;
        location    myAnim;
        location    LightDirection;
        location    EyePosition;
        location    sTexture;
    public:
        Program_SINE(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_SINE()
        {
        }
       
        /// ��ʼ������
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute highp   vec3  inVertex;"
                "attribute mediump vec3  inNormal;"

                "uniform highp mat4  MVPMatrix;"		// model view projection transformation
                "uniform highp vec3  LightDirection;"	// light direction in model space
                "uniform highp vec3  EyePosition;"	    // eye position in model space
                "uniform mediump float  myAnim;"
                "varying mediump vec2  TexCoord;"

                "void main()"
                "{"
                    "float ripple = 3.0*cos(0.2*inVertex.y + (radians(5.0*myAnim*360.0)));"
                    "gl_Position = MVPMatrix * vec4(inVertex.x,inVertex.y,inVertex.z + ripple,1.0);"

                    "mediump vec3 eyeDirection = normalize(EyePosition - inVertex);"

                    "TexCoord.x = dot(LightDirection, inNormal);"
                    "TexCoord.y = dot(eyeDirection, inNormal);"
                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform sampler2D  sTexture;"

                "varying mediump vec2  TexCoord;"

                "const lowp vec3  cBaseColor = vec3(0.2, 1.0, 0.7);"

                "void main()"
                "{"
                    "gl_FragColor = vec4(cBaseColor * texture2D(sTexture, TexCoord).rgb, 1.0);"
                "}"
            };
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");

            MVPMatrix       =   _device.getUniformLocation(prgId,"MVPMatrix");
            LightDirection  =   _device.getUniformLocation(prgId,"LightDirection");
            EyePosition     =   _device.getUniformLocation(prgId,"EyePosition");
            sTexture        =   _device.getUniformLocation(prgId,"sTexture");
            myAnim          =   _device.getUniformLocation(prgId,"myAnim");
            
            
            return  true;
        }

        /**
        *   ʹ�ó���
        */
        virtual void    begin()
        {

            _device.useProgram(this);
            _device.enableVertexAttributeArray(inVertex);
            _device.enableVertexAttributeArray(inNormal);
            
        }
        /**
        *   ʹ�����
        */
        virtual void    end()
        {
            _device.disableVertexAttributeArray(inVertex);
            _device.disableVertexAttributeArray(inNormal);
            _device.useProgram(0);
        }
    };



    class   Program_Perturbed :public ProgramId
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
        location    EyePosModel;
        location    sBaseTex;
        location    sNormalMap;
    public:
        Program_Perturbed(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_Perturbed()
        {
        }
       
        /// ��ʼ������
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute  highp   vec3  inVertex;"
                "attribute  mediump vec3  inNormal;"
                "attribute  mediump vec2  inTexCoord;"
                "attribute  mediump vec3  inTangent;"

                "uniform highp   mat4  MVPMatrix;"
                // Light position (point light) in model space
                "uniform mediump vec3  EyePosModel;"

                "varying mediump vec3  EyeDirection;"
                "varying mediump vec2  TexCoord;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex,1);"

                    // Calculate direction from eye position in model space
                    "mediump vec3 eyeDirModel = normalize(EyePosModel - inVertex);"

                    // transform light direction from model space to tangent space
                    "mediump vec3 binormal = cross(inNormal, inTangent);"
                    "mediump mat3 tangentSpaceXform = mat3(inTangent, binormal, inNormal);"
                    "EyeDirection = eyeDirModel * tangentSpaceXform;	"
                    "TexCoord = inTexCoord;"

                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform sampler2D  sReflectTex;"
                "uniform sampler2D  sNormalMap;"

                "varying mediump vec3  EyeDirection;"

                "varying mediump vec2  TexCoord;"

                "void main()"
                "{"
                    // Get the normal direction per pixel from the normal map
                    // The tNormal vector is defined in surface local coordinates (tangent space).
                    "mediump vec3 normal = texture2D(sNormalMap, TexCoord).rgb * 2.0 - 1.0;"

                    // reflect(): For the incident vector I and surface orientation N, returns the reflection direction:
                    // I - 2 * dot(N, I) * N, N must already be normalized in order to achieve the desired result.
                    "mediump vec3 reflectDir = reflect(normal, EyeDirection);"
                    "mediump vec2 reflectCoord = (reflectDir.xy) * 0.5 + 0.5;"

                    // Look-up in the 2D texture using the normal map disturbance
                    "gl_FragColor = texture2D(sReflectTex, reflectCoord);"
                "}"
            };
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");
            inTexCoord  =   _device.getAttribLocation(prgId, "inTexCoord");
            inTangent   =   _device.getAttribLocation(prgId,"inTangent");



            MVPMatrix       =   _device.getUniformLocation(prgId,"MVPMatrix");
            EyePosModel     =   _device.getUniformLocation(prgId,"EyePosModel");
            sBaseTex        =   _device.getUniformLocation(prgId,"sReflectTex");
            sNormalMap      =   _device.getUniformLocation(prgId,"sNormalMap");
            
            return  true;
        }

        /**
        *   ʹ�ó���
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
        *   ʹ�����
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


    

    class   Program_Reflections :public ProgramId
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
        location    EyePosModel;
        location    ModelWorld;
        location    sCubeMap;
    public:
        Program_Reflections(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_Reflections()
        {
        }
       
        /// ��ʼ������
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute highp   vec3  inVertex;"
                "attribute mediump vec3  inNormal;"

                "uniform highp   mat4  MVPMatrix;"
                "uniform mediump mat4  ModelWorld;"
                "uniform mediump vec3  EyePosModel;"

                "varying mediump vec3  ReflectDir;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex, 1.0);"

                    "mediump vec3 eyeDir = normalize(inVertex - EyePosModel);"

                    "ReflectDir = mat3(ModelWorld) * reflect(eyeDir, inNormal);"
                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform samplerCube    sCubeMap;"
                "varying mediump vec3   ReflectDir;"

                "void main()"
                "{"
                    "gl_FragColor = textureCube(sCubeMap, ReflectDir);"
                "}"

            };
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");

            MVPMatrix       =   _device.getUniformLocation(prgId,"MVPMatrix");
            EyePosModel     =   _device.getUniformLocation(prgId,"EyePosModel");
            ModelWorld      =   _device.getUniformLocation(prgId,"ModelWorld");
            sCubeMap        =   _device.getUniformLocation(prgId,"sCubeMap");
            
            return  true;
        }

        /**
        *   ʹ�ó���
        */
        virtual void    begin()
        {
            _device.useProgram(this);
            _device.enableVertexAttributeArray(inVertex);
            _device.enableVertexAttributeArray(inNormal);
        }
        /**
        *   ʹ�����
        */
        virtual void    end()
        {
            _device.disableVertexAttributeArray(inVertex);
            _device.disableVertexAttributeArray(inNormal);
            _device.useProgram(0);
        }
    };


}