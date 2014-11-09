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
  echo "OK"
  clean
}

echo "Default maximum settings"
compile
execute_test

echo "CH_CFG_OPTIMIZE_SPEED=FALSE"
XDEFS=-DCH_CFG_OPTIMIZE_SPEED=FALSE
compile
execute_test

echo "CH_CFG_TIME_QUANTUM=0"
XDEFS=-DCH_CFG_TIME_QUANTUM=0
compile
execute_test

echo "CH_CFG_USE_REGISTRY=FALSE"
XDEFS=-DCH_CFG_USE_REGISTRY=FALSE
compile
execute_test

echo "CH_CFG_USE_SEMAPHORES=FALSE CH_CFG_USE_MAILBOXES=FALSE"
XDEFS="-DCH_CFG_USE_SEMAPHORES=FALSE -DCH_CFG_USE_MAILBOXES=FALSE"
compile
execute_test

echo "CH_CFG_USE_SEMAPHORES_PRIORITY=TRUE"
XDEFS=-DCH_CFG_USE_SEMAPHORES_PRIORITY=TRUE
compile
execute_test

echo "CH_CFG_USE_MUTEXES=FALSE CH_CFG_USE_CONDVARS=FALSE"
XDEFS="-DCH_CFG_USE_MUTEXES=FALSE -DCH_CFG_USE_CONDVARS=FALSE"
compile
execute_test

echo "CH_CFG_USE_MUTEXES_RECURSIVE=TRUE"
XDEFS=-DCH_CFG_USE_MUTEXES_RECURSIVE=TRUE
compile
execute_test

echo "CH_CFG_USE_CONDVARS=FALSE"
XDEFS=-DCH_CFG_USE_CONDVARS=FALSE
compile
execute_test

echo "CH_CFG_USE_CONDVARS_TIMEOUT=FALSE"
XDEFS=-DCH_CFG_USE_CONDVARS_TIMEOUT=FALSE
compile
execute_test

echo "CH_CFG_USE_EVENTS=FALSE"
XDEFS=-DCH_CFG_USE_EVENTS=FALSE
compile
execute_test

echo "CH_CFG_USE_EVENTS_TIMEOUT=FALSE"
XDEFS=-DCH_CFG_USE_EVENTS_TIMEOUT=FALSE
compile
execute_test

