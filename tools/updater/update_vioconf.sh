#!/bin/bash
if [ $# -eq 2 ]
  then
  if [ $1 = "rootpath" ]
  then
    find $2 -name "vioconf.h" -exec bash update_vioconf.sh "{}" \;
  else
    echo "Usage: update_vioconf.sh [rootpath <root path>]"
  fi
elif [ $# -eq 1 ]
then
  declare conffile=$(<$1)
  if egrep -q "__CHIBIOS_VIO_CONF__" <<< "$conffile"
  then
    echo Processing: $1
    egrep -e "\#define\s+[a-zA-Z0-9_()]*\s+[^\s]" <<< "$conffile" | sed -r 's/\#define\s+([a-zA-Z0-9_]*)(\([^)]*\))?\s+/\1=/g' > ./values.txt
    if ! fmpp -q -C conf.fmpp -S ../ftl/processors/conf/vioconf
    then
      echo
      echo "aborted"
      exit 1
    fi
    cp ./vioconf.h $1
    rm ./vioconf.h ./values.txt
  fi
else
 echo "Usage: update_vioconf.sh [rootpath <root path>]"
 echo "       update_vioconf.sh <configuration file>]"
fi
