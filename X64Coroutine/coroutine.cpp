//
//  coroutine.cpp
//  MySimpleCoroutine
//
//  Created by rayhunterli on 2021/4/5.
//


#include "asm.h"
#include "coroutine.h"
#include <iostream>
#include <assert.h>


coroutine g_kFunMgr;

coroutine::coroutine():
	m_iDefaultStackMemorySize(128 * 1024),
	m_iCurrentFuncIndex(-1),
	m_iCreateFuncIndex(-1)
{
	m_kFunRecordMap.clear();
}


coroutine::~coroutine()
{
	
}

void coroutine::UpdateFun()
{
	auto iter = m_kFunRecordMap.begin();
	while(iter != m_kFunRecordMap.end())
	{
		if (iter->second.pkCorContext == nullptr)
		{
			void* pkDeleteMem = iter->second.pkStackMem;
			iter = m_kFunRecordMap.erase(iter);
			delete[] pkDeleteMem;
			continue;
		}
		m_iCurrentFuncIndex = iter->first;
		std::cout << "main enterty function index:" << iter->first << std::endl;

		// change from main context to virtual stack context 
		const corcontext corFuncContext =  jump_fcontext(iter->second.pkCorContext, nullptr).fcont;
		if (iter->second.pkCorContext != nullptr)
		{
			iter->second.pkCorContext = corFuncContext;
		}
		iter++;
	}
}

void coroutine::BreakFuncInstance(transfer t)
{	
	// change from  virtual stack context to main context
	auto iter = m_kFunRecordMap.find(m_iCurrentFuncIndex);
	if (iter != m_kFunRecordMap.end())
	{
		// t.fcont is main context,but the iter->second.pkCorContext is coroutine context
		iter->second.pkCorContext = jump_fcontext(t.fcont, nullptr).fcont;
	}
	
}


void coroutine::EndFuncInstance(transfer t)
{
	// it needs exit the coroutine function context
	auto iter = m_kFunRecordMap.find(m_iCurrentFuncIndex);
	if (iter != m_kFunRecordMap.end())
	{
		iter->second.pkCorContext = nullptr;
		jump_fcontext(t.fcont, nullptr);
	}
}



void coroutine::RegeisterFun(coroutinefunc pf)
{
	size_t iStackSize = m_iDefaultStackMemorySize;
	char* pkMem = AllocMemory(iStackSize);
	if (pkMem == nullptr)
	{
		return;
	}

	FuncRecord rec(pf, ++m_iCreateFuncIndex, (void*)pkMem, iStackSize);

	char* pkMemTop = pkMem + iStackSize;
	void* pkStackTop = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pkMemTop) & ~static_cast<uintptr_t>(0xff));
	pkStackTop = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pkStackTop) - static_cast<uintptr_t>(64));
	size_t size = (reinterpret_cast<uintptr_t>(pkStackTop) - reinterpret_cast<uintptr_t>(pkMem));

	rec.pkUseStack = pkStackTop;
	rec.iUseStatckSize = size;
	rec.pkCorContext = make_fcontext(pkStackTop, size, pf);

	m_kFunRecordMap[m_iCreateFuncIndex] = rec;
}


char* coroutine::AllocMemory(size_t iSize)
{
	char* pkMemNew = new char[m_iDefaultStackMemorySize];
	assert(pkMemNew != nullptr);
	return pkMemNew;

}
