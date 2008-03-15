*****************************************************************************
** ChibiOS/RT port for x86 into a Win32 process                            **
*****************************************************************************

** TARGET **

The demo runs under any Windows version as an application program. The serial
I/O is simulated over TCP/IP sockets.

** The Demo **

The demo listens on the two serial ports, when a connection is detected a
thread is started that serves a small command shell.
The demo shows how create/terminate threads at runtime, how listen to events,
how ho work with serial ports, how use the messages.
You can develop your ChibiOS/RT application using this demo as a simulator
then you can recompile it for a different architecture.
See demo.c for details.

** Build Procedure **

The demo was built using the MinGW toolchain.
