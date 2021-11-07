#!/bin/bash
rm ../../manuals/html/hal_stm32f3xx/*
doxygen Doxyfile_html
rm ../../manuals/html/hal_stm32f3xx/*.md5
rm ../../manuals/html/hal_stm32f3xx/*.map

