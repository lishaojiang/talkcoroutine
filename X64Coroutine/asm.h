// MySimpleCoroutine
// asm.h
// just for asm code function declare
// create by rayhunterli
// 2021/4/5

#ifndef MYSIMPLECOROUTINE_ASM_H_
#define MYSIMPLECOROUTINE_ASM_H_


typedef void* corcontext;
struct transfer
{
	corcontext fcont;
	void* data;
};
typedef void (*MakeFun)(transfer);

extern "C" transfer jump_fcontext(corcontext const to ,void* vp);
extern "C" corcontext make_fcontext(void* sp,long long size,MakeFun fn);


#endif