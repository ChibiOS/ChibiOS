#!/bin/bash
find ../../os/nil -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
find ../../os/oslib -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
find ../../os/license -name "*.[ch]" -exec python3 stylecheck.py "{}" \;
