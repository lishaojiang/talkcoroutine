//rayhutnerli
//2021/4/17



#include <iostream>
#include "fun.h"
#include "coroutine.h"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	_CrtSetDebugFillThreshold(0);
#endif

    std::cout <<"Hello World!"<< std::endl;
    RunPrintfTest();

    int iMainFrame = 0;
    g_kFunMgr.RegeisterFun(RunPrintfTestCanBreak);
    while(!g_kFunMgr.IsEmpty())
    {
        std::cout << "FrameCout:" << iMainFrame++ << std::endl;
        g_kFunMgr.UpdateFun();
    }
    std::cout <<"update FunMgr over!"<< std::endl;
    
    return 1;
}