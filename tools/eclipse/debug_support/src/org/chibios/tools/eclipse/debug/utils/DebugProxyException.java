package org.chibios.tools.eclipse.debug.utils;

public class DebugProxyException extends Exception {

  private static final long serialVersionUID = 6860700758297226746L;

  public DebugProxyException() {
    super("Debug Proxy Exception");
  }

  public DebugProxyException(String s) {
    super(s);
  }
}
