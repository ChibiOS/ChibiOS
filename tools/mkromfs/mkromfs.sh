#!/bin/sh

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)

exec python3 "$SCRIPT_DIR/mkromfs.py" "$@"
