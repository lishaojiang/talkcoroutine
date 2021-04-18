//rayhutnerli
//2021/4/17


// just for test,it is so simple
// not support funcition param
// not support f

#ifndef COROUTINE_H_
#define COROUTINE_H_

#include <list>
#include "funenv.h"


class CoroutineMgr
{
public:
    void RegeisterFun(RegCallFun pfn);
    void UpdateFun();
    bool IsEmpty();
protected:
    std::list<FunEnvInfoPtr> m_kFunList;
};

extern CoroutineMgr g_kFunMgr;

#endif