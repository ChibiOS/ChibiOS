#include <stdio.h>
#include <errno.h>

#include "osal.h"

#include "syscalls_cpp.hpp"

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((used))
void _exit(int status){
   (void) status;
   osalSysHalt("Unrealized");
   while(TRUE){}
}

__attribute__((used))
pid_t _getpid(void){
   return 1;
}

#undef errno
extern int errno;
__attribute__((used))
int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

__attribute__((used))
void _open_r(void){
  return;
}

void __cxa_pure_virtual() {
  osalSysHalt("Pure virtual function call.");
}

#ifdef __cplusplus
}
#endif
