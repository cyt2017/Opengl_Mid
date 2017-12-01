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
        *   ����ƶ�
        */
        virtual void    onMouseMove(int absx, int absy, int absz)  =   0;
        /**
        *   ��갴��
        */
        virtual void    onMousePress(int absx, int absy, CELL::MouseButton id)  =   0;
        /**
        *   ���˫��
        */
        virtual void    onMouseDbPress(int absx, int absy, CELL::MouseButton id) =   0;
        /**
        *   ����ͷ�
        */
        virtual void    onMouseRelease(int absx, int absy, CELL::MouseButton id)  =   0;
        /**
        *   ���̰���
        */
        virtual void    onKeyPress(CELL::KeyCode key, int text)  =   0;
        /**
        *   ����̧��
        */
        virtual void    onKeyRelease(CELL::KeyCode key)  =   0;
        /**
        *   ����
        */
        virtual void    onChar(int ch)  =   0;
    };
}
