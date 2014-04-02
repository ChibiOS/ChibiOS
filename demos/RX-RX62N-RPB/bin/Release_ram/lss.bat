@echo off
set path="d:\Program Files\KPIT\GNURX\rx-elf\rx-elf\bin"
rx-elf-objdump -h -S rx62n_demo.exe >rx62n_demo.lss
