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
