The launch configurations under:

  ./debug/*.launch

Contain hardcoded paths prefixed like:

  /home/giovanni/Projects/ChibiStudio/chibios_trunk/demos/various/SB-CLIENT-128k-08040000-4k-2001E000/build/sb.elf

Replace the initial part up to "chibios_trunk" with the path of your ChibiOS
directory before attempting a debug session using Eclipse. In addition, you
need also to compile the "SB-CLIENT-128k-08040000-16k-20018000" and
"SB-CLIENT-128k-08060000-16k-2001C000" projects, their image (sb.elf) is
 loaded inside the demo sandboxes.
