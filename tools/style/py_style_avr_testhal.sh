#!/bin/bash
find ../../testhal/AVR -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
