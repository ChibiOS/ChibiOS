#include <stdio.h>
#include <errno.h>

#include "osal.h"

#include "syscalls_cpp.hpp"

/**
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
void _exit(int status){
   (void) status;
   osalSysHalt("Unrealized");
   while(TRUE){}
}
#ifdef __cplusplus
}
#endif

/**
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
pid_t _getpid(void){
   return 1;
}
#ifdef __cplusplus
}
#endif

/**
 *
 */
#undef errno
extern int errno;
#ifdef __cplusplus
extern "C" {
#endif
int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}
#ifdef __cplusplus
}
#endif

/**
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
void _open_r(void){
  return;
}
#ifdef __cplusplus
}
#endif

/**
 *
 */
#ifdef __cplusplus
extern "C" {
#endif
  void __cxa_pure_virtual() {
    osalSysHalt("Pure virtual function call.");
  }
#ifdef __cplusplus
}
#endif
