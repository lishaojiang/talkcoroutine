//rayhutnerli
//2021/4/17


#ifndef FUNINFO_H_
#define FUNINFO_H_

#include <stdint.h>
#include <string.h>

#pragma pack(1)


typedef void (*RegCallFun)(void*);

// now we just deal x86 version
// struct FunEnvInfo
typedef struct _FunEnvInfo
{
    _FunEnvInfo()
    {
        memset(this,0,sizeof(_FunEnvInfo));
    }
    int reEIP; // 0
    int reESP; // 4
    int reEBP; // 8
    int reEAX; // 12
    int reEBX; // 16
    int reECX; // 20
    int reEDX; // 24
    int reESI; // 28
    int reEDI; // 32
    int reEFLAG; // 36
    int iStackSize; // 40
    void* pStackMem; // 44
    RegCallFun pfCallFun; // 48
}FunEnvInfo,*FunEnvInfoPtr;


#pragma pack()

#endif