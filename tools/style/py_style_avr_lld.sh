#!/bin/bash
find ../../os/hal/ports/AVR/ -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
