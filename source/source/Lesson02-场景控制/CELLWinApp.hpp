#pragma once

#include <Windows.h>
#include <tchar.h>
#include <math.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>
#include "CELLMath.hpp"
#include "CELLFrame.hpp"
#include "CELLOpenGL.hpp"

namespace   CELL
{
    class   CELLWinApp
    {
    public:
        //! 实例句柄
        HINSTANCE   _hInstance;
        //! 窗口句柄
        HWND        _hWnd;
        //! 窗口的高度
        int         _width;
        //! 窗口的宽度
        int         _height;
        /// for gles2.0
        EGLConfig   _config;
        EGLSurface  _surface;
        EGLContext  _context;
        EGLDisplay  _display;
        CELLOpenGL  _device;
        CELLFrame*  _frame;
    public:
        CELLWinApp(HINSTANCE hInstance)
            :_hInstance(hInstance)
        {
            WNDCLASSEX  winClass;
            winClass.lpszClassName  =   _T("CELLWinApp");
            winClass.cbSize         =   sizeof(winClass);
            winClass.style          =   CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            winClass.lpfnWndProc    =   wndProc;
            winClass.hInstance      =   hInstance;
            winClass.hIcon	        =   0;
            winClass.hIconSm	    =   0;
            winClass.hCursor        =   LoadCursor(hInstance, IDC_ARROW);
            winClass.hbrBackground  =   (HBRUSH)GetStockObject(BLACK_BRUSH);
            winClass.lpszMenuName   =   NULL;
            winClass.cbClsExtra     =   0;
            winClass.cbWndExtra     =   0;
            RegisterClassEx(&winClass);
            _frame                  =   0;
        }
        virtual ~CELLWinApp()
        {
            UnregisterClass(_T("CELLWinApp"),_hInstance);
        }

        /**
        *   初始化 OpenGLES2.0
        */
        bool    initOpenGLES20()
        {
            const EGLint attribs[] =
            {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_DEPTH_SIZE,24,
                EGL_NONE
            };
            EGLint 	format(0);
            EGLint	numConfigs(0);
            EGLint  major;
            EGLint  minor;

            //! 1
            _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);

            //! 2init
            eglInitialize(_display, &major, &minor);

            //! 3
            eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

            eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
            //! 4 
            _surface	    = 	eglCreateWindowSurface(_display, _config, _hWnd, NULL);

            //! 5
            EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
            _context 	    = 	eglCreateContext(_display, _config, 0, attr);
            //! 6
            if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
            {
                return false;
            }

            eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
            eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

            return  true;

        }
        /**
        *   销毁OpenGLES2.0
        */
        void    destroyOpenGLES20()
        {
            if (_display != EGL_NO_DISPLAY)
            {
                eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                if (_context != EGL_NO_CONTEXT) 
                {
                    eglDestroyContext(_display, _context);
                }
                if (_surface != EGL_NO_SURFACE) 
                {
                    eglDestroySurface(_display, _surface);
                }
                eglTerminate(_display);
            }
            _display    =   EGL_NO_DISPLAY;
            _context    =   EGL_NO_CONTEXT;
            _surface    =   EGL_NO_SURFACE;
        }
    protected:
        static  LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            
            CELLWinApp*  pThis   =   (CELLWinApp*)GetWindowLong(hWnd,GWL_USERDATA);
            if (pThis)
            {
                return  pThis->onEvent(hWnd,msg,wParam,lParam);
            }
            if (WM_CREATE == msg)
            {
                CREATESTRUCT*   pCreate =   (CREATESTRUCT*)lParam;
                SetWindowLong(hWnd,GWL_USERDATA,(DWORD_PTR)pCreate->lpCreateParams);
            }
            return  DefWindowProc( hWnd, msg, wParam, lParam );
        }
    public:
        /**
        *   事件函数
        */
        virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam)          (int)((short)HIWORD(wParam))
#endif

            switch( msg )
            {
            case WM_SIZE:
                {
                    RECT    rt;
                    GetClientRect(_hWnd,&rt);
                    _width   =   rt.right - rt.left;
                    _height  =   rt.bottom - rt.top;
                    if (_frame)
                    {
                        _frame->onSize(_width,_height);
                    }
                }
                break;
            case WM_LBUTTONDOWN:
                {
                    if (_frame)
                    {
                        _frame->onMousePress(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Left);
                    }
                }
                break;
            case WM_LBUTTONUP:
                {
                    if (_frame)
                    {
                        _frame->onMouseRelease(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Left);
                    }
                }
                break;
            case WM_RBUTTONDOWN:
                {
                    if (_frame)
                    {
                        _frame->onMousePress(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Right);
                    }
                }
                break;
            case WM_RBUTTONUP:
                {
                    if (_frame)
                    {
                        _frame->onMouseRelease(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Right);
                    }
                }
                break;
            case WM_MOUSEMOVE:
                {
                    if (_frame)
                    {
                        _frame->onMouseMove(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),0);
                    }
                }
                break;

            case WM_MOUSEWHEEL:
                {
                    if (_frame)
                    {
                        _frame->onMouseMove(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),GET_WHEEL_DELTA_WPARAM(wParam));
                    }
                }
                break;
            case WM_CHAR:
                {
                    if (_frame)
                    {
                        _frame->onChar(wParam);
                    }
                }
                break;
            case WM_KEYDOWN:
                {
                }
                break;
            case WM_CLOSE:
            case WM_DESTROY:
                {
                    ::PostQuitMessage(0);
                }
                break;
            default:
                return DefWindowProc(_hWnd, msg, wParam, lParam );
            }
            return  S_OK;
            
        }

        /**
        *  初始化通知
        */
        virtual void    onInit()
        {

        }
        /**
        *   绘制通知
        */
        virtual void    onRender()
        {
            FrameEvent  evt;
            if (_frame)
            {
                _frame->onRender(evt,_width,_height);
            }
        }

        /**
        *   销毁
        */
        virtual void    onDestroy()
        {
        }
        virtual CELLFrame*  createFrame()
        {
            return  new CELLFrame(_device,this);
        }
        /**
        *   主函数
        */
        int     main(int width,int height)
        {
            _hWnd   =   CreateWindowEx( NULL,
                                        _T("CELLWinApp"),
                                        _T("CELLWinApp"),
                                        WS_OVERLAPPEDWINDOW,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        width,
                                        height, 
                                        NULL, 
                                        NULL,
                                        _hInstance, 
                                        this
                                        );

            if (_hWnd == 0)
            {
                return  -1;
            }
            UpdateWindow(_hWnd);

            ShowWindow(_hWnd,SW_SHOW);

            if (!initOpenGLES20())
            {
                return  false;
            }
            _device.initialize();
            _device.buildSystemFont("data/font/simsun.ttc",14);

            _frame  =   createFrame();
            if (_frame)
            {
                _frame->main(0,0);
            }

            MSG msg =   {0};
            while(msg.message != WM_QUIT)
            {
                if (msg.message == WM_DESTROY || 
                    msg.message == WM_CLOSE)
                {
                    break;
                }
                /**
                *   有消息，处理消息，无消息，则进行渲染绘制
                */
                if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
                { 
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                onRender();
                eglSwapBuffers(_display,_surface);
                
            }
            _frame->onDestroy();
            delete  _frame;
            onDestroy();
            /**
            *   销毁OpenGLES20
            */
            destroyOpenGLES20();

            return  0;
        }
    };
}
