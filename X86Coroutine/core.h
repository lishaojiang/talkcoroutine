//rayhutnerli
//2021/4/17


#ifndef CORE_H_
#define CORE_H_

#include "funenv.h"


void __stdcall CoroutineExec(FunEnvInfoPtr infoPtr);
void __stdcall CoroutineBreak(FunEnvInfoPtr infoPtr);
void __stdcall CoroutineEnd(FunEnvInfoPtr infoPtr);


#define COROUTINE_RUN(env) CoroutineExec(env);
#define COROUTINE_YIELD CoroutineBreak((FunEnvInfoPtr)pParam)
#define COROUTINE_END CoroutineEnd((FunEnvInfoPtr)pParam)


#endif