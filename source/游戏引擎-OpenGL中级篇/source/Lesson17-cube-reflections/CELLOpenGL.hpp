#pragma once

#include "CELLMath.hpp"
#include <stdlib.h>
#include "gles2/gl2.h"
#define	CELL_API




namespace   CELL
{
    class   CELLOpenGL;

    /**
    *   描述一个字符的信息
    *   在纹理上的位置，偏移距离，以及在哪一个纹理上
    */
    class   CELL_API Character
    {
    public:
        Character()
        {
            x0          =   0;
            y0          =   0;
            x1          =   0;
            y1          =   0;
            offsetX     =   0;
            offsetY     =   0;
        }
        /**
        *   存储当前字符在纹理上的坐标位置
        *   采用1024大小的纹理，可以存常用的汉字(16像素)
        */
        unsigned int   x0:10;
        unsigned int   y0:10;
        unsigned int   x1:10;
        unsigned int   y1:10;
        //! 字体的偏移x方向
        unsigned int   offsetX:8;
        //! 字体的偏移y方向
        unsigned int   offsetY:8;
        //! 纹理的所有，最大256个
        unsigned int   texIndex:8;
    };
    /**
    *   时间
    */
    class   FrameEvent
    {
    public:
        /**
        *   单位秒
        */
        real    _sinceLastFrame;
        /**
        *   单位秒
        */
        real    _sinceLastEvent;
    };

    class   FrameBufferId
    {
    public:
        FrameBufferId()
        {
            _width  =   0;
            _height =   0;
            _FBOID  =   0;
            _RBOID  =   0;
        }
        unsigned    _width;
        unsigned    _height;
        unsigned    _FBOID;
        unsigned    _RBOID;
    };


    class   Texture2dId
    {
    public:
        Texture2dId()
        {
            memset(this,0,sizeof(Texture2dId));
        }
        unsigned    _texture;
        unsigned    _width;
        unsigned    _height;
        void*       _user;
    };

    class   TextureCubeId
    {
    public:
        TextureCubeId()
        {
            memset(this,0,sizeof(TextureCubeId));
        }
        unsigned    _texture;
        unsigned    _width;
        unsigned    _height;
        void*       _user;
    };


    class   VertexBufferId
    {
    public:
        VertexBufferId()
        {
            _bufferId   =   unsigned(-1);
            _buffserSize=   0;
            _accessType =   0;
            _user       =   0;
        }
        unsigned    _bufferId;
        unsigned    _buffserSize;
        unsigned    _accessType;
        void*       _user;
    };


    class   IndexBufferId
    {
    public:
        IndexBufferId()
        {
            _bufferId   =   0xffffffff;
        }
        unsigned    _bufferId;
        unsigned    _buffserSize;
        unsigned    _accessType;
        short       _elementType;
        short       _primitive;
    };

    class   PixelBufferId
    {
    public:
        PixelBufferId()
        {
            _bufferId   =   unsigned(-1);
            _usage      =   0;
            _access     =   0;
            _user       =   0;
        }
        unsigned    _bufferId;
        unsigned    _buffserSize;
        unsigned    _usage;
        unsigned    _access;
        void*       _user;
    };


    class   CELL_API ShaderId
    {
    public:
        ShaderId()
        {
            _shaderId   =   -1;
        }
        int _shaderId;
    };

    class   CELL_API ProgramId
    {
    public:
        ProgramId()
        {
            _programId  =   -1;
        }
        int         _programId;
        ShaderId    _vertex;
        ShaderId    _fragment;
    };

    class   CELL_API Program :public ProgramId
    {
    public:
        typedef     int location;
        typedef     int atrribute;
    protected:
        CELLOpenGL* _device;
    public:
        Program();

        virtual ~Program();

        virtual	void 	destroy();
        /**
        *   舒适化函数，做基本的OpenGL和应用程序直接的接口
        */
        virtual void    initialize(CELLOpenGL* device);
        /**
        *   使用程序
        */
        virtual void    begin();
        /**
        *   使用完成
        */
        virtual void    end();

        /**
        *   Retrieve Location (index) of a Uniform Variable
        */
        static int      getUniformLocation(const ProgramId& program,const char *name);  

        static int      getAttribLocation(const ProgramId& program,const char* name);
       
        static void     setUniform1f(int index,float v0);
        static void     setUniform2f(int index,float v0, float v1);
        static void     setUniform3f(int index,float v0, float v1, float v2);;
        static void     setUniform4f(int index,float v0, float v1, float v2, float v3);;

        static void     setUniform1i(int index,int v0);
        static void     setUniform2i(int index,int v0, int v1);
        static void     setUniform3i(int index,int v0, int v1, int v2);
        static void     setUniform4i(int index,int v0, int v1, int v2, int v3);

        // Arrays
        static void     setUniform1fv(int index,int count, const float *value);
        static void     setUniform2fv(int index,int count, const float *value);
        static void     setUniform3fv(int index,int count, const float *value);
        static void     setUniform4fv(int index,int count, const float *value);

        static void     setUniform1iv(int index,int count, const int *value);
        static void     setUniform2iv(int index,int count, const int *value);
        static void     setUniform3iv(int index,int count, const int *value);
        static void     setUniform4iv(int index,int count, const int *value);

        static void     setUniformMatrix2fv(int index,int count, bool transpose, const float *value);
        static void     setUniformMatrix3fv(int index,int count, bool transpose, const float *value);
        static void     setUniformMatrix4fv(int index,int count, bool transpose, const float *value);

        static void     bindAttribLocation(const ProgramId& programId, int index, const char* name);

        //float
        static void     setVertexAttrib1f(unsigned index, float v0);
        static void     setVertexAttrib2f(unsigned index, float v0, float v1); 
        static void     setVertexAttrib3f(unsigned index, float v0, float v1, float v2);
        static void     setVertexAttrib4f(unsigned index, float v0, float v1, float v2, float v3);



    };


    class   CELLMaterial
    {
    public:

        Program*    _program;
        Texture2dId _texture;
    };


    struct  CELLUIVertex
    {
        float       x,y,z;
        float       u,v,w;
        Rgba4Byte   color;
    };

    class   CELL_API CELLUIProgram :public Program
    {
    protected:
        /**
        *   属性 attribute
        */
        location    _position;
        location    _color;
        location    _uv;
        /**
        *   uniform
        */
        location    _MVP;
        location    _texture;
    public:
        CELLUIProgram()
        {
            _position   =   -1;
            _color      =   -1;
            _uv         =   -1;
            _texture    =   -1;
            _MVP        =   -1;
        }
        virtual ~CELLUIProgram()
        {}

        location    getPositionAttribute() const
        {
            return  _position;
        }
        location    getColorAttribute() const
        {
            return  _color;
        }
        location    getUV1Attribute() const
        {
            return  _uv;
        }
        location    getMVPUniform() const 
        {
            return  _MVP;
        }

        location    getTexture1Uniform() const
        {
            return  _texture;
        }
        /**
        *   舒适化函数，做基本的OpenGL和应用程序直接的接口
        */
        virtual void    initialize(CELLOpenGL* device);
        /**
        *   使用程序
        */
        virtual void    begin();
        /**
        *   使用完成
        */
        virtual void    end();
    };

    /**
    *    [8/4/2013 Administrator]
    */
    class   CELL_API CELLOpenGL
    {
    public:
        /**
        *   系统字体使用的纹理2D GL_ALPHA格式
        */
        unsigned    _sysFontTexture;
        Character   _character[1<<16];
        void*       _library;
        void*       _face; 
        char*       _fontBuffer;
        //! 字体文理的宽度
        int         _textureWidth;
        //! 字体文理的高度
        int         _textureHeight;
        //! 临时变量，保存当前字体写到文理上的位置坐标
        int         _yStart;
        int         _xStart;
        //! 字体大小
        int         _fontSize;
        //! 上述字体的大小并不是字体的像素大小，所以需要用一个变量来计算字体的像素大小
        int         _fontPixelX;
        int         _fontPixelY;
        //! 绘制字体对应的shader
        CELLUIProgram   _uiProgram;

    public:
        CELLOpenGL();

        virtual ~CELLOpenGL();

        /**
        *   初始化OpenGL
        */
        void        initialize();
        /**
        *   销毁
        */
        void        destroy();
        /**
        *   建立字体系统
        */
        void        buildSystemFont(const char* font,int size);
        /**
        *   获取字符的信息
        */
        Character   getCharacter(wchar_t ch);

        /**
        *   开始绘制文本
        */
        void        beginText(const matrix4r& proj);

        void        endText();
        /**
        *   绘制文本
        */
        float2      drawText(
                        float x,
                        float y,
                        float z,
                        Rgba4Byte color,
                        const wchar_t* text,
                        size_t length = -1,
                        CELLUIVertex**  vertexs = 0,
                        size_t*         vertexLength = 0
                        );

        /**
        *   绘制文本
        */
        float2      drawTextToBuffer(
                        float x,
                        float y,
                        float z,
                        Rgba4Byte color,
                        const wchar_t* text,
                        size_t length = -1,
                        CELLUIVertex**  vertexs = 0,
                        size_t*         vertexLength = 0
                        );

        
        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

   
        static  void    clear(unsigned mask);
        static  void    clearColor(float r,float g,float b,float a);


        static  void    setLineWidth(float width);

        /**
        *   创建2D纹理
        */
        static  Texture2dId createTexure2D(
                                            int level,
                                            int intFmt,
                                            int width,
                                            int height,
                                            int border,
                                            int srcFmt,
                                            int type,
                                            const void* data,
                                            void* userData
                                            );

        static  Texture2dId createTexture2DFromStream(const char* stream,unsigned length);
        /**
        *   通过文件加载文理
        */
        static Texture2dId  createTexture2DFromFile(const char* fileName);
        /**
        *   向文理中写数据，即更新文理数据
        */
        static  void        textureImage2D (
                                            Texture2dId& texId,
                                            int level, 
                                            int fmt, 
                                            int width, 
                                            int height, 
                                            int border,
                                            unsigned format,
                                            unsigned type, 
                                            const void *pixels
                                            );
        static  void        bindTexture2D(const Texture2dId* textureId,unsigned stage = 0);

        static  void        bindTexture(unsigned target,unsigned id,unsigned stage);
        /**
        *   向指定的纹理中copy数据
        */
        static  void        copyTexSubImage2D(
                                            int level, 
                                            int xoffset, 
                                            int yoffset, 
                                            int x, 
                                            int y, 
                                            int width, 
                                            int height
                                            );

        static void         getTexImage( Texture2dId& id,unsigned fmt, unsigned type,void* buffer);
        /**
        *   删除纹理
        */
        static  void        destroyTexture2D(Texture2dId& id);

        /**
        *   创建立体文理
        */
        static  TextureCubeId   createCubeTextureFromFile(const char* files[]);

        static  void        bindTextureCube(TextureCubeId* tex,int stage = 0);
        static  void        destroyTextureCube(TextureCubeId& id);

        

        /**
        *   创建一个渲染对象，离屏对象
        */
        static  FrameBufferId   createFrameBuffer(int width,int height);

        static  void        resizeFrameBuffer(FrameBufferId bufferId,int width,int height);
        /**
        *   删除一个渲染对象
        */
        static  void        destroyFrameBuffer(FrameBufferId& frameBuffer);
        /**
        *   将一个纹理与渲染目标绑定，并使用
        */
        static  void        beginFrameBuffer(const FrameBufferId& targetId,const Texture2dId& textureId);
        /**
        *   完成渲染到目标
        *   在beginRenderToTarget 与endRenderToTarget之间添加绘制代码
        */
        static  void        endFrameBuffer();
        /**
        *   使用渲染对象
        */
        static  void        bindFrameBuffer(const FrameBufferId*targetId);
        /**
        *   level:  lod级别
        *   xoffset:
        *   yoffset:
        *   width:
        *   height:
        *   srcFormat:
                    //! GL_ALPHA
                    //! GL_LUMINANCE,
                    //! GL_LUMINANCE_ALPHA,
                    //! GL_RGB,GL_RGBA,GL_BGR,GL_BGRA,
                    //! GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
                    //! GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
                    //! GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        *   srcType:
                    //! GL_UNSIGNED_BYTE
                    //! GL_SHORT
                    //! GL_UNSIGNED_SHORT
        *   data:
        */
        static  void            updateTexture2D(int level,
                                                int xoffset,
                                                int yoffset,
                                                int width,
                                                int height,
                                                int srcFormat,
                                                int srcType,
                                                const void* data
                                                );
        /**
        *   创建顶点缓冲区
        */
        static  VertexBufferId  createVertexBuffer(unsigned size,unsigned acc,const void* data);
        static  void            updateVertexBuffer(unsigned offset,unsigned size,const void* data);
        //! 获取数据
        static  void            getVertexBuffer(int offset, int size, void* data);

        static  void            bindVertexBuffer(const VertexBufferId* id);
        static  void            destroyVertexBuffer(const VertexBufferId& id);
        
            

        /**
        *   创建顶点缓冲区
        */
        static  IndexBufferId   createIndexBuffer(unsigned size,unsigned acc,const void* data);
        static  void            updateIndexBuffer(unsigned offset,unsigned size,const void* data);

        static  void            bindIndexBuffer(const IndexBufferId* id);
        static  void            destroyIndexBuffer(const IndexBufferId& id);



        /**
        *   shader相关
        */
        static  ProgramId       createProgram(const char* vertex,const char* fragment);
        static  ProgramId       createProgramFromFile(const char* vertexFile,const char* fragmentFile);
        static  void            destroyProgram(ProgramId& program);
        static  void            useProgram(const ProgramId* program);
        /**
        *   Retrieve Location (index) of a Uniform Variable
        */
        static int      getUniformLocation(const ProgramId& program,const char *name);  

        static int      getAttribLocation(const ProgramId& program,const char* name);
       
        static void     setUniform1f(int index,float v0);
        static void     setUniform2f(int index,float v0, float v1);
        static void     setUniform3f(int index,float v0, float v1, float v2);;
        static void     setUniform4f(int index,float v0, float v1, float v2, float v3);;

        static void     setUniform1i(int index,int v0);
        static void     setUniform2i(int index,int v0, int v1);
        static void     setUniform3i(int index,int v0, int v1, int v2);
        static void     setUniform4i(int index,int v0, int v1, int v2, int v3);

        // Arrays
        static void     setUniform1fv(int index,int count, const float *value);
        static void     setUniform2fv(int index,int count, const float *value);
        static void     setUniform3fv(int index,int count, const float *value);
        static void     setUniform4fv(int index,int count, const float *value);

        static void     setUniform1iv(int index,int count, const int *value);
        static void     setUniform2iv(int index,int count, const int *value);
        static void     setUniform3iv(int index,int count, const int *value);
        static void     setUniform4iv(int index,int count, const int *value);

        static void     setUniformMatrix2fv(int index,int count, bool transpose, const float *value);
        static void     setUniformMatrix3fv(int index,int count, bool transpose, const float *value);
        static void     setUniformMatrix4fv(int index,int count, bool transpose, const float *value);

        static void     bindAttribLocation(const ProgramId& programId, int index, const char* name);

        //float
        static void     setVertexAttrib1f(unsigned index, float v0);
        static void     setVertexAttrib2f(unsigned index, float v0, float v1); 
        static void     setVertexAttrib3f(unsigned index, float v0, float v1, float v2);
        static void     setVertexAttrib4f(unsigned index, float v0, float v1, float v2, float v3);
        /**
        *   设置视口
        */
        static  void    setViewPort(int x,int y,int width,int height);
        /**
        *   启用渲染状态
        */
        static  void    enableRenderState(unsigned state);
        static  void    disableRenderState(unsigned state);
        static  void    scissor(int x,int y,int width,int height);
        static  void    blendFunction(unsigned sfactor, unsigned dfactor);
        
        /**
        *   获取shader中的属性字段
        */
        static  void    enableVertexAttributeArray(unsigned attrIndex);
        static  void    disableVertexAttributeArray(unsigned attrIndex);

        static  void    attributePointer( int index, int size, int type, int normalized, int stride, const void* pointer );

        static  void    drawArray(unsigned mode,int first,int count);
        static  void    drawElements(unsigned mode, int count,int type,const void* data);

        static  void    cullFace(unsigned face);

    };
}
