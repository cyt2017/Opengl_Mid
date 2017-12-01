#pragma once

#include "CELLMath.hpp"
#include "CELLObject.hpp"
#include "CELLInput.hpp"
#include "CELLOpenGL.hpp"

namespace   CELL
{

    class   CELLFrame 
            :public CELLInput
            ,public CELLObject
    {
    private:
        void    operator = (const CELLFrame& );
        CELLFrame(const CELLFrame& right);

    protected:
        CELLOpenGL& _device;
    public:
        CELLFrame(CELLOpenGL& device,void* user = 0) 
            :_device(device)
            ,CELLObject(user)
        {}
        virtual	~CELLFrame(){}

        virtual void    onTouchBegin(){}
        virtual void    onTouchMove(){}
        virtual void    onTouchEnd(){}
        /**
        *   ����ƶ�
        */
        virtual void    onMouseMove(int /*absx*/, int /*absy*/, int /*absz*/){}
        /**
        *   ��갴��
        */
        virtual void    onMousePress(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   ���˫��
        */
        virtual void    onMouseDbPress(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   ����ͷ�
        */
        virtual void    onMouseRelease(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   ���̰���
        */
        virtual void    onKeyPress(CELL::KeyCode /*key*/, int /*text*/){}
        /**
        *   ����̧��
        */
        virtual void    onKeyRelease(CELL::KeyCode /*key*/){}
        /**
        *   ����
        */
        virtual void    onChar(int /*ch*/){}

        /**
        *   ��ں���
        */
        virtual int     main(int /*argc*/,char** /*argv*/)  {   return 0;   }

        /**
        *   ���ƺ���
        */
        virtual void    onRender(const FrameEvent& /*evt*/,int width,int height){}
        /**
        *   ���ڴ�С�����仯�¼�
        */
        virtual void    onSize(int /*width*/,int /*height*/){}
        /**
        *   �����¼�
        */
        virtual void    onDestroy(){}
        


    };

}
