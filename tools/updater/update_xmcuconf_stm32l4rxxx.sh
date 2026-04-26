#!/bin/bash
if [ $# -eq 2 ]
  then
  if [ $1 = "rootpath" ]
  then
    find $2 -name "xmcuconf.h" -exec bash update_xmcuconf_stm32l4rxxx.sh "{}" \;
  else
    echo "Usage: update_xmcuconf_stm32l4rxxx.sh [rootpath <root path>]"
  fi
elif [ $# -eq 1 ]
then
  declare conffile=$(<$1)
  if egrep -q "STM32L4P5_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4Q5_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4R5_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4S5_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4R7_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4S7_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4R9_XMCUCONF" <<< "$conffile" || egrep -q "STM32L4S9_XMCUCONF" <<< "$conffile"
  then
    echo Processing: $1
    egrep -e "\#define\s+[a-zA-Z0-9_()]*\s+[^\s]" <<< "$conffile" | sed -r 's/\#define\s+([a-zA-Z0-9_]*)(\([^)]*\))?\s+/\1=/g' > ./values.txt
    if ! fmpp -q -C conf.fmpp -S ../ftl/processors/conf/xmcuconf_stm32l4rxxx
    then
      echo
      echo "aborted"
      exit 1
    fi
    cp ./xmcuconf.h $1
    rm ./xmcuconf.h ./values.txt
  fi
else
 echo "Usage: update_xmcuconf_stm32l4rxxx.sh [rootpath <root path>]"
 echo "       update_xmcuconf_stm32l4rxxx.sh <configuration file>]"
fi
