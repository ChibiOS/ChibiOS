#!/bin/bash
export XOPT XDEFS

XOPT="-ggdb -O2 -fomit-frame-pointer -DDELAY_BETWEEN_TESTS=0"
XDEFS=""

function clean() {
  make clean > /dev/null
}

function compile() {
  echo -n "  * Building..."
  if ! make > buildlog.txt
  then
    echo "failed"
    clean
    exit
  fi
  echo "OK"
}

function execute_test() {
  echo -n "  * Testing..."
  if ! ./ch > testlog.txt
  then
    echo "failed"
    clean
    exit
  fi
  clean
  echo "OK"
}

function misra() {
  echo -n "  * Analysing..."
  if ! make misra > misralog.txt
  then
    echo "failed"
    exit
  fi
  echo "OK"
}

echo "Default maximum settings"
compile
execute_test
misra

echo "CH_CFG_OPTIMIZE_SPEED=FALSE"
XDEFS=-DCH_CFG_OPTIMIZE_SPEED=FALSE
compile
execute_test
misra

echo "CH_CFG_TIME_QUANTUM=0"
XDEFS=-DCH_CFG_TIME_QUANTUM=0
compile
execute_test
misra

echo "CH_CFG_USE_REGISTRY=FALSE"
XDEFS=-DCH_CFG_USE_REGISTRY=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_SEMAPHORES=FALSE CH_CFG_USE_MAILBOXES=FALSE"
XDEFS="-DCH_CFG_USE_SEMAPHORES=FALSE -DCH_CFG_USE_MAILBOXES=FALSE"
compile
execute_test
misra

echo "CH_CFG_USE_SEMAPHORES_PRIORITY=TRUE"
XDEFS=-DCH_CFG_USE_SEMAPHORES_PRIORITY=TRUE
compile
execute_test
misra

echo "CH_CFG_USE_MUTEXES=FALSE CH_CFG_USE_CONDVARS=FALSE"
XDEFS="-DCH_CFG_USE_MUTEXES=FALSE -DCH_CFG_USE_CONDVARS=FALSE"
compile
execute_test
misra

echo "CH_CFG_USE_MUTEXES_RECURSIVE=TRUE"
XDEFS=-DCH_CFG_USE_MUTEXES_RECURSIVE=TRUE
compile
execute_test
misra

echo "CH_CFG_USE_CONDVARS=FALSE"
XDEFS=-DCH_CFG_USE_CONDVARS=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_CONDVARS_TIMEOUT=FALSE"
XDEFS=-DCH_CFG_USE_CONDVARS_TIMEOUT=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_EVENTS=FALSE"
XDEFS=-DCH_CFG_USE_EVENTS=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_EVENTS_TIMEOUT=FALSE"
XDEFS=-DCH_CFG_USE_EVENTS_TIMEOUT=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_MESSAGES=FALSE"
XDEFS=-DCH_CFG_USE_MESSAGES=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_MESSAGES_PRIORITY=TRUE"
XDEFS=-DCH_CFG_USE_MESSAGES_PRIORITY=TRUE
compile
execute_test
misra

echo "CH_CFG_USE_MAILBOXES=FALSE"
XDEFS=-DCH_CFG_USE_MAILBOXES=FALSE
compile
execute_test
misra

echo "CH_CFG_USE_MEMCORE=FALSE CH_CFG_USE_MEMPOOLS=FALSE CH_CFG_USE_HEAP=FALSE CH_CFG_USE_DYNAMIC=FALSE"
XDEFS="-DCH_CFG_USE_MEMCORE=FALSE -DCH_CFG_USE_MEMPOOLS=FALSE -DCH_CFG_USE_HEAP=FALSE -DCH_CFG_USE_DYNAMIC=FALSE"
compile
execute_test
misra

echo "CH_CFG_USE_MEMPOOLS=FALSE CH_CFG_USE_HEAP=FALSE CH_CFG_USE_DYNAMIC=FALSE"
XDEFS="-DCH_CFG_USE_MEMPOOLS=FALSE -DCH_CFG_USE_HEAP=FALSE -DCH_CFG_USE_DYNAMIC=FALSE"
compile
execute_test
misra

echo "CH_CFG_USE_MEMPOOLS=FALSE"
XDEFS="-DCH_CFG_USE_MEMPOOLS=FALSE"
compile
execute_test
misra

echo "CH_CFG_USE_HEAP=FALSE"
XDEFS="-DCH_CFG_USE_HEAP=FALSE"
compile
execute_test
misra

echo "CH_CFG_USE_DYNAMIC=FALSE"
XDEFS=-DCH_CFG_USE_DYNAMIC=FALSE
compile
execute_test
misra

#echo "CH_DBG_STATISTICS=TRUE"
#XDEFS=-DCH_DBG_STATISTICS=TRUE
#compile
#execute_test
#misra

echo "CH_DBG_SYSTEM_STATE_CHECK=TRUE"
XDEFS=-DCH_DBG_SYSTEM_STATE_CHECK=TRUE
compile
execute_test
misra

echo "CH_DBG_ENABLE_CHECKS=TRUE"
XDEFS=-DCH_DBG_ENABLE_CHECKS=TRUE
compile
execute_test
misra

echo "CH_DBG_ENABLE_ASSERTS=TRUE"
XDEFS=-DCH_DBG_ENABLE_ASSERTS=TRUE
compile
execute_test
misra

echo "CH_DBG_ENABLE_TRACE=TRUE"
XDEFS=-DCH_DBG_ENABLE_TRACE=TRUE
compile
execute_test
misra

#echo "CH_DBG_ENABLE_STACK_CHECK=TRUE"
#XDEFS=-DCH_DBG_ENABLE_STACK_CHECK=TRUE
#compile
#execute_test
#misra

echo "CH_DBG_FILL_THREADS=TRUE"
XDEFS=-DCH_DBG_FILL_THREADS=TRUE
compile
execute_test
misra

echo "CH_DBG_THREADS_PROFILING=FALSE"
XDEFS=-DCH_DBG_THREADS_PROFILING=FALSE
compile
execute_test
misra

echo "CH_DBG_SYSTEM_STATE_CHECK=TRUE CH_DBG_ENABLE_CHECKS=TRUE CH_DBG_ENABLE_ASSERTS=TRUE CH_DBG_ENABLE_TRACE=TRUE CH_DBG_FILL_THREADS=TRUE"
XDEFS="-DCH_DBG_SYSTEM_STATE_CHECK=TRUE -DCH_DBG_ENABLE_CHECKS=TRUE -DCH_DBG_ENABLE_ASSERTS=TRUE -DCH_DBG_ENABLE_TRACE=TRUE -DCH_DBG_FILL_THREADS=TRUE"
compile
execute_test
misra
