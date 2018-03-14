#!/bin/bash
if [ $# -eq 0 ]
then
  find ../.. -name "halconf.h" -exec bash update_halconf.sh "{}" \;
elif [ $# -eq 1 ]
then
#  if egrep -q "" $1
#  then
    echo Processing: $1
    cat $1 | egrep -e "\#define\s+[a-zA-Z0-9_]*\s+[a-zA-Z0-9_]" | cut --bytes=9- - | sed 's/  */=/g' > ./values.txt
    if ! fmpp -q -C halconf.fmpp
    then
      echo
      echo "aborted"
      exit 1
    fi
    cp ./halconf.h $1
    rm ./halconf.h ./values.txt
#  fi
else
  echo "illegal number of arguments"
fi
