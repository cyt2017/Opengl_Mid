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
                "attribute vec3 _position;"
                "attribute vec4 _color;"
                "attribute vec2 _uv;"

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
                "attribute vec3  inVertex;"
                "attribute vec3  inNormal;"
                "attribute vec2  inTexCoord;"
                "attribute vec3  inTangent;"

                "uniform mat4  MVPMatrix;"
                // Light position (point light) in model space
                "uniform vec3  LightPosModel;"

                "varying vec3  LightVec;"
                "varying vec2  TexCoord;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex,1);"

                    //! 计算灯光的方向，在模型空间中
                    "vec3 lightDirection = normalize(LightPosModel - vec3(inVertex));"

                    //! 将灯的方向从模型空间，转化到tangent空间
                    "vec3 bitangent = cross(inNormal, inTangent);"
                    "mat3 tangentSpaceXform = mat3(inTangent, bitangent, inNormal);"
                    "LightVec = lightDirection * tangentSpaceXform;"

                    "TexCoord = inTexCoord;"
                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform sampler2D  sBaseTex;"
                "uniform sampler2D  sNormalMap;"

                "varying    vec3  LightVec;"
                "varying vec2  TexCoord;"

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
       
        /// 初始化函数
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute   vec3  inVertex;"
                "attribute vec3  inNormal;"

                "uniform mat4  MVPMatrix;"		// model view projection transformation
                "uniform vec3  LightDirection;"	// light direction in model space
                "uniform vec3  EyePosition;"	    // eye position in model space

                "varying vec2  TexCoord;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex,1.0);"

                    "vec3 eyeDirection = normalize(EyePosition - inVertex);"

                    "TexCoord.x = dot(LightDirection, inNormal);"
                    "TexCoord.y = dot(eyeDirection, inNormal);"
                "}"

            };
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
        *   使用程序
        */
        virtual void    begin()
        {

            _device.useProgram(this);
            _device.enableVertexAttributeArray(inVertex);
            _device.enableVertexAttributeArray(inNormal);
            
        }
        /**
        *   使用完成
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
       
        /// 初始化函数
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute   vec3  inVertex;"
                "attribute vec3  inNormal;"

                "uniform mat4  MVPMatrix;"		// model view projection transformation
                "uniform vec3  LightDirection;"	// light direction in model space
                "uniform vec3  EyePosition;"	    // eye position in model space
                "uniform float  myAnim;"
                "varying vec2  TexCoord;"

                "void main()"
                "{"
                    "float ripple = 3.0*cos(0.2*inVertex.y + (radians(5.0*myAnim*360.0)));"
                    "gl_Position = MVPMatrix * vec4(inVertex.x,inVertex.y,inVertex.z + ripple,1.0);"

                    "vec3 eyeDirection = normalize(EyePosition - inVertex);"

                    "TexCoord.x = dot(LightDirection, inNormal);"
                    "TexCoord.y = dot(eyeDirection, inNormal);"
                "}"

            };
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
        *   使用程序
        */
        virtual void    begin()
        {

            _device.useProgram(this);
            _device.enableVertexAttributeArray(inVertex);
            _device.enableVertexAttributeArray(inNormal);
            
        }
        /**
        *   使用完成
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
       
        /// 初始化函数
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute    vec3  inVertex;"
                "attribute  vec3  inNormal;"
                "attribute  vec2  inTexCoord;"
                "attribute  vec3  inTangent;"

                "uniform   mat4  MVPMatrix;"
                // Light position (point light) in model space
                "uniform vec3  EyePosModel;"

                "varying vec3  EyeDirection;"
                "varying vec2  TexCoord;"

                "void main()"
                "{"
                    "gl_Position = MVPMatrix * vec4(inVertex,1);"

                    // Calculate direction from eye position in model space
                    "vec3 eyeDirModel = normalize(EyePosModel - inVertex);"

                    // transform light direction from model space to tangent space
                    "vec3 binormal = cross(inNormal, inTangent);"
                    "mat3 tangentSpaceXform = mat3(inTangent, binormal, inNormal);"
                    "EyeDirection = eyeDirModel * tangentSpaceXform;	"
                    "TexCoord = inTexCoord;"

                "}"

            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform sampler2D  sReflectTex;"
                "uniform sampler2D  sNormalMap;"

                "varying vec3  EyeDirection;"

                "varying vec2  TexCoord;"

                "void main()"
                "{"
                    // Get the normal direction per pixel from the normal map
                    // The tNormal vector is defined in surface local coordinates (tangent space).
                    "vec3 normal = texture2D(sNormalMap, TexCoord).rgb * 2.0 - 1.0;"

                    // reflect(): For the incident vector I and surface orientation N, returns the reflection direction:
                    // I - 2 * dot(N, I) * N, N must already be normalized in order to achieve the desired result.
                    "vec3 reflectDir = reflect(normal, EyeDirection);"
                    "vec2 reflectCoord = (reflectDir.xy) * 0.5 + 0.5;"

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


    

    class   Program_Fog :public ProgramId
    {
    public:
        typedef int attribute; 
        typedef int uniform; 
    public:
        CELLOpenGL& _device;

        attribute   inVertex;
        attribute   inNormal;
        attribute   inTexCoord;

        uniform     MVPMatrix;
        uniform     ModelViewMatrix;
        uniform     LightDirection;
        uniform     iFogMode;
        uniform     EyePositon;
        uniform     FogDensity;
        uniform     FogEnd;
        uniform     FogRcpEndStartDiff;
        uniform     sTexture;
        uniform     FogColor;
    public:
        Program_Fog(CELLOpenGL& device)
            :_device(device)
        {
        }
        ~Program_Fog()
        {
        }
       
        /// 初始化函数
        virtual bool    initialize()
        {
            const char* vs  =   
            {
                "attribute vec3  inVertex;"
                "attribute vec3  inNormal;"
                "attribute vec2  inTexCoord;"

                "uniform mat4  MVPMatrix;"
                "uniform mat4  ModelViewMatrix;"
                "uniform vec3  LightDirection;"
                // fog uniforms
                "uniform float  FogDensity;"
                "uniform float  FogEnd;"

                "varying vec2  TexCoord;"
                "varying vec3  DiffuseLight;"
                "varying vec3  FogIntensity;"

                "void main()"
                "{"
                    //! 这里需要将顶点转化到摄像机空间，目的是用来计算摄像机与顶点之间的距离
                    "vec3   viewPos = vec3(ModelViewMatrix * vec4(inVertex,1));"
                    "float  eyeDist = length(viewPos);"


                    // transform vertex position
                    "gl_Position = MVPMatrix * vec4(inVertex,1);"

                    "TexCoord = inTexCoord;"

                    "float DiffuseIntensity = dot(inNormal, normalize(LightDirection));"

                    // clamp negative values and add some ambient light
                    "DiffuseLight = vec3(max(DiffuseIntensity, 0.0) * 0.5 + 0.5);"

                    "float fogIntensity = 1.0;"
                    
                    "fogIntensity   =   (FogEnd - eyeDist) * FogDensity;"
                    "fogIntensity   =   clamp(fogIntensity, 0.0, 1.0);"
                    "FogIntensity   =   vec3(fogIntensity,fogIntensity,fogIntensity);"
                "}"


            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform    sampler2D  sTexture;"

                "uniform    vec3  FogColor;"
                "varying    vec2  TexCoord;"
                "varying    vec3  DiffuseLight;"
                "varying    vec3  FogIntensity;"

                "void main()"
                "{"
                    "vec3 texColor  = texture2D(sTexture, TexCoord).rgb;"
                    "vec3 color = texColor * DiffuseLight;"

                    "color = mix(FogColor, color, FogIntensity);"
                    "gl_FragColor = vec4(color, 1.0);"
                "}"

            };
            
            ProgramId&  prgId   =   *this;
            prgId   =   _device.createProgram(vs,ps);
            
            inVertex    =   _device.getAttribLocation(prgId, "inVertex");
            inNormal    =   _device.getAttribLocation(prgId, "inNormal");
            inTexCoord  =   _device.getAttribLocation(prgId, "inTexCoord");


            
            EyePositon          =   _device.getUniformLocation(prgId,"EyePositon");
            MVPMatrix           =   _device.getUniformLocation(prgId,"MVPMatrix");
            ModelViewMatrix     =   _device.getUniformLocation(prgId,"ModelViewMatrix");
            LightDirection      =   _device.getUniformLocation(prgId,"LightDirection");
            iFogMode            =   _device.getUniformLocation(prgId,"iFogMode");
            FogDensity          =   _device.getUniformLocation(prgId,"FogDensity");
            FogEnd              =   _device.getUniformLocation(prgId,"FogEnd");
            FogRcpEndStartDiff  =   _device.getUniformLocation(prgId,"FogRcpEndStartDiff");
            sTexture            =   _device.getUniformLocation(prgId,"sTexture");
            FogColor            =   _device.getUniformLocation(prgId,"FogColor");
            
            
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