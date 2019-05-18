#!/bin/bash
find ../../os/nil -name "*.[ch]" -exec perl stylecheck.pl "{}" \;

