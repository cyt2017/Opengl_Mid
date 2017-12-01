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


    struct  Face
    {
        short   x,y,z;
    };


    class   Model
    {
    public:
        std::string         _name;
        std::vector<Face>   _arFaces;
        std::vector<Vertex> _arVertex;

    };

    class   Node
    {
    public:
        std::string     _name;
        std::string     _fileName;
        float3          _pos;
        float3          _scale;
        float3          _rot;
    };

    class   GLES20Frame :public CELLFrame
    {
    protected:

        PROGRAM_P2_T2_C3    _shader;
        Texture2dId         _texture;
        Texture2dId         _textureRole;
        CELL3RDCamera       _camera;

        Role                _role;

        bool                _rightButtonDown;
        float2              _mousePos;

        
        std::map<std::string,Model> _map;

        std::vector<Node>           _arNode;
    public:
        GLES20Frame(CELLOpenGL& device,void* user = 0)
            :CELLFrame(device,user)
        {
            _rightButtonDown    =   false;
        }

        virtual char*   readFile(const char* fileName)
        {
            FILE*   pFile   =   fopen(fileName,"rb");
            if (pFile == 0)
            {
                return  false;
            }
            fseek(pFile,0,SEEK_END);
            size_t  length  =   ftell(pFile);
            fseek(pFile,0,SEEK_SET);
            char*   pText   =   new char[length + 1];
            fread(pText,1,length,pFile);
            pText[length]   =   0;
            fclose(pFile);
            return  pText;

        }

        virtual bool    loadScene(const char* fileScene)
        {
            char*   pXML    =   readFile(fileScene);
            if (pXML == 0)
            {
                return  false;
            }

            try
            {

                rapidxml::xml_document<>    doc;
                doc.parse<0>(pXML);
                rapidxml::xml_node<>*   pRoot   =   doc.first_node();
                rapidxml::xml_node<>*   node    =   pRoot->first_node();

                for ( ; node != 0 ; node = node->next_sibling())
                {
                    rapidxml::xml_attribute<>*  attrName    =   node->first_attribute("name");
                    rapidxml::xml_attribute<>*  attrFile    =   node->first_attribute("file");
                    rapidxml::xml_attribute<>*  attrPos     =   node->first_attribute("pos");
                    rapidxml::xml_attribute<>*  attrRot     =   node->first_attribute("rot");
                    rapidxml::xml_attribute<>*  attrScale   =   node->first_attribute("scale");

                    if (_map.find(attrFile->value()) ==_map.end())
                    {
                        Model   model;
                        load(attrFile->value(),model);
                        _map[attrFile->value()] =   model;
                    }
                    Node    node;
                    node._fileName  =   attrFile->value();
                    sscanf(attrPos->value(),"%f %f %f",&node._pos.x,&node._pos.y,&node._pos.z);
                    sscanf(attrRot->value(),"%f %f %f",&node._rot.x,&node._rot.y,&node._rot.z);
                    sscanf(attrScale->value(),"%f %f %f",&node._scale.x,&node._scale.y,&node._scale.z);
                    _arNode.push_back(node);
                }
            }
            catch(...)
            {
            }

            delete  []pXML;
            return  true;
        }
        virtual bool    load(const char* fileName,Model& mode)
        {
            std::map<std::string,Model>::iterator itr = _map.find(fileName);
            char*   pXML    =   readFile(fileName);
            if (pXML == 0)
            {
                return  false;
            }

            try
            {
                
                rapidxml::xml_document<>    doc;
                doc.parse<0>(pXML);
                rapidxml::xml_node<>*   pRoot   =   doc.first_node();
                rapidxml::xml_node<>*   pMesh   =   pRoot->first_node();
                rapidxml::xml_node<>*   pFaces  =   pMesh->first_node("faceIndex");
                rapidxml::xml_node<>*   vertexs =   pMesh->first_node("vertex");

                for (rapidxml::xml_node<>* face = pFaces->first_node() ; face != 0 ; face = face->next_sibling())
                {
                    int     x,y,z;
                    const char* strFace =   face->value();
                    sscanf(strFace,"%d %d %d",&x,&y,&z);
                    Face    faceIndex    =   {x,y,z};
                    mode._arFaces.push_back(faceIndex);
                }

                for (rapidxml::xml_node<>* vert = vertexs->first_node() ; vert != 0 ; vert = vert->next_sibling())
                {
                    Vertex  vVert;
                    vVert.r =   1;
                    vVert.g =   1;
                    vVert.b =   1;
                    vVert.a =   1;
                    const char* strVert =   vert->value();
                    sscanf(strVert,"%f %f %f %f %f %f %f %f",&vVert.x,&vVert.y,&vVert.z,&vVert.u,&vVert.v,&vVert.nx,&vVert.ny,&vVert.nz);
                  
                    mode._arVertex.push_back(vVert);
                }
            }
            catch(...)
            {
            }

            delete  []pXML;
            return  true;
            
        }
        /**
        *   入口函数
        */
        virtual int     main(int /*argc*/,char** /*argv*/) 
        {   
            loadScene("data/model/scene.xml");

            _device.enableRenderState(GL_DEPTH_TEST);
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


                

                _device.bindTexture2D(&_textureRole);

                MVP =   matProj * matView * matModel;
                _device.setUniformMatrix4fv(_shader._MVP,1,false,MVP.data());
                _device.attributePointer(_shader._positionAttr, 3,  GL_FLOAT,false,sizeof(Vertex),&boxVertex[0].x);
                _device.attributePointer(_shader._uvAttr,       2,  GL_FLOAT,false,sizeof(Vertex),&boxVertex[0].u);
                _device.attributePointer(_shader._colorAttr,    4,  GL_FLOAT,false,sizeof(Vertex),&boxVertex[0].r);

                _device.drawArray(GL_TRIANGLES,0,sizeof(boxVertex)/sizeof(boxVertex[0]));


                std::vector<Node>::iterator itr = _arNode.begin();
                for ( ; itr != _arNode.end() ; ++ itr )
                {
                    Node&           node        =   *itr;
                    Model&          model       =   _map[node._fileName];
                    quaternion      quat;
                    CELL::matrix4   matWorld    =   CELL::makeTransform(node._pos,node._scale,quat);

                    MVP =   matProj * matView * matWorld;
                    _device.setUniformMatrix4fv(_shader._MVP,1,false,MVP.data());

                    _device.attributePointer(_shader._positionAttr, 3,  GL_FLOAT,false,sizeof(Vertex),&model._arVertex[0].x);
                    _device.attributePointer(_shader._uvAttr,       2,  GL_FLOAT,false,sizeof(Vertex),&model._arVertex[0].u);
                    _device.attributePointer(_shader._colorAttr,    4,  GL_FLOAT,false,sizeof(Vertex),&model._arVertex[0].r);

                    _device.bindTexture2D(&_textureRole);

                    _device.drawElements(GL_TRIANGLES,model._arFaces.size() * 3,GL_UNSIGNED_SHORT,&model._arFaces.front());
                }


            }
            _shader.end();

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