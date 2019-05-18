#!/bin/bash
find ../../os/rt -name "*.[ch]" -exec perl stylecheck.pl "{}" \;

