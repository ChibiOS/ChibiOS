The launch configurations under:

  ./debug/*.launch

Contain hardcoded paths prefixed like:

  /home/giovanni/Projects/ChibiStudio/chibios_trunk/demos/various/SB-CLIENT-32k-08070000-4k-2001E000/build/sb.elf

Replace the initial part up to "chibios_trunk" with the path of your ChibiOS
directory before attempting a debug session using Eclipse. In addition, you
need also to compile the "SB-CLIENT-32k-08070000-4k-2001E000" project, its
image (sb.elf) is loaded inside the demo sandbox.
