
#pragma once


namespace   CELL
{
    /**
    *   图片纹理数据格式
    */
    enum	PIXEL_FORMAT
    {
        PIXEL_UNKNOW	,
        PIXEL_I8		,
        PIXEL_A8        ,
        PIXEL_R8G8B8	,
        PIXEL_R8G8B8A8	,
        PIXEL_B8G8R8	,
        PIXEL_B8G8R8A8	,
        PIXEL_B8G8R8E8	,
        PIXEL_R8G8B8E8	,			//!	hdr
        PIXEL_RGBA4     ,           //! 16bit
        PIXEL_IA8       ,           //! 16bit
        PIXEL_RGB565    ,           //! 16bit
        PIXEL_I16       ,           //! 16bit
        PIXEL_RGB10A2   ,           //! 32bit 
        PIXEL_YUY2		,
        PIXEL_YUYV		,
        PIXEL_UYVY		,
        PIXEL_RGBA_S3TC_DXT1,
        PIXEL_RGBA_S3TC_DXT3,
        PIXEL_RGBA_S3TC_DXT5,
        PIXEL_FORCE_DWORD  =   0x7fffffff,
    };

    #define DDSCAPS2_CUBEMAP    0x00000200L
    

    #define DUMMYUNIONNAMEN(n)
    
    typedef void*           LPVOID;
    typedef unsigned char   BYTE;
    typedef unsigned short  WORD;
    typedef unsigned int    DWORD;
    typedef long            LONG;

    #ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)  \
                        ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                        ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
    #endif

    #define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
    #define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
    #define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
    #define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
    #define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

    typedef struct _DDSCAPS2_CELL
    {
        DWORD       dwCaps;         // capabilities of surface wanted
        DWORD       dwCaps2;
        DWORD       dwCaps3;
        union
        {
            DWORD       dwCaps4;
            DWORD       dwVolumeDepth;
        } DUMMYUNIONNAMEN(1);
    } CELLDDSCAPS2;


    typedef struct _DDCOLORKEY_CELL
    {
        DWORD       dwColorSpaceLowValue;  
        DWORD       dwColorSpaceHighValue; 
    } CELL_DDCOLORKEY;

    typedef struct _DDPIXELFORMAT_CELL
    {
        DWORD       dwSize;                 // size of structure
        DWORD       dwFlags;                // pixel format flags
        DWORD       dwFourCC;               // (FOURCC code)
        union
        {
            DWORD   dwRGBBitCount;          // how many bits per pixel
            DWORD   dwYUVBitCount;          // how many bits per pixel
            DWORD   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
            DWORD   dwAlphaBitDepth;        // how many bits for alpha channels
            DWORD   dwLuminanceBitCount;    // how many bits per pixel
            DWORD   dwBumpBitCount;         // how many bits per "buxel", total
            DWORD   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
            // format list and if DDPF_D3DFORMAT is set
        } DUMMYUNIONNAMEN(1);
        union
        {
            DWORD   dwRBitMask;             // mask for red bit
            DWORD   dwYBitMask;             // mask for Y bits
            DWORD   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
            DWORD   dwLuminanceBitMask;     // mask for luminance bits
            DWORD   dwBumpDuBitMask;        // mask for bump map U delta bits
            DWORD   dwOperations;           // DDPF_D3DFORMAT Operations
        } DUMMYUNIONNAMEN(2);
        union
        {
            DWORD   dwGBitMask;             // mask for green bits
            DWORD   dwUBitMask;             // mask for U bits
            DWORD   dwZBitMask;             // mask for Z bits
            DWORD   dwBumpDvBitMask;        // mask for bump map V delta bits
            struct
            {
                WORD    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
                WORD    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
            } MultiSampleCaps;

        } DUMMYUNIONNAMEN(3);
        union
        {
            DWORD   dwBBitMask;             // mask for blue bits
            DWORD   dwVBitMask;             // mask for V bits
            DWORD   dwStencilBitMask;       // mask for stencil bits
            DWORD   dwBumpLuminanceBitMask; // mask for luminance in bump map
        } DUMMYUNIONNAMEN(4);
        union
        {
            DWORD   dwRGBAlphaBitMask;      // mask for alpha channel
            DWORD   dwYUVAlphaBitMask;      // mask for alpha channel
            DWORD   dwLuminanceAlphaBitMask;// mask for alpha channel
            DWORD   dwRGBZBitMask;          // mask for Z channel
            DWORD   dwYUVZBitMask;          // mask for Z channel
        } DUMMYUNIONNAMEN(5);
    } CELL_DDPIXELFORMAT;


    typedef struct _DDSURFACEDESC2_CELL
    {
        DWORD               dwSize;                 // size of the DDSURFACEDESC structure
        DWORD               dwFlags;                // determines what fields are valid
        DWORD               dwHeight;               // height of surface to be created
        DWORD               dwWidth;                // width of input surface
        union
        {
            LONG            lPitch;                 // distance to start of next line (return value only)
            DWORD           dwLinearSize;           // Formless late-allocated optimized surface size
        } DUMMYUNIONNAMEN(1);
        union
        {
            DWORD           dwBackBufferCount;      // number of back buffers requested
            DWORD           dwDepth;                // the depth if this is a volume texture 
        } DUMMYUNIONNAMEN(5);
        union
        {
            DWORD           dwMipMapCount;          // number of mip-map levels requestde
            DWORD           dwRefreshRate;          // refresh rate (used when display mode is described)
            DWORD           dwSrcVBHandle;          // The source used in VB::Optimize
        } DUMMYUNIONNAMEN(2);
        DWORD               dwAlphaBitDepth;        // depth of alpha buffer requested
        DWORD               dwReserved;             // reserved
        LPVOID              lpSurface;              // pointer to the associated surface memory
        union
        {
            CELL_DDCOLORKEY ddckCKDestOverlay;      // color key for destination overlay use
            DWORD           dwEmptyFaceColor;       // Physical color for empty cubemap faces
        } DUMMYUNIONNAMEN(3);
        CELL_DDCOLORKEY     ddckCKDestBlt;          // color key for destination blt use
        CELL_DDCOLORKEY     ddckCKSrcOverlay;       // color key for source overlay use
        CELL_DDCOLORKEY     ddckCKSrcBlt;           // color key for source blt use
        union
        {
            CELL_DDPIXELFORMAT  ddpfPixelFormat;        // pixel format description of the surface
            DWORD               dwFVF;                  // vertex format description of vertex buffers
        } DUMMYUNIONNAMEN(4);
        CELLDDSCAPS2            ddsCaps;                // direct draw surface capabilities
        DWORD               dwTextureStage;         // stage in multitexture cascade
    } CELL_DDSURFACEDESC2;

    class   CELLImage
    {
    public:
        unsigned	    _size;
        unsigned	    _width;
        unsigned	    _height;
        unsigned	    _depth;
        PIXEL_FORMAT	_format;	
        unsigned	    _mipmap;
        unsigned	    _blockSize;
        char*           _data;
        bool            _deleteData;
        bool		    _is2d;
        bool		    _compressed;
    public:
        CELLImage()
        {
            _size       =   0;
            _width      =   0;
            _height     =   0;
            _depth      =   0;
            _format     =   PIXEL_UNKNOW;
            _mipmap     =   0;
            _is2d       =   true;
            _compressed =   false;
            _blockSize  =   0;
            _data       =   0;
            _deleteData =   true;
        }
        virtual ~CELLImage()
        {
            destroy();
        }
    public:
        void    destroy()
        {
            if (_data && _deleteData)
            {
                delete []_data;
            }
            _size       =   0;
            _width      =   0;
            _height     =   0;
            _depth      =   0;
            _format     =   PIXEL_UNKNOW;
            _mipmap     =   0;
            _is2d       =   true;
            _compressed =   false;
            _blockSize  =   0;
            _data       =   0;
        }

        char*	getPixles(unsigned mipMap)
        {
            if ( mipMap == 0)
            {
                return	_data;
            }
            if ( mipMap >= _mipmap )
            {
                return	0;
            }
            else
            {
                unsigned	iOffset	=	0;
                for ( size_t i = 0 ; i < mipMap ; ++ i )
                {
                    iOffset	+=	getMipmapsize(i);
                }
                return	_data + iOffset;
            }
        }

        unsigned    getWidth(unsigned miplev )
        {
            return  _width>>miplev;
        }

        unsigned    getHeight(unsigned miplev )
        {
            return  _height>>miplev;
        }

        unsigned	getMipmapsize(unsigned nMipLev )
        {
            unsigned    iRetSize    =   0;
            unsigned    iTemWidth   =   getWidth(nMipLev);
            unsigned    iTemHeight  =   getHeight(nMipLev);
            if (_compressed)
            {
                iRetSize    +=	((iTemWidth + 3)/4) * ((iTemHeight + 3)/4) * _blockSize;
            }
            else
            {
                iRetSize    +=	iTemWidth * iTemHeight * getBytesPerPixel(_format);
            }

            return  iRetSize;
        }


        inline  unsigned    getBytesPerPixel(PIXEL_FORMAT fmt)
        {
            switch(fmt)
            {
            case    PIXEL_I8		:
                return  1;
            case    PIXEL_B8G8R8	:
            case    PIXEL_R8G8B8	:
                return  3;
            case    PIXEL_R8G8B8A8	:
            case    PIXEL_B8G8R8A8	:
            case    PIXEL_B8G8R8E8	:
            case    PIXEL_R8G8B8E8	:			//!	hdr
                return  4;
            case    PIXEL_RGBA4     :           //! 16bit
            case    PIXEL_IA8       :           //! 16bit
            case    PIXEL_RGB565    :           //! 16bit
            case    PIXEL_I16       :           //! 16bit
                return  2;
            case    PIXEL_RGB10A2   :           //! 32bit 
                return  4;
            case    PIXEL_YUY2		:
            case    PIXEL_YUYV		:
            case    PIXEL_UYVY		:
                return  2;
            }
            return  1;
        }
        /**
        *   计算图片的大小
        */
        unsigned    calcSize()
        {
            _size   =   0;

            if (_mipmap == 0)
            {
                if ( _compressed )
                    return	_size = _width * _height * _blockSize /16;
                else
                    return	_size = _width * _height * getBytesPerPixel(_format);
            }
            else
            {
                if (_compressed)
                {
                    for ( size_t i = 0 ; i < _mipmap ; ++ i)
                    {
                        unsigned	iTemWidth	=	_width>>i;
                        unsigned	iTemHeight	=	_height>>i;
                        _size	+=	iTemWidth * iTemHeight * _blockSize / 16;
                    }
                }
                else
                {
                    for ( size_t i = 0 ; i < _mipmap ; ++ i)
                    {
                        unsigned	iTemWidth	=	_width>>i;
                        unsigned	iTemHeight	=	_height>>i;
                        _size	+=	iTemWidth * iTemHeight * _blockSize / 16;
                    }
                }
            }
            return	(unsigned)_size;
        }


        bool    loadDDSFromStream(const char* stream,unsigned /*length*/)
        {
            const char* src =   stream;
            /**
            *   是否是dds
            */
            if( strncmp( src, "DDS ", 4 ) != 0 )
            {
                return  false;
            }
            src         +=  4;
            _deleteData =   false;
            /**
            *   dds header
            */
            CELL_DDSURFACEDESC2 ddsd;
            memcpy(&ddsd,src,sizeof(ddsd));

            src         +=  sizeof(ddsd);

            _width      =   ddsd.dwWidth;
            _height     =   ddsd.dwHeight;
            _depth	    =   (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP)? 0 : (ddsd.dwDepth == 0)? 1 : ddsd.dwDepth;
            _mipmap     =   ddsd.dwMipMapCount;
            _compressed =   true;
            switch( ddsd.ddpfPixelFormat.dwFourCC )
            {
            case FOURCC_DXT1:
                {
                    //	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                    _format	    =	PIXEL_RGBA_S3TC_DXT1;
                    _blockSize	=	8;
                }
                break;
            case FOURCC_DXT3:
                {
                    //	DXT3's compression ratio is 4:1
                    _format	    =	PIXEL_RGBA_S3TC_DXT3;
                    _blockSize	=	16;
                }
                break;
            case FOURCC_DXT5:
                {
                    //	DXT5's compression ratio is 4:1
                    _format	    =	PIXEL_RGBA_S3TC_DXT5;
                    _blockSize	=	16;
                }
                break;
            default:
                {   
                    _compressed =   false;
                    switch (ddsd.ddpfPixelFormat.dwRGBBitCount)
                    {
                    case 8: 
                        {
                            _format    =   PIXEL_I8;
                        }
                        break;
                    case 16:
                        {
                            _format    =   (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xF000)? PIXEL_RGBA4 : 
                                (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xFF00)? PIXEL_IA8 : 
                                (ddsd.ddpfPixelFormat.dwBBitMask == 0x1F)? PIXEL_RGB565 : PIXEL_I16;
                        }
                        break;
                    case 24:
                        {
                            _format    =   PIXEL_R8G8B8; 
                        }
                        break;
                    case 32:
                        {
                            _format    =   (ddsd.ddpfPixelFormat.dwRBitMask == 0x3FF00000)? PIXEL_RGB10A2 : PIXEL_R8G8B8A8;
                        }
                        break;
                    default:
                        return false;
                    }
                }
            }
            calcSize();
            /**
            *   内存赋值
            */
            _data   =   (char*)src;
            if (_format == PIXEL_R8G8B8A8)
            {
                for (size_t i = 0 ;i < _size ; i += 4)
                {
                    char temp       =   _data[i + 0];
                    _data[i + 0]    =   _data[i + 3];
                    _data[i + 3]    =   temp;
                }
            }
        }
        bool    loadDDS(const char* fileName)
        {
            /**
            *   销毁之前数据
            */
            destroy();

            CELL_DDSURFACEDESC2 ddsd;
            char    filecode[4];
            FILE*   pFile	=	fopen( fileName, "rb" );
            if( pFile == 0 )
            {
                return  false;
            }
            fread( filecode, 1, 4, pFile );
            if( strncmp( filecode, "DDS ", 4 ) != 0 )
            {
                fclose( pFile );
                return  false;
            }
            fread( &ddsd, sizeof(ddsd), 1, pFile );
            _width      =   ddsd.dwWidth;
            _height     =   ddsd.dwHeight;
            _depth	    =   (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP)? 0 : (ddsd.dwDepth == 0)? 1 : ddsd.dwDepth;
            _mipmap     =   ddsd.dwMipMapCount;
            _compressed =   true;
            switch( ddsd.ddpfPixelFormat.dwFourCC )
            {
            case FOURCC_DXT1:
                {
                    //	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                    _format	    =	PIXEL_RGBA_S3TC_DXT1;
                    _blockSize	=	8;
                }
                break;
            case FOURCC_DXT3:
                {
                    //	DXT3's compression ratio is 4:1
                    _format	    =	PIXEL_RGBA_S3TC_DXT3;
                    _blockSize	=	16;
                }
                break;
            case FOURCC_DXT5:
                {
                    //	DXT5's compression ratio is 4:1
                    _format	    =	PIXEL_RGBA_S3TC_DXT5;
                    _blockSize	=	16;
                }
                break;
            default:
                {   
                    _compressed =   false;
                    switch (ddsd.ddpfPixelFormat.dwRGBBitCount)
                    {
                    case 8: 
                        {
                            _format    =   PIXEL_I8;
                        }
                        break;
                    case 16:
                        {
                            _format    =   (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xF000)? PIXEL_RGBA4 : 
                                            (ddsd.ddpfPixelFormat.dwRGBAlphaBitMask == 0xFF00)? PIXEL_IA8 : 
                                            (ddsd.ddpfPixelFormat.dwBBitMask == 0x1F)? PIXEL_RGB565 : PIXEL_I16;
                        }
                        break;
                    case 24:
                        {
                            _format    =   PIXEL_R8G8B8; 
                        }
                        break;
                    case 32:
                        {
                            _format    =   (ddsd.ddpfPixelFormat.dwRBitMask == 0x3FF00000)? PIXEL_RGB10A2 : PIXEL_R8G8B8A8;
                        }
                        break;
                    default:
                        fclose(pFile);
                        return false;
                    }
                }
            }
            calcSize();
            _data   =   new char[_size];
            fread( _data, 1, _size, pFile );
            if (_format == PIXEL_R8G8B8A8)
            {
                for (size_t i = 0 ;i < _size ; i += 4)
                {
                    char temp       =   _data[i + 0];
                    _data[i + 0]    =   _data[i + 3];
                    _data[i + 3]    =   temp;
                }
            }
            fclose( pFile );
            return  true;
        }
    };
}