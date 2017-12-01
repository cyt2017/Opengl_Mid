#pragma once

#include "CELLFrame.hpp"
#include "rapidxml.hpp"

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

        Role                _role;
    public:
        GLES20Frame(CELLOpenGL& device,void* user = 0)
            :CELLFrame(device,user)
        {
        }
        /**
        *   入口函数
        */
        virtual int     main(int /*argc*/,char** /*argv*/) 
        {   
            _shader.initialize(_device);
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


            _role.render(1.0f/60.0f);
            
            _camera.setTarget(_role._pos);

            _camera.update();

            CELL::matrix4   matModel;
            matModel.translate(_role._pos);

            CELL::matrix4   matView =   _camera.getView();

            CELL::matrix4   matProj =   CELL::perspective<float>(45.0f,float(width)/float(height),0.1f,1000.0f);

            CELL::matrix4   MVP     =   matProj * matView ;
           

            float   gSize   =   100;
            float   gPos    =   -10;
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
                _device.attributePointer(_shader._colorAttr,    2,  GL_FLOAT,false,sizeof(Vertex),&grounds[0].r);

                _device.drawArray(GL_TRIANGLES,0,6);


                _device.bindTexture2D(&_textureRole);

                MVP =   matProj * matView * matModel;
                _device.setUniformMatrix4fv(_shader._MVP,1,false,MVP.data());
                _device.attributePointer(_shader._positionAttr, 3,  GL_FLOAT,false,sizeof(Vertex),&boxVertex[0].x);
                _device.attributePointer(_shader._uvAttr,       2,  GL_FLOAT,false,sizeof(Vertex),&boxVertex[0].u);
                _device.attributePointer(_shader._colorAttr,    2,  GL_FLOAT,false,sizeof(Vertex),&boxVertex[0].r);

                _device.drawArray(GL_TRIANGLES,0,sizeof(boxVertex)/sizeof(boxVertex[0]));


            }
            _shader.end();

        }

    };
}