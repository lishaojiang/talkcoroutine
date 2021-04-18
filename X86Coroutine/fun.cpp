//rayhutnerli
//2021/4/17


#include <iostream>
#include "fun.h"
#include "core.h"

void RunPrintfTest()
{
    int i = 0;
    for(i = 0;i < 10;i++)
    {
		std::cout << "I am run,Times:" << i << std::endl;
    }
}



void RunPrintfTestCanBreak(void* pParam)
{
    int i = 0;
    int a = 100;
    for(i = 0;i < 10;i++)
    {
        a += 10;
		std::cout << "I am run,Times:" << i << ",the a is:"<< a << std::endl;
        COROUTINE_YIELD;
    }
    COROUTINE_END;
}