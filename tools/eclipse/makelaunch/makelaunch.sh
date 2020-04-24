#!/bin/bash
if [ $# -ne 3 ]
then
  echo "Usage: makelaunch.sh <project directory path> <elf file path> <\"Eclipse Project Name\">"
  exit 1
else
  DEBUGDIR=debug
  ELFRELPATH=$(realpath --relative-to=$1 $2)
  echo "Creating debug launch configurations for \"$ELFRELPATH\" ..."
  ELFNAME=$(echo "$ELFRELPATH" | sed -r "s|/|:|g")
  LAUNCHNAME1="$3 ($ELFNAME)(OpenOCD, Flash and Run)"
  LAUNCHNAME2="$3 ($ELFNAME)(OpenOCD, Just Run)"
  mkdir -p $1/$DEBUGDIR
  cat ./arm_flash-and-run.launch | sed -r "s|EEEEEEEE|$2|g" | sed -r "s|PPPPPPPP|$3|g" > "$1/$DEBUGDIR/$LAUNCHNAME1.launch"
  cat ./arm_just-run.launch | sed -r "s|EEEEEEEE|$2|g" | sed -r "s|PPPPPPPP|$3|g" > "$1/$DEBUGDIR/$LAUNCHNAME2.launch"
  echo
  echo "Done"
fi
exit 0

