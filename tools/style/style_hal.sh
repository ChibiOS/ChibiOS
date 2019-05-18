#!/bin/bash
find ../../os/hal/include -name "*.[ch]" -exec perl stylecheck.pl "{}" \;
find ../../os/hal/src -name "*.[ch]" -exec perl stylecheck.pl "{}" \;
find ../../os/hal/templates -name "*.[ch]" -exec perl stylecheck.pl "{}" \;

