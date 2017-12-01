

#include "CELLWinApp.hpp"
#include "GLES20Frame.hpp"

class   MyApp :public CELL::CELLWinApp
{
public:
    MyApp(HINSTANCE hInst) 
        :CELL::CELLWinApp(hInst)
    {

    }
    virtual CELL::CELLFrame*  createFrame()
    {
        return  new CELL::GLES20Frame(_device,this);
    }
};


int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    MyApp  app(hInstance);
    app.main(800,600);

    return 0;
}

