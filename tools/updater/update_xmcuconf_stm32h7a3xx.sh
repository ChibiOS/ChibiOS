#!/bin/bash
if [ $# -eq 2 ]
  then
  if [ $1 = "rootpath" ]
  then
    find $2 -name "xmcuconf.h" -exec bash update_xmcuconf_stm32h7a3xx.sh "{}" \;
  else
    echo "Usage: update_xmcuconf_stm32h7a3xx.sh [rootpath <root path>]"
  fi
elif [ $# -eq 1 ]
then
  declare conffile=$(<$1)
  if egrep -q "STM32H7A3_XMCUCONF" <<< "$conffile" || egrep -q "STM32H7B3_XMCUCONF" <<< "$conffile" || egrep -q "STM32H7A3_Q_XMCUCONF" <<< "$conffile" || egrep -q "STM32H7B3_Q_XMCUCONF" <<< "$conffile"
  then
    echo Processing: $1
    egrep -e "\#define\s+[a-zA-Z0-9_()]*\s+[^\s]" <<< "$conffile" | sed -r 's/\#define\s+([a-zA-Z0-9_]*)(\([^)]*\))?\s+/\1=/g' > ./values.txt
    if ! fmpp -q -C conf.fmpp -S ../ftl/processors/conf/xmcuconf_stm32h7a3xx
    then
      echo
      echo "aborted"
      exit 1
    fi
    cp ./xmcuconf.h $1
    rm ./xmcuconf.h ./values.txt
  fi
else
 echo "Usage: update_xmcuconf_stm32h7a3xx.sh [rootpath <root path>]"
 echo "       update_xmcuconf_stm32h7a3xx.sh <configuration file>]"
fi
