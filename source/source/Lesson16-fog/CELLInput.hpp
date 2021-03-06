#pragma once
#include    "CELLKeyCode.hpp"
#include    "CELLMouseButton.hpp"

namespace   CELL
{
    class   CELLInput
    {
    public:
    	virtual	~CELLInput(){}
        /**
        *   鼠标移动
        */
        virtual void    onMouseMove(int absx, int absy, int absz)  =   0;
        /**
        *   鼠标按下
        */
        virtual void    onMousePress(int absx, int absy, CELL::MouseButton id)  =   0;
        /**
        *   鼠标双击
        */
        virtual void    onMouseDbPress(int absx, int absy, CELL::MouseButton id) =   0;
        /**
        *   鼠标释放
        */
        virtual void    onMouseRelease(int absx, int absy, CELL::MouseButton id)  =   0;
        /**
        *   键盘按下
        */
        virtual void    onKeyPress(CELL::KeyCode key, int text)  =   0;
        /**
        *   键盘抬起
        */
        virtual void    onKeyRelease(CELL::KeyCode key)  =   0;
        /**
        *   输入
        */
        virtual void    onChar(int ch)  =   0;
    };
}
