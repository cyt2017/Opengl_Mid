#include "CELLOpenGL.hpp"

#include "freetype/ftglyph.h"
#include "CELLImage.hpp"
#include "CELLImageReader.hpp"

#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H
#include FT_BITMAP_H
#include FT_WINFONTS_H


char*	readFile(const char* fileName,unsigned& length)
{
    FILE*   pFile  =   fopen(fileName,"rb");
    if (pFile)
    {
        fseek( pFile, 0, SEEK_END );
        length = ftell( pFile );
        fseek(pFile, 0, SEEK_SET );
        char* buffer =   new char[length + 1];
        fread( buffer, 1, length, pFile );
        buffer[length] = 0;
        fclose( pFile );
        return  buffer;
    }
    else
    {
        char    buffer[256];
        sprintf_s(buffer,"read %s  failed",fileName);
        assert(pFile != 0 && buffer);
    }
    return  0;

}


namespace   CELL
{

    GLenum      getSrcFormat(const char* fmt)
    {
        if (strcmp(fmt,"PIXEL_R8G8B8A8") == 0)
        {
            return  GL_RGBA;
        }
        else if(strcmp(fmt,"PIXEL_R8G8B8") == 0)
        {
            return  GL_RGB;
        }
        else if(strcmp(fmt,"PIXEL_RGBA4") == 0)
        {
            return  GL_RGBA;
        }
        else if(strcmp(fmt,"PIXEL_RGB565") == 0)
        {
            return  GL_RGB;
        }
        else if(strcmp(fmt,"PIXEL_A8") == 0)
        {
            return  GL_ALPHA;
        }
        else if(strcmp(fmt,"PIXEL_I8") == 0)
        {
            return  GL_LUMINANCE;
        }
        else if(strcmp(fmt,"PIXEL_IA8") == 0)
        {
            return  GL_LUMINANCE_ALPHA;
        }
       
        else
        {
            return  GL_RGB;
        }
    }

    GLenum      getSrcType(const char* fmt)
    {
        if (strcmp(fmt,"PIXEL_RGBA4") == 0)
        {
            return  GL_UNSIGNED_SHORT_4_4_4_4;
        }
        if (strcmp(fmt,"PIXEL_RGB565") == 0)
        {
            return  GL_UNSIGNED_SHORT_5_6_5;
        }
        else
        {
            return  GL_UNSIGNED_BYTE;
        }
    }


    GLenum      getSrcFormat(PIXEL_FORMAT fmt)
    {
        switch(fmt)
        {
        case PIXEL_A8:
            return  GL_ALPHA;
        case PIXEL_I8:
            return  GL_LUMINANCE;
        case PIXEL_IA8:
            return  GL_LUMINANCE_ALPHA;
        case PIXEL_R8G8B8:
            return  GL_RGB;
        case PIXEL_R8G8B8A8:
            return  GL_RGBA;
        case PIXEL_YUYV:
        case PIXEL_UYVY:
        case PIXEL_YUY2:
            return  GL_LUMINANCE_ALPHA;
        default:
            return  GL_LUMINANCE_ALPHA;
        }
    }
    GLenum      getSrcType(PIXEL_FORMAT fmt)
    {
        switch(fmt)
        {
        case PIXEL_R8G8B8:
        case PIXEL_R8G8B8A8:
        case PIXEL_B8G8R8:
        case PIXEL_B8G8R8A8:
            return	GL_UNSIGNED_BYTE;
        case PIXEL_YUYV:
        case PIXEL_UYVY:
        case PIXEL_YUY2:
            return	GL_UNSIGNED_BYTE;
        default:
            return	GL_UNSIGNED_BYTE;
        }
    }


    GLenum      getInternalFormat(const char* fmt)
    {
        if (strcmp(fmt,"PIXEL_R8G8B8A8") == 0)
        {
            return  GL_RGBA;
        }
        else if(strcmp(fmt,"PIXEL_R8G8B8") == 0)
        {
            return  GL_RGB;
        }
        else if(strcmp(fmt,"PIXEL_RGBA4") == 0)
        {
            return  GL_RGBA4;
        }
        else if(strcmp(fmt,"PIXEL_A8") == 0)
        {
            return  GL_ALPHA;
        }
        else if(strcmp(fmt,"PIXEL_I8") == 0)
        {
            return  GL_LUMINANCE;
        }
        else if(strcmp(fmt,"PIXEL_IA8") == 0)
        {
            return  GL_LUMINANCE_ALPHA;
        }
        else
        {
            return  GL_RGB;
        }
    }

    GLenum      getInternalFormat(PIXEL_FORMAT fmt)
    {
        switch(fmt)
        {
        case PIXEL_A8:
            return  GL_ALPHA;
        case PIXEL_I8:
            return  GL_LUMINANCE;
        case PIXEL_IA8:
            return  GL_LUMINANCE_ALPHA;
        case PIXEL_R8G8B8:
            return	GL_RGB;
        case PIXEL_R8G8B8A8:
            return	GL_RGBA;
        case PIXEL_B8G8R8:
            return	GL_RGB;
        case PIXEL_B8G8R8A8:
            return	GL_RGBA;
        case PIXEL_YUYV:
        case PIXEL_UYVY:
        case PIXEL_YUY2:
            return	GL_LUMINANCE_ALPHA;
        default:
            return	GL_LUMINANCE_ALPHA;
        }
    }


    CELLOpenGL::CELLOpenGL()
    {
        /**
        *    初始化字体库
        */
        _library        =   0;
        _face           =   0;
        _fontBuffer     =   0;
        _fontSize       =   0;
        _fontPixelX     =   0;
        _fontPixelY     =   0;
        _xStart         =   0;
        _yStart         =   0;
        _textureWidth   =   512;
        _textureHeight  =   512;
        _sysFontTexture =   unsigned(-1);

        memset(_character,0,sizeof(_character));
        FT_Init_FreeType( (FT_Library*)&_library );
        assert(_library != 0 );
    }
    CELLOpenGL::~CELLOpenGL()
    {

        /**
        *   释放字体库
        */
        destroy();

        /**
        *   释放字体库
        */
        if (_library)
        {
            FT_Done_FreeType((FT_Library)_library);
            _library    =   0;
        }
    }


    void CELLOpenGL::initialize()
    {
        destroy();
    	glEnable(GL_TEXTURE_2D);
        _uiProgram.initialize(this);
    }


    void CELLOpenGL::destroy()
    {
        if (_sysFontTexture != -1)
        {
            glDeleteTextures(1,&_sysFontTexture);
            _sysFontTexture =   unsigned(-1);
        }
        if (_face)
        {
            FT_Done_Face(FT_Face(_face));
            _xStart     =   0;
            _yStart     =   0;
            _face       =   0;
            memset(_character,0,sizeof(_character));
        }
        if (_fontBuffer)
        {
            delete []_fontBuffer;
            _fontBuffer =   0;
        }
        _uiProgram.destroy();
    }


    /**
    *   建立字体系统
    */
    void    CELLOpenGL::buildSystemFont(const char* font,int fontSize)
    {

        unsigned    length  =   0;
        _fontBuffer  =   readFile(font,length);

        /**
        *   保存字体的大小
        */
        _fontSize   =   fontSize;
        /**
        *   已经创建了字体则销毁
        *   支持多次调用
        */
        if (_face)
        {
            FT_Done_Face(FT_Face(_face));
            _xStart         =   0;
            _yStart         =   0;
            memset(_character,0,sizeof(_character));
        }
        /**
        *   销毁字体
        */
        if (_sysFontTexture != -1)
        {
            glDeleteTextures(1,&_sysFontTexture);
        }

        glGenTextures(1,&_sysFontTexture);
        /**
        *   使用这个纹理id,或者叫绑定(关联)
        */
        glBindTexture( GL_TEXTURE_2D, _sysFontTexture );
        /**
        *   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式 
        */
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexImage2D( 
            GL_TEXTURE_2D,      //! 指定是二维图片
            0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
            GL_ALPHA,           //! 纹理的使用的存储格式
            _textureWidth,
            _textureHeight,
            0,                  //! 是否的边
            GL_ALPHA,           //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
            GL_UNSIGNED_BYTE,   //! 数据是8bit数据
            0
            );
        FT_New_Memory_Face((FT_Library)_library,(const FT_Byte *)_fontBuffer,length,0,(FT_Face*)&_face);

        FT_F26Dot6 ftSize = (FT_F26Dot6)(fontSize * (1 << 6));

        FT_Set_Char_Size((FT_Face)_face, ftSize, 0, 72, 72);

        assert(_face != 0 );

    }

    
    Character CELLOpenGL::getCharacter( wchar_t ch )
    {

        if (_character[ch].x0 == 0 &&
            _character[ch].x0 == 0 &&
            _character[ch].x1 == 0 &&
            _character[ch].y1 == 0
            )
        {

            glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

            if (_xStart + std::max(_fontPixelX,_fontSize) > _textureWidth)
            {
                /**
                *   写满一行,从新开始
                */
                _xStart =   0;
                /**
                *   y开始位置要增加
                */
                _yStart +=  std::max(_fontPixelY,_fontSize);
            }
            FT_Load_Glyph((FT_Face) _face, FT_Get_Char_Index( (FT_Face)_face, ch ), FT_LOAD_DEFAULT );
            FT_Glyph glyph;
            FT_Get_Glyph( FT_Face(_face)->glyph, &glyph );

            /**
            *   根据字体的大小决定是否使用反锯齿绘制模式
            *   当字体比较小的是说建议使用ft_render_mode_mono
            *   当字体比较大的情况下12以上，建议使用ft_render_mode_normal模式
            */
            if (_fontSize >= 16)
            {
                FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
            }
            else
            {
                FT_Glyph_To_Bitmap( &glyph, ft_render_mode_mono, 0, 1 );
            }
            
            FT_BitmapGlyph  bitmap_glyph    =   (FT_BitmapGlyph)glyph;
            FT_Bitmap&      bitmap          =   bitmap_glyph->bitmap;
            FT_Bitmap       ftBitmap;
            FT_Bitmap_New(&ftBitmap);

            if(bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            {
                if (FT_Bitmap_Convert((FT_Library)_library, &bitmap, &ftBitmap, 1) == 0)
                {
                    /**
                    *   Go through the bitmap and convert all of the nonzero values to 0xFF (white).
                    */
                    for (unsigned char* p = ftBitmap.buffer, * endP = p + ftBitmap.width * ftBitmap.rows; p != endP; ++p)
                        *p ^= -*p ^ *p;
                    bitmap  =   ftBitmap;
                }
            }
            /**
            *   如果没有数据，则不写，直接过去
            */
            if (bitmap.width == 0 || bitmap.rows == 0)
            {
                
                char    mem[1024 * 32];
                memset(mem,0,sizeof(mem));

                _character[ch].x0       =   _xStart;
                _character[ch].y0       =   _yStart;
                _character[ch].x1       =   _xStart + _fontSize/2;
                _character[ch].y1       =   _yStart + _fontSize - 1;
                _character[ch].offsetY  =   _fontSize - 1;
                _character[ch].offsetX  =   0;

                

                glBindTexture(GL_TEXTURE_2D,_sysFontTexture);

                glTexSubImage2D (
                    GL_TEXTURE_2D,
                    0,
                    _xStart,
                    _yStart,
                    _fontSize/2,
                    _fontSize,
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    mem
                    );
                _xStart +=  _fontSize/2;


            }
            else
            {
                glBindTexture(GL_TEXTURE_2D,_sysFontTexture);

                _character[ch].x0       =   _xStart;
                _character[ch].y0       =   _yStart;
                _character[ch].x1       =   _xStart + bitmap.width;
                _character[ch].y1       =   _yStart + bitmap.rows;

                _character[ch].offsetY  =   bitmap_glyph->top;
                _character[ch].offsetX  =   bitmap_glyph->left;

                glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
                glTexSubImage2D (
                    GL_TEXTURE_2D,
                    0,
                    _xStart,
                    _yStart,
                    std::max(1,bitmap.width),
                    std::max(1,bitmap.rows),
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    bitmap.buffer
                    );
                _xStart     +=  (bitmap.width + 1);
                _fontPixelY =   std::max(_fontPixelY,bitmap.rows);
                _fontPixelX =   std::max(_fontPixelX,bitmap.width);
            }

            FT_Bitmap_Done((FT_Library)_library,&ftBitmap);
        }
        return  _character[ch];
    }


    void CELLOpenGL::beginText( const matrix4r& proj )
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture( GL_TEXTURE_2D, _sysFontTexture );

        _uiProgram.begin();
        /**
        *   绑定程序数据
        */
        setUniformMatrix4fv(_uiProgram.getMVPUniform(),1,false,proj.data());
        setUniform1i(_uiProgram.getTexture1Uniform(),0);
    }

    void CELLOpenGL::endText()
    {
        _uiProgram.end();
    }

    float2 CELLOpenGL::drawText( 
        float x,
        float y, 
        float z, 
        Rgba4Byte color,
        const wchar_t* text, 
        size_t length /*= -1 */ ,
        CELLUIVertex**  vertexs/* = 0*/,
        size_t* vertexLength/* = 0*/)
    {
        static      CELLUIVertex  vert[1024];

        float       texWidth    =   (float)_textureWidth;
        float       texHeight   =   (float)_textureHeight;
        float       xStart      =   (float)(int)x;
        float       yStart      =   (float)(int)y + _fontSize;
        float       zStart      =   z;
        unsigned    index       =   0;
        unsigned    size        =   length == -1 ? wcslen(text) :length;
        float2      vSize(0,0);


        if (size == 0)
        {
            return  vSize;
        }


        

        for (unsigned i = 0 ;i <  size; ++ i )
        {
            Character   ch  =   getCharacter(text[i]);

            float       h           =   float(ch.y1 - ch.y0);
            float       w           =   float(ch.x1 - ch.x0);
            float       offsetY     =   (float(h) - float(ch.offsetY));
            /**
            *   第一个点
            */
            vert[index + 0].x       =   xStart;
            vert[index + 0].y       =   yStart - h + offsetY;
            vert[index + 0].z       =   zStart;
            vert[index + 0].u       =   ch.x0/texWidth;
            vert[index + 0].v       =   ch.y0/texHeight;
            vert[index + 0].w       =   1;
            vert[index + 0].color   =   color;
            /**
            *   第二个点
            */
            vert[index + 1].x       =   xStart + w;
            vert[index + 1].y       =   yStart - h + offsetY;
            vert[index + 1].z       =   zStart;
            vert[index + 1].u       =   ch.x1/texWidth;
            vert[index + 1].v       =   ch.y0/texHeight;
            vert[index + 1].w       =   1;
            vert[index + 1].color   =   color;
            /**
            *   第三个点
            */
            vert[index + 2].x       =   xStart + w;
            vert[index + 2].y       =   yStart  + offsetY;
            vert[index + 2].z       =   zStart;
            vert[index + 2].u       =   ch.x1/texWidth;
            vert[index + 2].v       =   ch.y1/texHeight;
            vert[index + 2].w       =   1;
            vert[index + 2].color   =   color;



            /**
            *   第一个点
            */
            vert[index + 3].x       =   xStart;
            vert[index + 3].y       =   yStart - h + offsetY;
            vert[index + 3].z       =   zStart;
            vert[index + 3].u       =   ch.x0/texWidth;
            vert[index + 3].v       =   ch.y0/texHeight;
            vert[index + 3].w       =   1;
            vert[index + 3].color   =   color;

            /**
            *   第三个点
            */
            vert[index + 4].x       =   xStart + w;
            vert[index + 4].y       =   yStart  + offsetY;
            vert[index + 4].z       =   zStart;
            vert[index + 4].u       =   ch.x1/texWidth;
            vert[index + 4].v       =   ch.y1/texHeight;
            vert[index + 4].w       =   1;
            vert[index + 4].color   =   color;


            /**
            *   第四个点
            */
            vert[index + 5].x       =   xStart;
            vert[index + 5].y       =   yStart  + offsetY;
            vert[index + 5].z       =   zStart;
            vert[index + 5].u       =   ch.x0/texWidth;
            vert[index + 5].v       =   ch.y1/texHeight;
            vert[index + 5].w       =   1;
            vert[index + 5].color   =   color;

            index   +=  6;
            xStart  +=  w + (ch.offsetX + 1);

            vSize.x +=  w + (ch.offsetX + 1);
            vSize.y =   std::max(h + offsetY,vSize.y);

        }
        attributePointer(_uiProgram.getPositionAttribute(),   3,  GL_FLOAT,           GL_FALSE,   sizeof(CELLUIVertex),   vert);
        attributePointer(_uiProgram.getUV1Attribute(),        3,  GL_FLOAT,           GL_FALSE,   sizeof(CELLUIVertex),   &vert[0].u);
        attributePointer(_uiProgram.getColorAttribute(),      4,  GL_UNSIGNED_BYTE,   GL_TRUE,    sizeof(CELLUIVertex),   &vert[0].color);
        glDrawArrays(GL_TRIANGLES,0,index);
        /**
        *   向外输出顶点数据.
        */
        if (vertexs)
        {
            *vertexs    =   vert;
        }
        if (vertexLength)
        {
            *vertexLength   =   index;
        }
        return  vSize;
    }


    float2 CELLOpenGL::drawTextToBuffer( 
        float x,
        float y, 
        float z, 
        Rgba4Byte color,
        const wchar_t* text, 
        size_t length /*= -1 */ ,
        CELLUIVertex**  vertexs/* = 0*/,
        size_t* vertexLength/* = 0*/)
    {

		*vertexs        =   0;
		*vertexLength   =   0;

        static      CELLUIVertex  vert[1024];

        float       texWidth    =   (float)_textureWidth;
        float       texHeight   =   (float)_textureHeight;
        float       xStart      =   (float)int(x);
        float       yStart      =   (float)int(y + _fontSize);
        float       zStart      =   z;
        unsigned    index       =   0;
        unsigned    size        =   length == -1 ? wcslen(text) :length;
        float2      vSize(0,0);


        if (size == 0)
        {
            return  vSize;
        }

        for (unsigned i = 0 ;i <  size; ++ i )
        {
            Character   ch  =   getCharacter(text[i]);

            float       h           =   float(ch.y1 - ch.y0);
            float       w           =   float(ch.x1 - ch.x0);
            float       offsetY     =   (float(h) - float(ch.offsetY));
            /**
            *   第一个点
            */
            vert[index + 0].x       =   xStart;
            vert[index + 0].y       =   yStart - h + offsetY;
            vert[index + 0].z       =   zStart;
            vert[index + 0].u       =   ch.x0/texWidth;
            vert[index + 0].v       =   ch.y0/texHeight;
            vert[index + 0].w       =   1;
            vert[index + 0].color   =   color;
            /**
            *   第二个点
            */
            vert[index + 1].x       =   xStart + w;
            vert[index + 1].y       =   yStart - h + offsetY;
            vert[index + 1].z       =   zStart;
            vert[index + 1].u       =   ch.x1/texWidth;
            vert[index + 1].v       =   ch.y0/texHeight;
            vert[index + 1].w       =   1;
            vert[index + 1].color   =   color;
            /**
            *   第三个点
            */
            vert[index + 2].x       =   xStart + w;
            vert[index + 2].y       =   yStart  + offsetY;
            vert[index + 2].z       =   zStart;
            vert[index + 2].u       =   ch.x1/texWidth;
            vert[index + 2].v       =   ch.y1/texHeight;
            vert[index + 2].w       =   1;
            vert[index + 2].color   =   color;

            /**
            *   第一个点
            */
            vert[index + 3].x       =   xStart;
            vert[index + 3].y       =   yStart - h + offsetY;
            vert[index + 3].z       =   zStart;
            vert[index + 3].u       =   ch.x0/texWidth;
            vert[index + 3].v       =   ch.y0/texHeight;
            vert[index + 3].w       =   1;
            vert[index + 3].color   =   color;

            /**
            *   第三个点
            */
            vert[index + 4].x       =   xStart + w;
            vert[index + 4].y       =   yStart  + offsetY;
            vert[index + 4].z       =   zStart;
            vert[index + 4].u       =   ch.x1/texWidth;
            vert[index + 4].v       =   ch.y1/texHeight;
            vert[index + 4].w       =   1;
            vert[index + 4].color   =   color;
            /**
            *   第四个点
            */
            vert[index + 5].x       =   xStart;
            vert[index + 5].y       =   yStart  + offsetY;
            vert[index + 5].z       =   zStart;
            vert[index + 5].u       =   ch.x0/texWidth;
            vert[index + 5].v       =   ch.y1/texHeight;
            vert[index + 5].w       =   1;
            vert[index + 5].color   =   color;

            index   +=  6;
            xStart  +=  w + (ch.offsetX + 1);

            vSize.x +=  w + (ch.offsetX + 1);

            vSize.y =   std::max(h + offsetY,vSize.y);
        }
    
        /**
        *   向外输出顶点数据.
        */
        if (vertexs)
        {
            *vertexs    =   vert;
        }
        if (vertexLength)
        {
            *vertexLength   =   index;
        }
        return  vSize;
    }
    void CELLOpenGL::clear(unsigned mask)
    {
        //! GL_DEPTH_BUFFER_BIT
        //! GL_COLOR_BUFFER_BIT
        //! GL_COLOR_BUFFER_BIT
        //! GL_STENCIL_BUFFER_BIT
        //! GL_ACCUM_BUFFER_BIT;
        //! GL_TRANSFORM_BIT
        //! GL_ENABLE_BIT
        //! GL_HINT_BIT
        //! GL_EVAL_BIT
        glClear(mask);
    }

    void CELLOpenGL::clearColor(float r,float g,float b,float a)
    {
        glClearColor(r,g,b,a);
    }


    void CELLOpenGL::setLineWidth( float width )
    {
        glLineWidth(width);
    }


    Texture2dId CELLOpenGL::createTexure2D(
                                            int level,
                                            int intFmt,
                                            int width,
                                            int height,
                                            int border,
                                            int srcFmt,
                                            int type,
                                            const void* data,
                                            void* userData
                                            )
    {
        Texture2dId textureId;
        textureId._width    =   width;
        textureId._height   =   height;
        textureId._user     =   userData;
        glGenTextures(1,&textureId._texture);
        glBindTexture( GL_TEXTURE_2D, textureId._texture );
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        
        glTexImage2D(
            GL_TEXTURE_2D, 
            level, 
            intFmt,
            width, 
            height,
            border, 
            srcFmt , 
            type,
            data
            );
        return  textureId;
    }

    CELL::Texture2dId CELLOpenGL::createTexture2DFromStream( const char* stream,unsigned length )
    {
        Texture2dId texId;
        CELLImageReader reader((char*)stream,(int)length);
        if (!reader.parse())
        {
            return  texId;
        }

        glGenTextures(1, &texId._texture);
        glBindTexture(GL_TEXTURE_2D, texId._texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        ImageHeader*header  =   (ImageHeader*)reader.getCursor();
        texId._width        =   (unsigned)header->getWidth();
        texId._height       =   (unsigned)header->getHeight();


        GLenum  intFormat   =   getInternalFormat(reader.getFormat());
        GLenum  srcType     =   getSrcType(reader.getFormat());
        GLint   srcFmt      =   getSrcFormat(reader.getFormat());
        GLint   level       =   0;
        while(!reader.eof())
        {
            header  =   (ImageHeader*)reader.getCursor();
            reader.skip(sizeof(ImageHeader));
            char*           pixel   =   reader.getCursor();

            glTexImage2D(
                GL_TEXTURE_2D,
                level, 
                intFormat, 
                header->getWidth(),
                header->getHeight(), 
                0,
                srcFmt,
                srcType,
                pixel
                );
            ++level;
            reader.skip(header->getLength());
        }
        return  texId;
    }


    CELL::Texture2dId CELLOpenGL::createTexture2DFromFile( const char* fileName )
    {
        Texture2dId texId;
        unsigned    length  =   0;
        char*       buffer  =   readFile(fileName,length);
        if (buffer)
        {
            texId   =   createTexture2DFromStream(buffer,length);
            delete [] buffer;
        }
        return  texId;
    }


    void CELLOpenGL::textureImage2D( Texture2dId& texId,int level, int fmt, int width, int height, int border, unsigned format, unsigned type, const void *data )
    {
        glBindTexture(GL_TEXTURE_2D,texId._texture);

        texId._width    =   width;
        texId._height   =   height;
        glTexImage2D(
            GL_TEXTURE_2D, 
            level, 
            fmt,
            width, 
            height,
            border, 
            format , 
            type,
            data
            );
    }

    void CELLOpenGL::bindTexture2D(  const Texture2dId* textureId,unsigned stage)
    {
        static  unsigned sTexture   =   unsigned(-1);
        static  unsigned sStage     =   unsigned(-1);
        
        if (textureId)
        {
            if (sTexture == textureId->_texture && sStage == stage)
            {
                return;
            }
            else
            {
                sTexture    =   textureId->_texture;
                sStage      =   stage;

            }
            glActiveTexture(GL_TEXTURE0 + stage);
            glBindTexture(GL_TEXTURE_2D,textureId->_texture);
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + stage);
            glBindTexture(GL_TEXTURE_2D,0);
        }
    }

    void CELLOpenGL::copyTexSubImage2D( 
                                        int level, 
                                        int xoffset,
                                        int yoffset, 
                                        int x, 
                                        int y, 
                                        int width, 
                                        int height 
                                        )
    {
        glCopyTexSubImage2D(
                            GL_TEXTURE_2D, 
                            level, 
                            xoffset, 
                            yoffset, 
                            x, 
                            y, 
                            width,
                            height
                            );
    }


    void CELLOpenGL::updateTexture2D(
                                    int level,
                                    int xoffset,
                                    int yoffset,
                                    int width,
                                    int height,
                                    int srcFormat,
                                    int srcType,
                                    const void* data
                                    )
    {
        glTexSubImage2D(
                        GL_TEXTURE_2D, 
                        level,          //! lod级别
                        xoffset,        //! x偏移量
                        yoffset,        //! y偏移量
                        width, 
                        height, 
                        srcFormat,      
                        //! GL_ALPHA
                        //! GL_LUMINANCE,
                        //! GL_LUMINANCE_ALPHA,
                        //! GL_RGB,GL_RGBA,GL_BGR,GL_BGRA,
                        //! GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
                        //! GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
                        //! GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
                        srcType,    
                        //! GL_UNSIGNED_BYTE
                        //! GL_SHORT
                        //! GL_UNSIGNED_SHORT
                        data
                        );
    }

    void CELLOpenGL::getTexImage( Texture2dId& /*id*/,unsigned /*fmt*/, unsigned /*type*/,void* /*buffer*/)
    {
#ifdef WIN32
        //bindTexture2D(&id,0);
        //glGetTexImage(GL_TEXTURE_2D,0,fmt,type,buffer);
#else
        LOG_E("getTexImage");
        assert("glGetTexImage == 0" && 0 != 0);
#endif
    }

    void CELLOpenGL::destroyTexture2D( Texture2dId& id )
    {
        if (id._texture != -1)
        {
            glDeleteTextures(1,&id._texture);
        }
    }

    VertexBufferId CELLOpenGL::createVertexBuffer( unsigned size,unsigned acc,const void* data )
    {
        VertexBufferId  bufferId;
        bufferId._buffserSize   =   size;
        bufferId._accessType    =   acc;
        glGenBuffers(1, &bufferId._bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, bufferId._bufferId);
        switch(acc)
        {
        case GL_STREAM_DRAW:
        case GL_STATIC_DRAW: 
        case GL_DYNAMIC_DRAW:
            glBufferData(GL_ARRAY_BUFFER, size, data, acc);
            break;
        default:
            assert("createVertexBuffer error access type" && 0);
            break;
        }
        return  bufferId;
    }

    void CELLOpenGL::updateVertexBuffer(unsigned offset,unsigned size,const void* data )
    {
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    void    CELLOpenGL::getVertexBuffer(int /*offset*/, int /*size*/, void* /*data*/)
    {
#ifdef WIN32
        //glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
#endif
    }


    void CELLOpenGL::bindVertexBuffer( const VertexBufferId* id )
    {
        if (id)
        {
            glBindBuffer(GL_ARRAY_BUFFER, id->_bufferId);
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    void CELLOpenGL::destroyVertexBuffer( const VertexBufferId& id )
    {
        if (id._bufferId)
        {
            glDeleteBuffers(1,&id._bufferId);
        }
    }

    IndexBufferId CELLOpenGL::createIndexBuffer( unsigned size,unsigned acc,const void* data )
    {
        IndexBufferId  bufferId;
        bufferId._buffserSize   =   size;
        bufferId._accessType    =   acc;
        glGenBuffers(1, &bufferId._bufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId._bufferId);
        switch(acc)
        {
        case GL_STREAM_DRAW:
        case GL_STATIC_DRAW: 
        case GL_DYNAMIC_DRAW:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, acc);
            break;
        default:
            assert("createIndexBuffer error access type" && 0);
                break;
        }
        return  bufferId;
    }

    void CELLOpenGL::updateIndexBuffer(unsigned offset,unsigned size,const void* data )
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    }


    void CELLOpenGL::bindIndexBuffer( const IndexBufferId* id )
    {
        if (id)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id->_bufferId);
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    void CELLOpenGL::destroyIndexBuffer( const IndexBufferId& id )
    {
        if (id._bufferId)
        {
            glDeleteBuffers(1,&id._bufferId);
        }
    }


    CELL::ProgramId CELLOpenGL::createProgram( const char* vertex,const char* fragment )
    {
        ProgramId   program;
        bool        error   =   false;
        do 
        {
            if (vertex)
            {
                program._vertex._shaderId   = glCreateShader( GL_VERTEX_SHADER );
                glShaderSource( program._vertex._shaderId, 1, &vertex, 0 );
                glCompileShader( program._vertex._shaderId );

                GLint   compileStatus;
                glGetShaderiv( program._vertex._shaderId, GL_COMPILE_STATUS, &compileStatus );
                error   =   compileStatus == GL_FALSE;
                if( error )
                {
                    GLchar messages[256];
                    glGetShaderInfoLog( program._vertex._shaderId, sizeof(messages), 0,messages);
                    assert( messages && 0 != 0);
                    break;
                }
            }
            if (fragment)
            {
                program._fragment._shaderId   = glCreateShader( GL_FRAGMENT_SHADER );
                glShaderSource( program._fragment._shaderId, 1, &fragment, 0 );
                glCompileShader( program._fragment._shaderId );

                GLint   compileStatus;
                glGetShaderiv( program._fragment._shaderId, GL_COMPILE_STATUS, &compileStatus );
                error   =   compileStatus == GL_FALSE;

                if( error )
                {
                    GLchar messages[256];
                    glGetShaderInfoLog( program._fragment._shaderId, sizeof(messages), 0,messages);
                    assert( messages && 0 != 0);
                    break;
                }
            }

            program._programId  =   glCreateProgram( );

            if (program._vertex._shaderId)
            {
                glAttachShader( program._programId, program._vertex._shaderId);
            }
            if (program._fragment._shaderId)
            {
                glAttachShader( program._programId, program._fragment._shaderId);
            }
            
            glLinkProgram( program._programId );

            GLint linkStatus;
            glGetProgramiv( program._programId, GL_LINK_STATUS, &linkStatus );
            if (linkStatus == GL_FALSE)
            {
                GLchar messages[256];
                glGetProgramInfoLog( program._programId, sizeof(messages), 0, messages);
                break;
            }
            glUseProgram(program._programId);

        } while(false);

        if (error)
        {
            if (program._fragment._shaderId)
            {
                glDeleteShader(program._fragment._shaderId);
                program._fragment._shaderId =   0;
            }
            if (program._vertex._shaderId)
            {
                glDeleteShader(program._vertex._shaderId);
                program._vertex._shaderId   =   0;
            }
            if (program._programId)
            {
                glDeleteProgram(program._programId);
                program._programId          =   0;
            }
        }
        return  program;
    }

    CELL::ProgramId CELLOpenGL::createProgramFromFile( const char* vertexFile,const char* fragmentFile )
    {
        unsigned    length          =   0;
        char*       verexSource     =   0;
        char*       fragmentSource  =   0;
        if (vertexFile)
        {
            verexSource     =   readFile(vertexFile,length) ;
        }

        if (fragmentFile)
        {
             fragmentSource =   readFile(fragmentFile,length) ;
        }
        ProgramId   program =   createProgram(verexSource,fragmentSource);
        if (verexSource)
        {
            delete  []verexSource;
        }
        if (fragmentSource)
        {
            delete  []fragmentSource;
        }
        return  program;
    }

    void CELLOpenGL::destroyProgram( ProgramId& program )
    {
        glUseProgram(0);

        if (program._vertex._shaderId)
        {
            glDetachShader(program._programId,program._vertex._shaderId);
            glDeleteShader(program._vertex._shaderId);
            program._vertex._shaderId =   0;
        }
        if (program._fragment._shaderId)
        {
            glDetachShader(program._programId,program._fragment._shaderId);
            glDeleteShader(program._vertex._shaderId);
            glDeleteShader(program._fragment._shaderId);
            program._fragment._shaderId =   0;
        }
        if (program._programId)
        {
            glDeleteProgram(program._programId);
            program._programId  =   0;
        }
    }

    void CELLOpenGL::useProgram( const ProgramId* program )
    {
        if (program)
        {
            glUseProgram(program->_programId);
        }
        else
        {
            glUseProgram(0);
        }
    }

    int CELLOpenGL::getUniformLocation( const ProgramId& program,const char *name )
    {
        return  glGetUniformLocation(program._programId, name);
    }

    int CELLOpenGL::getAttribLocation( const ProgramId& program,const char* name )
    {
        return  glGetAttribLocation(program._programId,name);
    }


    void CELLOpenGL::setUniform1f( int index,float v0 )
    {
        glUniform1f(index, v0);
    }

    void CELLOpenGL::setUniform2f( int index,float v0, float v1 )
    {
        glUniform2f(index, v0,v1);
    }

    void CELLOpenGL::setUniform3f( int index,float v0, float v1, float v2 )
    {
        glUniform3f(index, v0,v1,v2);
    }

    void CELLOpenGL::setUniform4f( int index,float v0, float v1, float v2, float v3 )
    {
        glUniform4f(index, v0,v1,v2,v3);
    }

    void CELLOpenGL::setUniform1i( int index,int v0 )
    {
        glUniform1i(index, v0);
    }

    void CELLOpenGL::setUniform2i( int index,int v0, int v1 )
    {
        glUniform2i(index, v0,v1);
    }

    void CELLOpenGL::setUniform3i( int index,int v0, int v1, int v2 )
    {
        glUniform3i(index, v0,v1,v2);
    }

    void CELLOpenGL::setUniform4i( int index,int v0, int v1, int v2, int v3 )
    {
        glUniform4i(index, v0,v1,v2,v3);
    }

    void CELLOpenGL::setUniform1fv( int index,int count, const float *value )
    {
        glUniform1fv(index, count, value);
    }

    void CELLOpenGL::setUniform2fv( int index,int count, const float *value )
    {
        glUniform2fv(index, count, value);
    }

    void CELLOpenGL::setUniform3fv( int index,int count, const float *value )
    {
        glUniform3fv(index, count, value);
    }

    void CELLOpenGL::setUniform4fv( int index,int count, const float *value )
    {
        glUniform4fv(index, count, value);
    }

    void CELLOpenGL::setUniform1iv( int index,int count, const int *value )
    {
        glUniform1iv(index, count, value);
    }

    void CELLOpenGL::setUniform2iv( int index,int count, const int *value )
    {
        glUniform2iv(index, count, value);
    }

    void CELLOpenGL::setUniform3iv( int index,int count, const int *value )
    {
        glUniform3iv(index, count, value);
    }

    void CELLOpenGL::setUniform4iv( int index,int count, const int *value )
    {
        glUniform4iv(index, count, value);
    }

    void CELLOpenGL::setUniformMatrix2fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix2fv(index, count, (GLboolean) transpose, value);
    }

    void CELLOpenGL::setUniformMatrix3fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix3fv(index, count, (GLboolean) transpose, value);
    }

    void CELLOpenGL::setUniformMatrix4fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix4fv(index, count, (GLboolean) transpose, value);
    }

    void CELLOpenGL::bindAttribLocation( const ProgramId& prgId,int index, const char* name )
    {
        glBindAttribLocation(prgId._programId, index, name);
    }

    void CELLOpenGL::setVertexAttrib1f( unsigned index, float v0 )
    {
        glVertexAttrib1f(index, v0);
    }

    void CELLOpenGL::setVertexAttrib2f( unsigned index, float v0, float v1 )
    {
        glVertexAttrib2f(index, v0,v1);
    }

    void CELLOpenGL::setVertexAttrib3f( unsigned index, float v0, float v1, float v2 )
    {
        glVertexAttrib3f(index, v0,v1,v2);
    }

    void CELLOpenGL::setVertexAttrib4f( unsigned index, float v0, float v1, float v2, float v3 )
    {
        glVertexAttrib4f(index, v0,v1,v2,v3);
    }



    void CELLOpenGL::setViewPort( int x,int y,int width,int height )
    {
        glViewport(GLint(x),GLint(y),GLsizei(width),GLsizei(height));
    }



    void CELLOpenGL::enableRenderState( unsigned state )
    {
        switch(state)
        {
        case GL_TEXTURE1:
        case GL_TEXTURE2:
        case GL_TEXTURE3:
        case GL_TEXTURE4:
        case GL_TEXTURE5:
        case GL_TEXTURE6:
        case GL_TEXTURE7:
        	glActiveTexture(state);
            break;
        case GL_TEXTURE_2D:
            glEnable(GL_TEXTURE_2D);
            break;
        case GL_DEPTH_TEST:
            glEnable(GL_DEPTH_TEST);
            break;
        case GL_STENCIL_TEST:
            glEnable(GL_STENCIL_TEST);
            break;
        case GL_SCISSOR_TEST:
            glEnable(GL_SCISSOR_TEST);
            break;
        case GL_CULL_FACE:
            glEnable(GL_CULL_FACE);
            break;
        case GL_BLEND:
            glEnable(GL_BLEND);
            break;
        default:
            glEnable(state);
            break;
            
        }
    }

    void CELLOpenGL::disableRenderState( unsigned state )
    {
        switch(state)
        {

        case GL_TEXTURE1:
        case GL_TEXTURE2:
        case GL_TEXTURE3:
        case GL_TEXTURE4:
        case GL_TEXTURE5:
        case GL_TEXTURE6:
        case GL_TEXTURE7:
            //glClientActiveTexture(GL_TEXTURE1);
        case GL_TEXTURE_2D:
            glDisable(GL_TEXTURE_2D);
            break;
        case GL_DEPTH_TEST:
            glDisable(GL_DEPTH_TEST);
            break;
        case GL_STENCIL_TEST:
            glDisable(GL_STENCIL_TEST);
            break;;
            break;
        case GL_SCISSOR_TEST:
            glDisable(GL_SCISSOR_TEST);
            break;
        case GL_CULL_FACE:
            glDisable(GL_CULL_FACE);
            break;
        case GL_BLEND:
            glDisable(GL_BLEND);
            break;
        default:
            glDisable(state);
            break;
        }
    }

    void CELLOpenGL::scissor( int x,int y,int width,int height )
    {
        glScissor(x,y,width,height);
    }

    void CELLOpenGL::blendFunction( unsigned sfactor, unsigned dfactor )
    {
        glBlendFunc(sfactor,dfactor);
    }

    void CELLOpenGL::enableVertexAttributeArray( unsigned attrIndex )
    {
        glEnableVertexAttribArray(attrIndex);
    }

    void CELLOpenGL::disableVertexAttributeArray( unsigned attrIndex )
    {
        glDisableVertexAttribArray(attrIndex);
    }


  

    void CELLOpenGL::attributePointer( int index, int size, int type, int normalized, int stride, const void* pointer )
    {
        glVertexAttribPointer(index,size,type,(GLboolean)normalized,stride,pointer);
    }


    void CELLOpenGL::drawArray( unsigned mode,int first,int count )
    {
        glDrawArrays(mode,first,count);
    }


    void CELLOpenGL::drawElements( unsigned mode, int count,int type,const void* data )
    {
        glDrawElements(mode,count,type,data);
    }


    void CELLOpenGL::cullFace( unsigned face )
    {
        switch(face)
        {
        case GL_FRONT:
            glCullFace(GL_FRONT);
            break;
        case GL_BACK:
            glCullFace(GL_BACK);
            break;
        case GL_FRONT_AND_BACK:
            glCullFace(GL_FRONT_AND_BACK);
            break;
        }
    }

    FrameBufferId CELLOpenGL::createFrameBuffer( int width,int height)
    {
        FrameBufferId    targetId;
        targetId._width  =   std::min(16,width);
        targetId._height =   std::min(16,height);

        glGenFramebuffers(1, &targetId._FBOID);
        glBindFramebuffer(GL_FRAMEBUFFER, targetId._FBOID);

        glGenRenderbuffers(1, &targetId._RBOID);
        glBindRenderbuffer(GL_RENDERBUFFER, targetId._RBOID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, targetId._RBOID);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return  targetId;
    }


    void CELLOpenGL::destroyFrameBuffer( FrameBufferId& frameBuffer )
    {
        
        glDeleteFramebuffers(1, &frameBuffer._FBOID);
        glDeleteRenderbuffers(1,&frameBuffer._RBOID);
        frameBuffer._RBOID  =   0;
        frameBuffer._FBOID  =   0;
    }


    void CELLOpenGL::beginFrameBuffer( const FrameBufferId& targetId,const Texture2dId& textureId )
    {
        glBindFramebuffer(GL_FRAMEBUFFER, targetId._FBOID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId._texture, 0);
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, targetId._RBOID );
    }

    void CELLOpenGL::bindFrameBuffer( const FrameBufferId*targetId )
    {
        if (targetId)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, targetId->_FBOID);
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void CELLOpenGL::endFrameBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void CELLOpenGL::resizeFrameBuffer( FrameBufferId bufferId,int width,int height )
    {
        glBindRenderbuffer(GL_RENDERBUFFER, bufferId._RBOID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    }

    Program::Program() :_device(0)
    {

    }

    Program::~Program()
    {
        destroy();
    }


    void Program::destroy()
    {
        if (_device)
        {
            _device->destroyProgram(*this);
        }
    }


    void Program::initialize( CELLOpenGL*)
    {

    }

    void Program::begin()
    {
    }

    void Program::end()
    {
    }


    int Program::getUniformLocation( const ProgramId& program,const char *name )
    {
        return  glGetUniformLocation(program._programId, name);
    }

    int Program::getAttribLocation( const ProgramId& program,const char* name )
    {
        return  glGetAttribLocation(program._programId,name);
    }


    void Program::setUniform1f( int index,float v0 )
    {
        glUniform1f(index, v0);
    }

    void Program::setUniform2f( int index,float v0, float v1 )
    {
        glUniform2f(index, v0,v1);
    }

    void Program::setUniform3f( int index,float v0, float v1, float v2 )
    {
        glUniform3f(index, v0,v1,v2);
    }

    void Program::setUniform4f( int index,float v0, float v1, float v2, float v3 )
    {
        glUniform4f(index, v0,v1,v2,v3);
    }

    void Program::setUniform1i( int index,int v0 )
    {
        glUniform1i(index, v0);
    }

    void Program::setUniform2i( int index,int v0, int v1 )
    {
        glUniform2i(index, v0,v1);
    }

    void Program::setUniform3i( int index,int v0, int v1, int v2 )
    {
        glUniform3i(index, v0,v1,v2);
    }

    void Program::setUniform4i( int index,int v0, int v1, int v2, int v3 )
    {
        glUniform4i(index, v0,v1,v2,v3);
    }

    void Program::setUniform1fv( int index,int count, const float *value )
    {
        glUniform1fv(index, count, value);
    }

    void Program::setUniform2fv( int index,int count, const float *value )
    {
        glUniform2fv(index, count, value);
    }

    void Program::setUniform3fv( int index,int count, const float *value )
    {
        glUniform3fv(index, count, value);
    }

    void Program::setUniform4fv( int index,int count, const float *value )
    {
        glUniform4fv(index, count, value);
    }

    void Program::setUniform1iv( int index,int count, const int *value )
    {
        glUniform1iv(index, count, value);
    }

    void Program::setUniform2iv( int index,int count, const int *value )
    {
        glUniform2iv(index, count, value);
    }

    void Program::setUniform3iv( int index,int count, const int *value )
    {
        glUniform3iv(index, count, value);
    }

    void Program::setUniform4iv( int index,int count, const int *value )
    {
        glUniform4iv(index, count, value);
    }

    void Program::setUniformMatrix2fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix2fv(index, count, (GLboolean) transpose, value);
    }

    void Program::setUniformMatrix3fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix3fv(index, count, (GLboolean) transpose, value);
    }

    void Program::setUniformMatrix4fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix4fv(index, count, (GLboolean) transpose, value);
    }

    void Program::bindAttribLocation( const ProgramId& prgId,int index, const char* name )
    {
        glBindAttribLocation(prgId._programId, index, name);
    }

    void Program::setVertexAttrib1f( unsigned index, float v0 )
    {
        glVertexAttrib1f(index, v0);
    }

    void Program::setVertexAttrib2f( unsigned index, float v0, float v1 )
    {
        glVertexAttrib2f(index, v0,v1);
    }

    void Program::setVertexAttrib3f( unsigned index, float v0, float v1, float v2 )
    {
        glVertexAttrib3f(index, v0,v1,v2);
    }

    void Program::setVertexAttrib4f( unsigned index, float v0, float v1, float v2, float v3 )
    {
        glVertexAttrib4f(index, v0,v1,v2,v3);
    }

    void CELLUIProgram::initialize( CELLOpenGL* device )
    {
        _device =   device;
        const char* vs  =   
        {
            "precision  lowp float; "
            "uniform    mat4 _MVP;"
            "attribute  vec3 _position;"
            "attribute  vec3 _uv;"
            "attribute  vec4 _color;"

            "varying    vec3 _outUV;"
            "varying    vec4 _outColor;"
            "void main()"
            "{"
            "   vec4    pos =   vec4(_position.x,_position.y,_position.z,1);"
            "   gl_Position =   _MVP * pos;"
            "   _outUV      =   _uv;"
            "   _outColor   =   _color;"
            "}"
        };
        const char* ps  =   
        {
            "precision  lowp float; "
            "uniform    sampler2D   _texture;"
            "varying    vec4        _outColor;"
            "varying    vec3        _outUV;"
            "void main()"
            "{"
            "   vec4   color   =   texture2D(_texture,vec2(_outUV.x,_outUV.y));"
            "  	if( _outUV.z > 0.0 )\n "
            "		gl_FragColor    =   color.w * _outColor;\n"
            " 	else "
            "       gl_FragColor    =   color * _outColor;"
            "}"
        };
        ProgramId&  programId   =   *this;

        programId   =   device->createProgram(vs,ps);



        _position   =   device->getAttribLocation(programId,    "_position");
        _uv         =   device->getAttribLocation(programId,    "_uv");
        _color      =   device->getAttribLocation(programId,    "_color");


        _texture    =   device->getUniformLocation(programId,   "_texture");
        _MVP        =   device->getUniformLocation(programId,   "_MVP");
    }

    void CELLUIProgram::begin()
    {
        _device->useProgram(this);
        if (_position != -1)
        {
            _device->enableVertexAttributeArray(_position);
        }
        if (_uv != -1)
        {
            _device->enableVertexAttributeArray(_uv);
        }
        if (_color != -1)
        {
            _device->enableVertexAttributeArray(_color);
        }
    }

    void CELLUIProgram::end()
    {
        if (_position != -1)
        {
            _device->disableVertexAttributeArray(_position);
        }
        if (_uv != -1)
        {
            _device->disableVertexAttributeArray(_uv);
        }
        if (_color != -1)
        {
            _device->disableVertexAttributeArray(_color);
        }
        _device->useProgram(0);
    }

}
