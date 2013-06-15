#!/bin/bash
JAVA_HOME=/usr/lib/jvm/java-6-sun
export JAVA_HOME
PATH=$PATH:$JAVA_HOME/bin
export PATH
fmpp -C config.fmpp

