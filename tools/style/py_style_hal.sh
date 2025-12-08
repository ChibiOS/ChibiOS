#!/bin/bash
find ../../os/hal/include -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
find ../../os/hal/src -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
find ../../os/hal/templates -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
find ../../os/hal/osal -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
find ../../os/hal/ports/STM32 -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
