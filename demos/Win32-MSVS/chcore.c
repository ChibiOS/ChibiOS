#include <windows.h>
#include <stdio.h>

#undef CDECL

#include <ch.h>

static LARGE_INTEGER nextcnt;
static LARGE_INTEGER slice;

void InitSimCom1(void);
void InitSimCom2(void);
BOOL Com1ConnInterruptSimCom(void);
BOOL Com2ConnInterruptSimCom(void);
BOOL Com1InInterruptSimCom(void);
BOOL Com2InInterruptSimCom(void);
BOOL Com1OutInterruptSimCom(void);
BOOL Com2OutInterruptSimCom(void);

/*
 * Simulated HW initialization.
 */
void InitCore(void) {
  WSADATA wsaData;

  // Initialization.
  if (WSAStartup(2, &wsaData) != 0) {
    printf("Unable to locate a winsock DLL\n");
    exit(1);
  }

  printf("Win32 ChibiOS/RT simulator\n\n");
  printf("Thread structure %d bytes\n", sizeof(Thread));
  if (!QueryPerformanceFrequency(&slice)) {
    printf("QueryPerformanceFrequency() error");
    exit(1);
  }
  printf("Core Frequency   %u Hz\n", slice.LowPart);
  slice.QuadPart /= CH_FREQUENCY;
  QueryPerformanceCounter(&nextcnt);
  nextcnt.QuadPart += slice.QuadPart;

  InitSimCom1();
  InitSimCom2();
}

/*
 * Interrupt simulation.
 */
static void ChkIntSources(void) {
  LARGE_INTEGER n;

  if (Com1InInterruptSimCom()   || Com2InInterruptSimCom()  ||
      Com1OutInterruptSimCom()  || Com2OutInterruptSimCom() ||
      Com1ConnInterruptSimCom() || Com2ConnInterruptSimCom()) {
    chSchRescheduleI();
    return;
  }

  // Interrupt Timer simulation (10ms interval).
  QueryPerformanceCounter(&n);
  if (n.QuadPart > nextcnt.QuadPart) {
    nextcnt.QuadPart += slice.QuadPart;
    chSchTimerHandlerI();
    if (chSchRescRequiredI())
      chSchDoRescheduleI();
  }
}

void __fastcall chSysPause(void) {

  chThdSetPriority(IDLEPRIO);

  while (TRUE) {

    ChkIntSources();
    Sleep(0);
  }
}

__declspec(naked) void __fastcall chSysHalt(void) {

  exit(2);
}

__declspec(naked) void __fastcall chSysSwitchI(Context *oldp, Context *newp) {

  __asm {
    // Switch out code
    push    ebp
    push    esi
    push    edi
    push    ebx
    mov     dword ptr [ecx],esp
    // Switch in code
    mov     esp,[edx]
    pop     ebx
    pop     edi
    pop     esi
    pop     ebp
    ret
  }
}

__declspec(naked) void __fastcall threadexit(void) {

  __asm {
//    add     esp,4                 ; The thread parameter
    push    eax                   ; The exit code returned by the thread
    call    chThdExit
    add     esp,4
    call    chSysHalt             ; Should *never* happen
  }
}
