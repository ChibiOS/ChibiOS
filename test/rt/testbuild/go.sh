#!/bin/bash
export XOPT XDEFS

XOPT="-ggdb -O0 -fomit-frame-pointer -DDELAY_BETWEEN_TESTS=0 -fprofile-arcs -ftest-coverage"
XDEFS=""

function clean() {
  echo -n "  * Cleaning..."
  make clean > /dev/null
  echo "OK"
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
  echo "OK"
}

function coverage() {
  echo -n "  * Coverage..."
  mkdir coverage    2> /dev/null
  mkdir coverage/$1 2> /dev/null
  echo "Configuration $2" > coverage/$1.txt
  echo "----------------------------------------------------------------" >> coverage/$1.txt
  if ! make gcov >> coverage/$1.txt 2> /dev/null
  then
    echo "failed"
    clean
    exit
  fi
  mv -f *.gcov ./coverage/$1
  echo "OK"
}

function misra() {
  echo -n "  * Analysing..."
  if ! make misra > misralog.txt
  then
    echo "failed"
    clean
    exit
  fi
  echo "OK"
}

function test() {
  if [ -z "$2" ]
  then
    msg=$1": Default Settings"
    XDEFS=
  else
    msg=$1": "$2
    XDEFS=$2
  fi
  echo $msg
  compile
  execute_test
  coverage $1 "$msg"
  misra
  clean
}

function partial() {
  compile
  execute_test
  misra
  clean
}

test cfg1 ""
test cfg2 "-DCH_CFG_OPTIMIZE_SPEED=FALSE"
test cfg3 "-DCH_CFG_TIME_QUANTUM=0"
test cfg4 "-DCH_CFG_USE_REGISTRY=FALSE"
test cfg5 "-DCH_CFG_USE_SEMAPHORES=FALSE -DCH_CFG_USE_MAILBOXES=FALSE"
test cfg6 "-DCH_CFG_USE_SEMAPHORES_PRIORITY=TRUE"
test cfg7 "-DCH_CFG_USE_MUTEXES=FALSE -DCH_CFG_USE_CONDVARS=FALSE"
test cfg8 "-DCH_CFG_USE_MUTEXES_RECURSIVE=TRUE"
test cfg9 "-DCH_CFG_USE_CONDVARS=FALSE"
test cfg10 "-DCH_CFG_USE_CONDVARS_TIMEOUT=FALSE"
test cfg11 "-DCH_CFG_USE_EVENTS=FALSE"
test cfg12 "-DCH_CFG_USE_EVENTS_TIMEOUT=FALSE"
test cfg13 "-DCH_CFG_USE_MESSAGES=FALSE"
test cfg14 "-DCH_CFG_USE_MESSAGES_PRIORITY=TRUE"
test cfg15 "-DCH_CFG_USE_MAILBOXES=FALSE"
test cfg16 "-DCH_CFG_USE_MEMCORE=FALSE -DCH_CFG_USE_MEMPOOLS=FALSE -DCH_CFG_USE_HEAP=FALSE -DCH_CFG_USE_DYNAMIC=FALSE"
test cfg17 "-DCH_CFG_USE_MEMPOOLS=FALSE -DCH_CFG_USE_HEAP=FALSE -DCH_CFG_USE_DYNAMIC=FALSE"
test cfg18 "-DCH_CFG_USE_MEMPOOLS=FALSE"
test cfg19 "-DCH_CFG_USE_HEAP=FALSE"
test cfg20 "-DCH_CFG_USE_DYNAMIC=FALSE"
#test cfg21 "-DCH_DBG_STATISTICS=TRUE"
test cfg22 "-DCH_DBG_SYSTEM_STATE_CHECK=TRUE"
test cfg23 "-DCH_DBG_ENABLE_CHECKS=TRUE"
test cfg24 "-DCH_DBG_ENABLE_ASSERTS=TRUE"
test cfg25 "-DCH_DBG_ENABLE_TRACE=TRUE"
#test cfg26 "-DCH_DBG_ENABLE_STACK_CHECK=TRUE"
test cfg27 "-DCH_DBG_FILL_THREADS=TRUE"
test cfg28 "-DCH_DBG_THREADS_PROFILING=FALSE"
test cfg29 "-DCH_DBG_SYSTEM_STATE_CHECK=TRUE -DCH_DBG_ENABLE_CHECKS=TRUE -DCH_DBG_ENABLE_ASSERTS=TRUE -DCH_DBG_ENABLE_TRACE=TRUE -DCH_DBG_FILL_THREADS=TRUE"

rm *log.txt 2> /dev/null
