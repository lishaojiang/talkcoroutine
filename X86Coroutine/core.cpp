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
		cmp [eax]FunEnvInfo.reEIP, 0

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
		sub esp, [eax]FunEnvInfo.iStackSize

		// 4.copy memory

		mov edi, esp
		mov esi, [eax]FunEnvInfo.pStackMem
		mov ecx, [eax]FunEnvInfo.iStackSize
		rep movsb

		// 5.setup common regs
		mov ebx, [eax]FunEnvInfo.reEBX
		mov ecx, [eax]FunEnvInfo.reECX
		mov edx, [eax]FunEnvInfo.reEDX
		mov esi, [eax]FunEnvInfo.reESI
		mov edi, [eax]FunEnvInfo.reEDI

		// 6.jum EIP
		mov eax, [eax]FunEnvInfo.reEIP
		jmp eax


		FIRSTEXCFUNCTION:
		// prepare function param and function addresss
		push eax
		mov eax, [eax]FunEnvInfo.pfCallFun

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
		mov [eax]FunEnvInfo.reESP, esp
		mov [eax]FunEnvInfo.reEBP, ebp
		mov [eax]FunEnvInfo.reEBX, ebx
		mov [eax]FunEnvInfo.reECX, ecx
		mov [eax]FunEnvInfo.reEDX, edx
		mov [eax]FunEnvInfo.reESI, esi
		mov [eax]FunEnvInfo.reEDI, edi

		// use esi instead of eax,eax maybe use as function return value
		mov esi, eax


		// get the eip and save
		// it will be jump to eip when the function is called next time
		mov ecx, [esp]
		mov [esi]FunEnvInfo.reEIP, ecx


		// skip the function ret address and param temp and save esp
		mov ecx, [esi]FunEnvInfo.reESP
		// we must add 8,4 ret,4 param
		add ecx, 8
		mov [esi]FunEnvInfo.reESP, ecx

		// calculate stack size of parent function and iStackSize
		mov edx, [esi]FunEnvInfo.reEBP
		sub edx,ecx
		mov [esi]FunEnvInfo.iStackSize, edx

		// if the parent function stack is greater than 0, allocate memory to save
		test edx,edx
		jle ERROR_PARENT_STACK_SIZE

		// If memory has been allocated, it will not be allocated again
		mov eax, [esi]FunEnvInfo.pStackMem
		test eax, eax
		jne HAS_ALLOCATED_MEMROY

		// allocate memory
		push edx
		push ecx
		push edx
		call dword ptr[malloc]
		add esp, 4
		mov [esi]FunEnvInfo.pStackMem, eax
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
		mov eax, [esi]FunEnvInfo.iStackSize
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
		mov eax, [esi]FunEnvInfo.pStackMem
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
		mov dword ptr[esi]FunEnvInfo.pStackMem, 0
		mov dword ptr[esi]FunEnvInfo.iStackSize, 0
		mov dword ptr[esi]FunEnvInfo.reEIP, 0

		DO_NOT_DELETE:
		pop esi
		ret 4;
	}
}