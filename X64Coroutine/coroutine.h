// MySimpleCoroutine
// coroutine.h
// 
// create by rayhunterli
// 2021/4/5
// add version new more functions 2021/4/9

#ifndef MYSIMPLECOROUTINE_COROUTINE_H_
#define MYSIMPLECOROUTINE_COROUTINE_H_

#include "asm.h"
#include <vector>
#include <map>


typedef void (*coroutinefunc)(transfer);

struct FuncRecord
{
	FuncRecord() :
		pkFunc(0),
		iFuncIndex(0),
		pkStackMem(nullptr),
		iStatckSize(0),
		iUseStatckSize(0),
		pkUseStack(nullptr),
		pkCorContext(nullptr) {}
	FuncRecord(coroutinefunc _pf,size_t _iFuncIndex,void* _pkMem,size_t _iSize) :
		pkFunc(_pf),
		iFuncIndex(_iFuncIndex),
		pkStackMem(_pkMem),
		iStatckSize(_iSize),
		iUseStatckSize(0),
		pkUseStack(nullptr),
		pkCorContext(nullptr) {}

	void* pkStackMem; // sp memory
	size_t iStatckSize; // sp size
	coroutinefunc pkFunc;
	size_t iFuncIndex;
	void* pkUseStack;
	size_t iUseStatckSize; // use size
	corcontext pkCorContext;
};

class coroutine
{
public:
	coroutine();
	~coroutine();
	void UpdateFun();

	void RegeisterFun(coroutinefunc pf);
	void EndFuncInstance(transfer t);
	void BreakFuncInstance(transfer t);

	const bool IsEmpty() const { return m_kFunRecordMap.size() == 0; }

private:
	char* AllocMemory(size_t iSize);
private:
	const int m_iDefaultStackMemorySize;
	size_t m_iCurrentFuncIndex;
	size_t m_iCreateFuncIndex;
	std::map<size_t,FuncRecord> m_kFunRecordMap;
public:
	
};

extern coroutine g_kFunMgr;

#define COROUTINE_YIELD g_kFunMgr.BreakFuncInstance(t)
#define COROUTINE_END g_kFunMgr.EndFuncInstance(t)

#endif