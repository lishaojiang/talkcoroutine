//rayhutnerli
//2021/4/17

#include "core.h"
#include <stdlib.h>


__declspec(naked) void __stdcall CoroutineExec(FunEnvInfoPtr infoPtr)
{
	// don't believe MSVC compile,it always assumes you use ebp
	// I've been cheated many times,rayhunterli
	// ex: mov eax, infoPtr
	// It may be translated incorrectly

	__asm
	{
		// compare last save eip empty
		mov eax, [esp + 4]
		cmp [eax], 0

		// protect some use regs
		push ebp
		push esi
		push edi
		push ecx
		push ebx

		// compare
		je FIRSTEXCFUNCTION

		// here,we jump the fuction again

		// 1. prepare function param and return address
		push eax
		mov ecx, dword ptr[EXEC_RET]
		push ecx

		// 2.setup ebp
		push ebp
		mov ebp, esp

		// 3.calculate stack size
		sub esp, [eax + 40]

		// 4.copy memory

		mov edi, esp
		mov esi, [eax + 44]
		mov ecx, [eax + 40]
		rep movsb

		// 5.setup common regs
		mov ebx, [eax + 16]
		mov ecx, [eax + 20]
		mov edx, [eax + 24]
		mov esi, [eax + 28]
		mov edi, [eax + 32]

		// 6.jum EIP
		mov eax, [eax + 0]
		jmp eax


		FIRSTEXCFUNCTION:
		// prepare function param and function addresss
		push eax
		mov eax, [eax + 48];

		// call the function
		call eax

		EXEC_RET :
		// We have to balance the stack

		// 1.fucntion praam 
		add esp, 4

		// save change
		pop ebx
		pop ecx
		pop edi
		pop esi
		pop ebp
		// call address and param
		ret 4;
	}

}


__declspec(naked) void __stdcall CoroutineBreak(FunEnvInfoPtr infoPtr)
{
	// don't believe MSVC compile,it always assumes you use ebp
	// I've been cheated many times,rayhunterli
	// ex: mov eax, infoPtr
	// It may be translated incorrectly

	__asm
	{
		// save some parent function regs
		// we must save ebx,edx,esi,edi
		mov eax, [esp + 4]
		mov [eax + 4], esp
		mov [eax + 8], ebp
		mov [eax + 16], ebx
		mov [eax + 20], ecx
		mov [eax + 24], edx
		mov [eax + 28], esi
		mov [eax + 32], edi

		// use esi instead of eax,eax maybe use as function return value
		mov esi, eax


		// get the eip and save
		// it will be jump to eip when the function is called next time
		mov ecx, [esp]
		mov [esi + 0], ecx


		// skip the function ret address and param temp and save esp
		mov ecx, [esi + 4]
		// we must add 8,4 ret,4 param
		add ecx, 8
		mov [esi + 4], ecx

		// calculate stack size of parent function and iStackSize
		mov edx, [esi + 8]
		sub edx,ecx
		mov [esi + 40], edx

		// if the parent function stack is greater than 0, allocate memory to save
		test edx,edx
		jle ERROR_PARENT_STACK_SIZE

		// If memory has been allocated, it will not be allocated again
		mov eax, [esi + 44]
		test eax, eax
		jne HAS_ALLOCATED_MEMROY

		// allocate memory
		push edx
		push ecx
		push edx
		call dword ptr[malloc]
		add esp, 4
		mov[esi + 44], eax
		pop ecx
		pop edx



		// memcpy the parent stack memory to new allocated memory
		HAS_ALLOCATED_MEMROY:
		push esi
		mov edi, eax
		mov esi, ecx
		mov ecx, edx
		rep movsb
		pop esi


		// we need to jump back to the coroutineexec function
		// not the parent function
		ERROR_PARENT_STACK_SIZE:
		// calculate parent stack size,skip the self function return address 4 bytes
		mov eax, [esi + 40]
		add eax, 8
		// balance the stack
		add esp, eax

		// get the caller's parent caller
		pop ebp
		ret

	}
}


__declspec(naked) void __stdcall CoroutineEnd(FunEnvInfoPtr infoPtr)
{
	// don't believe MSVC compile,it always assumes you use ebp
	// I've been cheated many times,rayhunterli
	// ex: mov eax, infoPtr
	// It may be translated incorrectly
	__asm
	{
		// compare whether we need to delete the stack memory
		push esi
		mov esi, [esp + 8]
		mov eax, [esi +44]
		test eax, eax
		je DO_NOT_DELETE

		// delete the memory
		push edx
		push ecx
		push eax
		call dword ptr[free]
		add esp, 4
		pop ecx
		pop edx



		// clean up some function's FunEnvInfoPtr params
		// we must use dword ptr for number
		mov dword ptr[esi + 44], 0
		mov dword ptr[esi + 40], 0
		mov dword ptr[esi + 0], 0

		DO_NOT_DELETE:
		pop esi
		ret 4;
	}
}