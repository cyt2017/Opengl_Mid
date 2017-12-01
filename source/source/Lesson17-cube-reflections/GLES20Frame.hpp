#pragma once

#include "CELLFrame.hpp"
#include "rapidxml.hpp"

#include "CELLProgramLibrary.hpp"
#include "CELLModel.hpp"

namespace   CELL
{
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
        *   �����ƶ���Ŀ���
        */
        void    setTarget(float3 target)
        {
            _target =   target;
        }
        /**
        *   ����λ��
        */
        void    setPosition(float3 pos)
        {
            _pos    =   pos;
            _pos.y  =   1;
        }

        void moveCheck(const float elasped)
        {
            /**
            *   Ŀ��λ�ò��ǵ�ǰλ�á�
            */
            if (_target == _pos)
            {
                return;
            }
            /**
            *   ��ȡ��ǰ���λ����Ŀ��λ�õ�ƫ����
            */
            float3  offset  =   _target - _pos;
            /**
            *   ��ȡ���ƶ��ķ���
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
        *   ���ƽ�ɫ
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
        Program_Reflections _bumpShader;
        Texture2dId         _texture;
        Texture2dId         _textureRole;
        CELL3RDCamera       _camera;

        CELLModel           _model;

        Role                _role;

        bool                _rightButtonDown;
        float2              _mousePos;

        TextureCubeId       _textureBase;
    public:
        GLES20Frame(CELLOpenGL& device,void* user = 0)
            :CELLFrame(device,user)
            ,_shader(device)
            ,_bumpShader(device)
            ,_model(device)
        {
            _rightButtonDown    =   false;
        }
        /**
        *   ��ں���
        */
        virtual int     main(int /*argc*/,char** /*argv*/) 
        {   
            _device.enableRenderState(GL_DEPTH_TEST);
            _shader.initialize();
            _bumpShader.initialize();

            _model.load("data/model/PerturbedUvs.xml");

            _texture        =   _device.createTexture2DFromFile("data/image/1.tex");
            _textureRole    =   _device.createTexture2DFromFile("data/image/main.tex");

            const char*       cubeMap[]  =   
            {
                "data/image/skybox/POS_X.tex",
                "data/image/skybox/NEG_X.tex",

                "data/image/skybox/POS_Y.tex",
                "data/image/skybox/NEG_Y.tex",

                "data/image/skybox/POS_Z.tex",
                "data/image/skybox/NEG_Z.tex",
            };
            _textureBase    =   _device.createCubeTextureFromFile(cubeMap);


            _role.setPosition(float3(0,0,0));
            _role.setTarget(float3(50,0,50));

            _camera.setRadius(50);
            _camera.setEye(float3(50,50,50));
            _camera.setTarget(float3(0,0,0));
            _camera.calcDir();
            _camera.setUp(float3(0,1,0));


            GLuint     tex;
            glGenTextures(1,&tex);

            glBindTexture(GL_TEXTURE_2D,tex);
            glTexImage2D(
                GL_TEXTURE_2D, 
                0, 
                GL_RGB,
                3840, 
                2160,
                0, 
                GL_RGB , 
                GL_UNSIGNED_BYTE,
                0
                );

            char*  data = new char[3840 * 2160 * 3];
            DWORD dw = GetTickCount();
            for(int i = 0 ; i < 100 ; ++ i)
            {
                glTexSubImage2D(
                    GL_TEXTURE_2D, 
                    0,          //! lod����
                    0,        //! xƫ����
                    0,        //! yƫ����
                    3840, 
                    2160, 
                    GL_RGB,      

                    GL_UNSIGNED_BYTE, 
                    data
                    );
            }

            DWORD dw11 = GetTickCount() - dw;

            return 0;   
        }

        /**
        *   ���ƺ���
        */
        virtual void    onRender(const FrameEvent& /*evt*/,int width,int height)
        {

            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glViewport(0,0,width,height);


            _camera.setViewSize((float)width,(float)height);

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

            _bumpShader.begin();
            {
                _device.bindTextureCube(&_textureBase);

                _device.setUniform1i(_bumpShader.sCubeMap,0);

                _model.onRender(_bumpShader,_camera);
            }
            _bumpShader.end();

        }

        /**
        *   ����ƶ�
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
        *   ��갴��
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
        *   ����ͷ�
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