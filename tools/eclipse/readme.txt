This folder contains ChibiOS/RT related Eclipse extensions:

- org.chibios.tools.eclipse.debug_x.y.z.jar, debugger extension that allows to
  inspect the ChibiOS/RT internal data structures at debug time. This plugin
  is best used when the kernel debug options are turned on.
  Installation:
  - Copy the plugin under [eclipse]/plugins.
  - Start Eclipse and enable the plugin from:
      Window->Show View->Others...->ChibiOS/RT
  - Make sure that Eclipse is using the "Standard Debugger" from:
      Window->Preferences->Run/Debug->Launching->Default Launchers->
      GDB Hardware Debugging->[Debug]
  - Start a debug session of a ChibiOS/RT demo or application and step until
    after chSysInit() has been invoked.
  - Inspect the various structures (Timers, Threads etc) using the ChibiOS/RT
    view by pressing the yellow refresh button.
