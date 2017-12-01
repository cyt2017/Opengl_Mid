#pragma once

#include "CELLFrame.hpp"

namespace   CELL
{
    class   GLES20Frame :public CELLFrame
    {
    public:
        GLES20Frame(CELLOpenGL& device,void* user = 0)
            :CELLFrame(device,user)
        {
        }
        /**
        *   ��ں���
        */
        virtual int     main(int /*argc*/,char** /*argv*/) 
        {   
            return 0;   
        }

        /**
        *   ���ƺ���
        */
        virtual void    onRender(const FrameEvent& /*evt*/,int width,int height)
        {
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glViewport(0,0,width,height);

        }

    };
}