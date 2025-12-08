#!/bin/bash
find ../../os/ex -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
