#pragma once

#include "CELLFrame.hpp"
#include "rapidxml.hpp"
#include <map>
#include <string>
using namespace std;

namespace   CELL
{


    class   PROGRAM_P2_T2_C3 :public ProgramId
    {
    public:
        typedef int location; 
    public:
        location    _positionAttr;
        location    _colorAttr;
        location    _uvAttr;
        location    _MVP;
        location    _texture;
    public:
        PROGRAM_P2_T2_C3()
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
        virtual bool    initialize(CELLOpenGL& device)
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
            prgId   =   device.createProgram(vs,ps);
            
            _positionAttr   =   glGetAttribLocation(_programId, "_position");
            _colorAttr      =   glGetAttribLocation(_programId, "_color");
            _uvAttr         =   glGetAttribLocation(_programId, "_uv");
            _MVP            =   glGetUniformLocation(_programId,"_MVP");
            _texture        =   glGetUniformLocation(_programId,"_texture");
            
            return  true;
        }

        /**
        *   使用程序
        */
        virtual void    begin()
        {
            glUseProgram(_programId);
            glEnableVertexAttribArray(_positionAttr);
            glEnableVertexAttribArray(_uvAttr);
            glEnableVertexAttribArray(_colorAttr);
            
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            glDisableVertexAttribArray(_positionAttr);
            glDisableVertexAttribArray(_uvAttr);
            glDisableVertexAttribArray(_colorAttr);
            glUseProgram(0);
        }
    };


    class   PROGRAM_DIR_LIGHT0 :public ProgramId
    {
    public:
        typedef int uniform; 
        typedef int attribute;
    public:
        attribute    _positionAttr;
        attribute    _normal;

        uniform modelViewProjection;
        uniform modelViewInverse;
        uniform eyePosition;
        uniform lightVector;
    public:
        PROGRAM_DIR_LIGHT0()
        {
            _positionAttr   =   -1;
            _normal         =   -1;
        }
        ~PROGRAM_DIR_LIGHT0()
        {
        }
       
        /// 初始化函数
        virtual bool    initialize(CELLOpenGL& device)
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
            
            ProgramId&  prgId   =   *this;
            prgId   =   device.createProgram(vs,ps);

            _positionAttr       =   glGetAttribLocation(_programId, "_position");
            _normal             =   glGetAttribLocation(_programId, "_normal");
            
            modelViewProjection =   glGetUniformLocation(_programId,"modelViewProjection");
            modelViewInverse    =   glGetUniformLocation(_programId,"modelViewInverse");
            eyePosition         =   glGetUniformLocation(_programId,"eyePosition");
            lightVector         =   glGetUniformLocation(_programId,"lightVector");
            
            return  true;
        }

        /**
        *   使用程序
        */
        virtual void    begin()
        {
            glUseProgram(_programId);
            glEnableVertexAttribArray(_positionAttr);
            glEnableVertexAttribArray(_normal);
            
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            glDisableVertexAttribArray(_positionAttr);
            glDisableVertexAttribArray(_normal);
            glUseProgram(0);
        }
    };


    class   CELL3RDCamera
    {
    public:
        float3      _eye;
        float3      _up;
        float3      _right;
        float3      _target;
        float3      _dir;
        float       _radius;
        matrix4     _matView;
        matrix4     _matProj;
        matrix4     _matWorld;
        float2      _viewSize;

        float       _yaw;

    public:
        CELL3RDCamera()
        {
            _radius     =   400;
            _yaw        =   0;
            _viewSize   =   float2(100,100);
            _matView.identify();
            _matProj.identify();
            _matWorld.identify();
        }

        float   getRadius() const 
        {
            return _radius;
        }
        void    setRadius(float val)
        { 
            _radius = val; 
        }

        CELL::float3 getEye() const 
        { 
            return _eye;
        }
        /**
        *   设置眼睛的位置
        */
        void    setEye(CELL::float3 val)
        { 
            _eye    =   val; 
        }
        /**
        *   计算方向
        */
        void    calcDir()
        {
            _dir    =   _target - _eye;	
            _dir    =   normalize(_dir);
        }

        CELL::float3 getTarget() const 
        { 
            return _target;
        }

        void    setTarget(CELL::float3 val) 
        { 
            _target = val;
        }
        
        CELL::float3 getUp() const 
        { 
            return _up;
        }
        void    setUp(CELL::float3 val)
        {
            _up = val;
        }
        float3  getDir() const
        {
            return  _dir;
        }

        float3  getRight() const
        {
            return  _right;
        }
        void    update()
        {
            float3  upDir   =   normalize(_up);
            _eye        =   _target - _dir * _radius;
            _right      =   normalize(cross(_dir, upDir));

            _matView    =   CELL::lookAt(_eye,_target,_up);
        }

        void    setViewSize(const float2& viewSize)
        {
            _viewSize   =   viewSize;
        }
        void    setViewSize(float x,float y)
        {
            _viewSize   =   float2(x,y);
        }

        float2  getViewSize()
        {
            return  _viewSize;
        }
        
        void    setProject(const matrix4& proj)
        {
            _matProj    =   proj;
        }
        const matrix4& getProject() const
        {
            return  _matProj;
        }
        const matrix4&  getView() const
        {
            return  _matView;
        }

        void    perspective(float fovy, float aspect, float zNear, float zFar)
        {
            _matProj    =   CELL::perspective<float>(fovy,aspect,zNear,zFar);
        }

         /**
        *   世界坐标转化为窗口坐标
        */
        bool    project( const float4& world, float4& screen )
        {
            screen  =   (_matProj * _matView * _matWorld) * world;
            if (screen.w == 0.0f)
            {
                return false;
            }
            screen.x    /=  screen.w;
            screen.y    /=  screen.w;
            screen.z    /=  screen.w;

            // map to range 0 - 1
            screen.x    =   screen.x * 0.5f + 0.5f;
            screen.y    =   screen.y * 0.5f + 0.5f;
            screen.z    =   screen.z * 0.5f + 0.5f;

            // map to viewport
            screen.x    =   screen.x * _viewSize.x;
            screen.y    =   _viewSize.y - (screen.y * _viewSize.y);
            return  true;
        }


        /**
        *   世界坐标转化为窗口坐标
        */
        float2  worldToScreen( const float3& world)
        {
            float4  worlds(world.x,world.y,world.z,1);
            float4  screens;
            project(worlds,screens);
            return  float2(screens.x,screens.y);
        }

        /**
        *   窗口坐标转化为世界坐标
        */
        float3  screenToWorld(const float2& screen)
        {
            float4  screens(screen.x,screen.y,0,1);
            float4  world;
            unProject(screens,world);
            return  float3(world.x,world.y,world.z);
        }

        float3  screenToWorld(float x,float y)
        {
            float4  screens(x,y,0,1);
            float4  world;
            unProject(screens,world);
            return  float3(world.x,world.y,world.z);
        }


        /**
        *   窗口坐标转化为世界坐标
        */
        bool    unProject( const float4& screen, float4& world )
        {
            float4 v;
            v.x =   screen.x;
            v.y =   screen.y;
            v.z =   screen.z;
            v.w =   1.0;

            // map from viewport to 0 - 1
            v.x =   (v.x) /_viewSize.x;
            v.y =   (_viewSize.y - v.y) /_viewSize.y;
            //v.y = (v.y - _viewPort.Y) / _viewPort.Height;

            // map to range -1 to 1
            v.x =   v.x * 2.0f - 1.0f;
            v.y =   v.y * 2.0f - 1.0f;
            v.z =   v.z * 2.0f - 1.0f;

            CELL::matrix4  inverse = (_matProj * _matView * _matWorld).inverse();

            v   =   v * inverse;
            if (v.w == 0.0f)
            {
                return false;
            }
            world   =   v / v.w;
            return true;
        }

        Ray createRayFromScreen(int x,int y)
        {
            float4  minWorld;
            float4  maxWorld;

            float4  screen(float(x),float(y),0,1);
            float4  screen1(float(x),float(y),1,1);

            unProject(screen,minWorld);
            unProject(screen1,maxWorld);
            Ray     ray;
            ray.setOrigin(float3(minWorld.x,minWorld.y,minWorld.z));

            float3  dir(maxWorld.x - minWorld.x,maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
            ray.setDirection(normalize(dir));
            return  ray;
        }
        /**
        *   下面的函数的功能是将摄像机的观察方向绕某个方向轴旋转一定的角度 
        *   改变观察者的位置，目标的位置不变化
        */
        void    rotateView(float angle) 
        { 
            _dir    =   rotateY<float>(_dir,angle);
        }
    };


    class   Role
    {
    public:
        float3  _pos;
        float3  _target;
        float   _speed;
    public:
        Role()
        {
            _speed  =   5;
        }
        /**
        *   设置移动的目标点
        */
        void    setTarget(float3 target)
        {
            _target =   target;
        }
        /**
        *   更新位置
        */
        void    setPosition(float3 pos)
        {
            _pos    =   pos;
            _pos.y  =   1;
        }

        void moveCheck(const float elasped)
        {
            /**
            *   目标位置不是当前位置。
            */
            if (_target == _pos)
            {
                return;
            }
            /**
            *   获取当前玩家位置与目标位置的偏移量
            */
            float3  offset  =   _target - _pos;
            /**
            *   获取到移动的方向
            */
            float3  dir     =   normalize(offset);
            
            if (distance(_target,_pos) > 1)
            {
                float   speed   =   elasped * _speed;

                _pos    +=  float3(dir.x * speed,0,dir.z  * speed) ;
            }
            else
            {
                _target  = _pos;
            }
        }
        /**
        *   绘制角色
        */
        void    render(float fElapsed)
        {
            moveCheck(fElapsed);
        }
    };



    struct Vertex
    {
        float x, y, z;
        float u,v;
        float r, g, b,a;
        float nx,ny,nz;
    };

    Vertex  boxVertex[36] =
    {
        //    tu   tv     r    g    b    a   nx    ny    nz       x     y     z 
        
        {-1.0f,-1.0f, 1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f, 1.0f },
        { 1.0f,-1.0f, 1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f, 1.0f },

        {-1.0f,-1.0f, 1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f, 1.0f },
        {-1.0f, 1.0f, 1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f, 1.0f },

        {-1.0f,-1.0f,-1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f,-1.0f },
        {-1.0f, 1.0f,-1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f,-1.0f },
        { 1.0f, 1.0f,-1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f,-1.0f },

        {-1.0f,-1.0f,-1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f,-1.0f },
        { 1.0f, 1.0f,-1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f,-1.0f },
        { 1.0f,-1.0f,-1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f,-1.0f },

        {-1.0f, 1.0f,-1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f },
        {-1.0f, 1.0f, 1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f },

        {-1.0f, 1.0f,-1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f,-1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 1.0f, 0.0f },

        {-1.0f,-1.0f,-1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,-1.0f, 0.0f },
        { 1.0f,-1.0f,-1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,-1.0f, 0.0f },
        { 1.0f,-1.0f, 1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,-1.0f, 0.0f },

        {-1.0f,-1.0f,-1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,-1.0f, 0.0f },
        { 1.0f,-1.0f, 1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,-1.0f, 0.0f },
        {-1.0f,-1.0f, 1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f,-1.0f, 0.0f },

        { 1.0f,-1.0f,-1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f,-1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f },

        { 1.0f,-1.0f,-1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f, 1.0f, 1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f },
        { 1.0f,-1.0f, 1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f,  1.0f, 0.0f, 0.0f },

        {-1.0f,-1.0f,-1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f, -1.0f, 0.0f, 0.0f },
        {-1.0f,-1.0f, 1.0f,  1.0f,0.0f,  1.0f,1.0f,1.0f,1.0f, -1.0f, 0.0f, 0.0f },
        {-1.0f, 1.0f, 1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f, -1.0f, 0.0f, 0.0f },

        {-1.0f,-1.0f,-1.0f,  0.0f,0.0f,  1.0f,1.0f,1.0f,1.0f, -1.0f, 0.0f, 0.0f },
        {-1.0f, 1.0f, 1.0f,  1.0f,1.0f,  1.0f,1.0f,1.0f,1.0f, -1.0f, 0.0f, 0.0f },
        {-1.0f, 1.0f,-1.0f,  0.0f,1.0f,  1.0f,1.0f,1.0f,1.0f, -1.0f, 0.0f, 0.0f }
    };

    class   GLES20Frame :public CELLFrame
    {
    protected:

        PROGRAM_P2_T2_C3    _shader;
        Texture2dId         _texture;
        Texture2dId         _textureRole;
        CELL3RDCamera       _camera;

        PROGRAM_DIR_LIGHT0  _lighting;

        Role                _role;

        bool                _rightButtonDown;
        float2              _mousePos;

    public:
        GLES20Frame(CELLOpenGL& device,void* user = 0)
            :CELLFrame(device,user)
        {
            _rightButtonDown    =   false;
        }
        /**
        *   入口函数
        */
        virtual int     main(int /*argc*/,char** /*argv*/) 
        {   
            _device.enableRenderState(GL_DEPTH_TEST);
            _shader.initialize(_device);
            _lighting.initialize(_device);

            _texture    =   _device.createTexture2DFromFile("data/image/1.tex");
            _textureRole=   _device.createTexture2DFromFile("data/image/main.tex");


            _role.setPosition(float3(0,0,0));
            _role.setTarget(float3(50,0,50));

            _camera.setRadius(50);
            _camera.setEye(float3(50,50,50));
            _camera.setTarget(float3(0,0,0));
            _camera.calcDir();
            _camera.setUp(float3(0,1,0));

            return 0;   
        }

        /**
        *   绘制函数
        */
        virtual void    onRender(const FrameEvent& /*evt*/,int width,int height)
        {
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glViewport(0,0,width,height);


            _camera.setViewSize(width,height);

            _role.render(1.0f/60.0f);
            
            _camera.setTarget(_role._pos);

            _camera.update();

            CELL::matrix4   matModel;
            matModel.translate(_role._pos);

            CELL::matrix4   matView =   _camera.getView();

            CELL::matrix4   matProj =   CELL::perspective<float>(45.0f,float(width)/float(height),0.1f,1000.0f);

            _camera.setProject(matProj);

            CELL::matrix4   MVP     =   matProj * matView ;
           

            float   gSize   =   100;
            float   gPos    =   0;
            float   rept    =   100;

            Vertex grounds[] =
            {
                { -gSize, gPos,-gSize,0.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f,0,1,0 },
                {  gSize, gPos,-gSize,rept, 0.0f,1.0f, 1.0f, 1.0f,1.0f,0,1,0 },
                {  gSize, gPos, gSize,rept, rept,1.0f, 1.0f, 1.0f,1.0f,0,1,0 },

                { -gSize, gPos,-gSize,0.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f,0,1,0 },
                {  gSize, gPos, gSize,rept, rept,1.0f, 1.0f, 1.0f,1.0f,0,1,0 },
                { -gSize, gPos, gSize,0.0f, rept,1.0f, 1.0f, 1.0f,1.0f,0,1,0 },
            };

            _device.bindTexture2D(&_texture);

            _shader.begin();
            {
                
                _device.setUniformMatrix4fv(_shader._MVP,1,false,MVP.data());
                _device.setUniform1i(_shader._texture,0);
                _device.attributePointer(_shader._positionAttr, 3,  GL_FLOAT,false,sizeof(Vertex),&grounds[0].x);
                _device.attributePointer(_shader._uvAttr,       2,  GL_FLOAT,false,sizeof(Vertex),&grounds[0].u);
                _device.attributePointer(_shader._colorAttr,    4,  GL_FLOAT,false,sizeof(Vertex),&grounds[0].r);

                _device.drawArray(GL_TRIANGLES,0,6);
            }
            _shader.end();

            _lighting.begin();
            {
                CELL::matrix4   matRot(1);
                CELL::matrix4   matModel;
                matModel.translate(_role._pos);
                static  float agle = 0;
                matRot.rotateY(agle);
                agle    +=  1.0f;
                MVP     =   matProj * matView * ( matModel * matRot );
                CELL::matrix4   mv      =   matView *(matModel * matRot);
                CELL::matrix4   mvInv   =   mv.inverse();

                float fEyePosition[] = { _camera._eye.x, _camera._eye.y, _camera._eye.z, 0.0f};
                float fLightVector[] = { 0.0f, 1.0f, 0.0f, 0.0f};

                // Normalize light vector
                float fLength = sqrtf( fLightVector[0]*fLightVector[0] +
                    fLightVector[1]*fLightVector[1] +
                    fLightVector[2]*fLightVector[2] );
                fLightVector[0] /= fLength;
                fLightVector[1] /= fLength;
                fLightVector[2] /= fLength;


                //! 绘制地面
                glUniformMatrix4fv(_lighting.modelViewProjection, 1, false, MVP.data());
                //! 绘制地面
                glUniformMatrix4fv(_lighting.modelViewInverse, 1, false, mvInv.data());


                glUniform3fv(_lighting.eyePosition, 1,fEyePosition);
                glUniform3fv(_lighting.lightVector, 1,fLightVector);


                glVertexAttribPointer(_lighting._positionAttr,3,  GL_FLOAT,   false,  sizeof(Vertex),&boxVertex[0].x);
                glVertexAttribPointer(_lighting._normal,3,     GL_FLOAT,   false,  sizeof(Vertex),&boxVertex[0].nx);

                glDrawArrays(GL_TRIANGLES,0,36 );
            }
            _lighting.end();

        }

        /**
        *   鼠标移动
        */
        virtual void    onMouseMove(int absx, int absy, int absz)
        {
            if (absz > 0)
            {
                _camera._radius = _camera._radius * 1.1f;
            }
            else if(absz < 0)
            {
                _camera._radius = _camera._radius * 0.9f;
            }
            else if(_rightButtonDown)
            {
                float2  curPos(absx,absy);
                float2  offset      =   curPos - _mousePos;
                        _mousePos   =   curPos;
                _camera.rotateView(offset.x * 0.1f);
                _camera.update();
            }
        }
        /**
        *   鼠标按下
        */
        virtual void    onMousePress(int absx, int absy, CELL::MouseButton id)
        {

            if (id == CELL::MouseButton::Left)
            {
                Ray     ray =   _camera.createRayFromScreen(absx,absy);

                float3  pos     =   ray.getOrigin();
                float   tm      =   abs((pos.y) / ray.getDirection().y); 
                float3  target  =   ray.getPoint(tm);
                _role.setTarget(float3(target.x,0,target.z));
            }
            else if( id== CELL::MouseButton::Right)
            {
                _mousePos           =   float2(absx,absy);
                _rightButtonDown    =   true;
            }
        }
      
        /**
        *   鼠标释放
        */
        virtual void    onMouseRelease(int /*absx*/, int /*absy*/, CELL::MouseButton id)
        {
            if( id == CELL::MouseButton::Right)
            {
                _rightButtonDown    =   false;
            }
        }


    };
}