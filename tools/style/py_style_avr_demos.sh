#!/bin/bash
find ../../demos/AVR -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
