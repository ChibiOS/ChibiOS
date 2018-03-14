#!/bin/bash
if [ $# -eq 0 ]
then
  find ../.. -name "chconf.h" -exec bash update_chconf_rt.sh "{}" \;
elif [ $# -eq 1 ]
then
  if egrep -q "_CHIBIOS_RT_CONF_" $1
  then
    echo Processing: $1
    cat $1 | egrep -e "\#define\s+[a-zA-Z0-9_]*\s+[a-zA-Z0-9_]" | cut --bytes=9- - | sed 's/  */=/g' > ./values.txt
    if ! fmpp -q -C chconf_rt.fmpp
    then
      echo
      echo "aborted"
      exit 1
    fi
    cp ./chconf.h $1
    rm ./chconf.h ./values.txt
  fi
else
  echo "illegal number of arguments"
fi
