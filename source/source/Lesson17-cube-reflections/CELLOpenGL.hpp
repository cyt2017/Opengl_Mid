#pragma once

#include "CELLMath.hpp"
#include <stdlib.h>
#include "gles2/gl2.h"
#define	CELL_API




namespace   CELL
{
    class   CELLOpenGL;

    /**
    *   ����һ���ַ�����Ϣ
    *   �������ϵ�λ�ã�ƫ�ƾ��룬�Լ�����һ��������
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
        *   �洢��ǰ�ַ��������ϵ�����λ��
        *   ����1024��С���������Դ泣�õĺ���(16����)
        */
        unsigned int   x0:10;
        unsigned int   y0:10;
        unsigned int   x1:10;
        unsigned int   y1:10;
        //! �����ƫ��x����
        unsigned int   offsetX:8;
        //! �����ƫ��y����
        unsigned int   offsetY:8;
        //! ��������У����256��
        unsigned int   texIndex:8;
    };
    /**
    *   ʱ��
    */
    class   FrameEvent
    {
    public:
        /**
        *   ��λ��
        */
        real    _sinceLastFrame;
        /**
        *   ��λ��
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
        *   ���ʻ���������������OpenGL��Ӧ�ó���ֱ�ӵĽӿ�
        */
        virtual void    initialize(CELLOpenGL* device);
        /**
        *   ʹ�ó���
        */
        virtual void    begin();
        /**
        *   ʹ�����
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
        *   ���� attribute
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
        *   ���ʻ���������������OpenGL��Ӧ�ó���ֱ�ӵĽӿ�
        */
        virtual void    initialize(CELLOpenGL* device);
        /**
        *   ʹ�ó���
        */
        virtual void    begin();
        /**
        *   ʹ�����
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
        *   ϵͳ����ʹ�õ�����2D GL_ALPHA��ʽ
        */
        unsigned    _sysFontTexture;
        Character   _character[1<<16];
        void*       _library;
        void*       _face; 
        char*       _fontBuffer;
        //! ��������Ŀ��
        int         _textureWidth;
        //! ��������ĸ߶�
        int         _textureHeight;
        //! ��ʱ���������浱ǰ����д�������ϵ�λ������
        int         _yStart;
        int         _xStart;
        //! �����С
        int         _fontSize;
        //! ��������Ĵ�С��������������ش�С��������Ҫ��һ��������������������ش�С
        int         _fontPixelX;
        int         _fontPixelY;
        //! ���������Ӧ��shader
        CELLUIProgram   _uiProgram;

    public:
        CELLOpenGL();

        virtual ~CELLOpenGL();

        /**
        *   ��ʼ��OpenGL
        */
        void        initialize();
        /**
        *   ����
        */
        void        destroy();
        /**
        *   ��������ϵͳ
        */
        void        buildSystemFont(const char* font,int size);
        /**
        *   ��ȡ�ַ�����Ϣ
        */
        Character   getCharacter(wchar_t ch);

        /**
        *   ��ʼ�����ı�
        */
        void        beginText(const matrix4r& proj);

        void        endText();
        /**
        *   �����ı�
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
        *   �����ı�
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
        *   ����2D����
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
        *   ͨ���ļ���������
        */
        static Texture2dId  createTexture2DFromFile(const char* fileName);
        /**
        *   ��������д���ݣ���������������
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
        *   ��ָ����������copy����
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
        *   ɾ������
        */
        static  void        destroyTexture2D(Texture2dId& id);

        /**
        *   ������������
        */
        static  TextureCubeId   createCubeTextureFromFile(const char* files[]);

        static  void        bindTextureCube(TextureCubeId* tex,int stage = 0);
        static  void        destroyTextureCube(TextureCubeId& id);

        

        /**
        *   ����һ����Ⱦ������������
        */
        static  FrameBufferId   createFrameBuffer(int width,int height);

        static  void        resizeFrameBuffer(FrameBufferId bufferId,int width,int height);
        /**
        *   ɾ��һ����Ⱦ����
        */
        static  void        destroyFrameBuffer(FrameBufferId& frameBuffer);
        /**
        *   ��һ����������ȾĿ��󶨣���ʹ��
        */
        static  void        beginFrameBuffer(const FrameBufferId& targetId,const Texture2dId& textureId);
        /**
        *   �����Ⱦ��Ŀ��
        *   ��beginRenderToTarget ��endRenderToTarget֮����ӻ��ƴ���
        */
        static  void        endFrameBuffer();
        /**
        *   ʹ����Ⱦ����
        */
        static  void        bindFrameBuffer(const FrameBufferId*targetId);
        /**
        *   level:  lod����
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
        *   �������㻺����
        */
        static  VertexBufferId  createVertexBuffer(unsigned size,unsigned acc,const void* data);
        static  void            updateVertexBuffer(unsigned offset,unsigned size,const void* data);
        //! ��ȡ����
        static  void            getVertexBuffer(int offset, int size, void* data);

        static  void            bindVertexBuffer(const VertexBufferId* id);
        static  void            destroyVertexBuffer(const VertexBufferId& id);
        
            

        /**
        *   �������㻺����
        */
        static  IndexBufferId   createIndexBuffer(unsigned size,unsigned acc,const void* data);
        static  void            updateIndexBuffer(unsigned offset,unsigned size,const void* data);

        static  void            bindIndexBuffer(const IndexBufferId* id);
        static  void            destroyIndexBuffer(const IndexBufferId& id);



        /**
        *   shader���
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
        *   �����ӿ�
        */
        static  void    setViewPort(int x,int y,int width,int height);
        /**
        *   ������Ⱦ״̬
        */
        static  void    enableRenderState(unsigned state);
        static  void    disableRenderState(unsigned state);
        static  void    scissor(int x,int y,int width,int height);
        static  void    blendFunction(unsigned sfactor, unsigned dfactor);
        
        /**
        *   ��ȡshader�е������ֶ�
        */
        static  void    enableVertexAttributeArray(unsigned attrIndex);
        static  void    disableVertexAttributeArray(unsigned attrIndex);

        static  void    attributePointer( int index, int size, int type, int normalized, int stride, const void* pointer );

        static  void    drawArray(unsigned mode,int first,int count);
        static  void    drawElements(unsigned mode, int count,int type,const void* data);

        static  void    cullFace(unsigned face);

    };
}
