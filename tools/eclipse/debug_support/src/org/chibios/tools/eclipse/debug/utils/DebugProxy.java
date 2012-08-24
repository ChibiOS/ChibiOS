package org.chibios.tools.eclipse.debug.utils;

import java.util.HashMap;
import java.util.LinkedHashMap;

import org.eclipse.cdt.debug.core.cdi.model.ICDITarget;
import org.eclipse.cdt.debug.internal.core.model.CDebugTarget;
import org.eclipse.cdt.debug.mi.core.MIException;
import org.eclipse.cdt.debug.mi.core.MIFormat;
import org.eclipse.cdt.debug.mi.core.MISession;
import org.eclipse.cdt.debug.mi.core.cdi.model.Target;
import org.eclipse.cdt.debug.mi.core.command.CommandFactory;
import org.eclipse.cdt.debug.mi.core.command.MIDataEvaluateExpression;
import org.eclipse.cdt.debug.mi.core.command.MIDataReadMemory;
import org.eclipse.cdt.debug.mi.core.output.MIDataEvaluateExpressionInfo;
import org.eclipse.cdt.debug.mi.core.output.MIDataReadMemoryInfo;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.model.IDebugTarget;

@SuppressWarnings("restriction")
public class DebugProxy {

  private CommandFactory cmd_factory;
  private MISession mi_session;

  protected final static String[] threadStates = {
    "READY",
    "CURRENT",
    "SUSPENDED",
    "WTSEM",
    "WTMTX",
    "WTCOND",
    "SLEEPING",
    "WTEXIT",
    "WTOREVT",
    "WTANDEVT",
    "SNDMSGQ",
    "SNDMSG",
    "WTMSG",
    "WTQUEUE",
    "FINAL"
  };

  private void getSession(CDebugTarget target)
      throws DebugProxyException {
    ICDITarget[] targets = target.getCDISession().getTargets();
    ICDITarget cdi_target = null;
    for (int i = 0; i < targets.length; i++) {
      if (targets[i] instanceof Target) {
        cdi_target = targets[i];
        break;
      }
    }
    if (cdi_target == null)
      throw new DebugProxyException("no CDI session found");
    mi_session = ((Target)cdi_target).getMISession();
    cmd_factory = mi_session.getCommandFactory();
  }

  public DebugProxy()
      throws DebugProxyException {
    IDebugTarget[] targets = DebugPlugin.getDefault().getLaunchManager().getDebugTargets();
    for (IDebugTarget target:targets) {
      if(target instanceof CDebugTarget) {
        getSession((CDebugTarget)target);
        return;
      }
    }
  }

  public DebugProxy(CDebugTarget target)
      throws DebugProxyException {
    getSession(target);
  }

  public String evaluateExpression(String expression)
      throws DebugProxyException {
    if (mi_session.getMIInferior().isRunning())
      return null;
    MIDataEvaluateExpression expr = cmd_factory.createMIDataEvaluateExpression(expression);
    try {
      mi_session.postCommand(expr);
      MIDataEvaluateExpressionInfo info = expr.getMIDataEvaluateExpressionInfo();
      if (info != null)
        return info.getExpression();
    } catch (MIException e) {}
    throw new DebugProxyException("error evaluating the expression: '" +
                                  expression + "'");
  }

  public long scanStack(long base, long end, long pattern)
      throws DebugProxyException {
    if (mi_session.getMIInferior().isRunning())
      return -1;
    if (end > base) {
      MIDataReadMemory mem = cmd_factory.createMIDataReadMemory(0,
          Long.toString(base),
          MIFormat.HEXADECIMAL,
          4,
          1,
          (int)(end - base),
          '.');
      try {
        mi_session.postCommand(mem);
         MIDataReadMemoryInfo info = mem.getMIDataReadMemoryInfo();
         if (info != null) {
           long[] data = info.getMemories()[0].getData();
           int i = 0;
           while ((i < data.length) && (data[i] == pattern))
               i++;
           return i * 4;
         }
      } catch (MIException e) {}
      throw new DebugProxyException("error reading memory at " +
          base);
    }
    return 0;
  }

  public String readCString(long address, int max)
      throws DebugProxyException {
    if (mi_session.getMIInferior().isRunning())
      return null;
    MIDataReadMemory mem = cmd_factory.createMIDataReadMemory(0,
                                                              Long.toString(address),
                                                              MIFormat.HEXADECIMAL,
                                                              1,
                                                              1,
                                                              max,
                                                              '.');
    try {
      mi_session.postCommand(mem);
       MIDataReadMemoryInfo info = mem.getMIDataReadMemoryInfo();
       if (info != null) {
          String s = info.getMemories()[0].getAscii();
          int i = s.indexOf('.');
          if (i >= 0)
            return s.substring(0, s.indexOf('.'));
          else
            return s;
       }
    } catch (MIException e) {}
    throw new DebugProxyException("error reading memory at " +
        address);
  }

  /**
   * @brief   Return the list of threads.
   * @details The threads list is fetched from memory by scanning the
   *          registry.
   *
   * @return  A @p LinkedHashMap object whose keys are the threads addresses
   *          as decimal strings, the value is an @p HashMap of the thread
   *          fields:
   *          - stack
   *          - stklimit
   *          - name
   *          - state
   *          - state_s
   *          - flags
   *          - prio
   *          - refs
   *          - time
   *          - wtobjp
   *          .
   *          Missing fields are set to "-".
   * @retval null                   If the debugger encountered an error or
   *                                the target is running.
   *
   * @throws DebugProxyException    If the debugger is active but the registry
   *                                is not found, not initialized or corrupted.
   */
  public LinkedHashMap<String, HashMap<String, String>> readThreads()
      throws DebugProxyException {
    // rlist structure address.
    String rlist;
    try {
      rlist = evaluateExpression("(uint32_t)&rlist");
      if (rlist == null)
        return null;
    } catch (DebugProxyException e) {
      throw new DebugProxyException("ChibiOS/RT not found on target");
    } catch (Exception e) {
      return null;
    }

    // Scanning registry.
    LinkedHashMap<String, HashMap<String, String>> lhm =
        new LinkedHashMap<String, HashMap<String, String>>(10);
    String current = rlist;
    String previous = rlist;
    while (true) {
      
      // Fetching next thread in the registry (newer link). This fetch fails
      // if the register is not enabled in the kernel and the p_newer field
      // does not exist.
      try {
        current = evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_newer");
      } catch (DebugProxyException e1) {
        throw new DebugProxyException("ChibiOS/RT registry not enabled in kernel");
      }

      // This can happen if the kernel is not initialized yet or if the
      // registry is corrupted.
      if (current.compareTo("0") == 0)
        throw new DebugProxyException("ChibiOS/RT registry integrity check failed, NULL pointer");

      // TODO: integrity check on the pointer value (alignment, range).

      // The previous thread in the list is fetched as a integrity check.
      String older = evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_older");
      if (older.compareTo("0") == 0)
        throw new DebugProxyException("ChibiOS/RT registry integrity check failed, NULL pointer");
      if (previous.compareTo(older) != 0)
        throw new DebugProxyException("ChibiOS/RT registry integrity check failed, double linked list violation");

      // End of the linked list condition.
      if (current.compareTo(rlist) == 0)
        break;

      // Hash of threads fields.
      HashMap<String, String> map = new HashMap<String, String>(16);

      // Fetch of the various fields in the Thread structure. Some fields
      // are optional so are placed within try-catch.
      long stklimit;
      try {
    	stklimit = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_stklimit"));
        map.put("stklimit", Long.toString(stklimit));
      } catch (DebugProxyException e) {
        map.put("stklimit", "-");
        stklimit = -1;
      }

      long stack;
      try {
        stack = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_ctx.r13"));
        map.put("stack", Long.toString(stack));
      } catch (DebugProxyException e) {
        try {
          stack = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_ctx.sp"));
          map.put("stack", Long.toString(stack));
        } catch (DebugProxyException ex) {
          map.put("stack", "-");
          stack = -1;
        }
      }

      if ((stklimit < 0) || (stack < 0))
      	map.put("stkunused", "-");
      else {
        if ((stack < 0) || (stack < stklimit))
      	  map.put("stkunused", "overflow");
        else {
          long stkunused = scanStack(stklimit, stack, 0x55555555);
          map.put("stkunused", Long.toString(stkunused));
        }
      }

      long n;
      try {
        n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_name"));
        if (n == 0)
          map.put("name", "<no name>");
        else
          map.put("name", readCString(n, 16));
      } catch (DebugProxyException e) {
        map.put("name", "-");
      }

      n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_state"));
      map.put("state", Long.toString(n));
      if ((n >= 0) && (n < threadStates.length)) {
        map.put("state_s", threadStates[(int)n]);
      }
      else
        map.put("state_s", "unknown");

      n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_flags"));
      map.put("flags", Long.toString(n));

      n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_prio"));
      map.put("prio", Long.toString(n));

      try {
        n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_refs"));
        map.put("refs", Long.toString(n));
      } catch (DebugProxyException e) {
        map.put("refs", "-");
      }

      try {
        n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_time"));
        map.put("time", Long.toString(n));
      } catch (DebugProxyException e) {
        map.put("time", "-");
      }

      try {
        n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + current + ")->p_u.wtobjp"));
        map.put("wtobjp", Long.toString(n));
      } catch (DebugProxyException e) {
        map.put("wtobjp", "-");
      }

      // Inserting the new thread map into the threads list.
      lhm.put(current, map);

      previous = current;
    }
    return lhm;
  }

  /**
   * @brief   Return the list of timers.
   * @details The timers list is fetched from memory by scanning the
   *          @p vtlist structure.
   *
   * @return  A @p LinkedHashMap object whose keys are the timers addresses
   *          as decimal strings, the value is an @p HashMap of the timers
   *          fields:
   *          - delta
   *          - func
   *          - par
   *          .
   * @retval null                   If the debugger encountered an error or
   *                                the target is running.
   *
   * @throws DebugProxyException    If the debugger is active but the structure
   *                                @p vtlist is not found, not initialized or
   *                                corrupted.
   */
  public LinkedHashMap<String, HashMap<String, String>> readTimers()
      throws DebugProxyException {
    // Delta list structure address.
    String vtlist;
    try {
      vtlist = evaluateExpression("(uint32_t)&vtlist");
      if (vtlist == null)
        return null;
    } catch (DebugProxyException e) {
      throw new DebugProxyException("ChibiOS/RT not found on target");
    } catch (Exception e) {
      return null;
    }

    // Scanning delta list.
    LinkedHashMap<String, HashMap<String, String>> lhm =
        new LinkedHashMap<String, HashMap<String, String>>(10);
    String current = vtlist;
    String previous = vtlist;
    while (true) {
      
      // Fetching next timer in the delta list (vt_next link).
      current = evaluateExpression("(uint32_t)((VirtualTimer *)" + current + ")->vt_next");

      // This can happen if the kernel is not initialized yet or if the
      // delta list is corrupted.
      if (current.compareTo("0") == 0)
        throw new DebugProxyException("ChibiOS/RT delta list integrity check failed, NULL pointer");

      // TODO: integrity check on the pointer value (alignment, range).

      // The previous timer in the delta list is fetched as a integrity check.
      String prev = evaluateExpression("(uint32_t)((VirtualTimer *)" + current + ")->vt_prev");
      if (prev.compareTo("0") == 0)
        throw new DebugProxyException("ChibiOS/RT delta list integrity check failed, NULL pointer");
      if (previous.compareTo(prev) != 0)
        throw new DebugProxyException("ChibiOS/RT delta list integrity check failed, double linked list violation");

      // End of the linked list condition.
      if (current.compareTo(vtlist) == 0)
        break;

      // Hash of timers fields.
      HashMap<String, String> map = new HashMap<String, String>(16);

      // Fetch of the various fields in the Thread structure. Some fields
      // are optional so are placed within try-catch.
      long n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((VirtualTimer *)" + current + ")->vt_time"));
      map.put("delta", Long.toString(n));

      n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((VirtualTimer *)" + current + ")->vt_func"));
      map.put("func", Long.toString(n));

      n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((VirtualTimer *)" + current + ")->vt_par"));
      map.put("par", Long.toString(n));

      // Inserting the new thread map into the threads list.
      lhm.put(current, map);

      previous = current;
    }
    return lhm;
  }

  /**
   * @brief   Return the list of trace buffer entries.
   * @details The trace buffer is fetched from memory by scanning the
   *          @p dbg_trace_buffer array.
   *
   * @return  A @p LinkedHashMap object whose keys are the timers addresses
   *          as decimal strings, the value is an @p HashMap of the timers
   *          fields:
   *          - time
   *          - tp
   *          - wtobjp
   *          - state
   *          - state_s
   *          .
   * @retval null                   If the debugger encountered an error or
   *                                the target is running.
   *
   * @throws DebugProxyException    If the debugger is active but the structure
   *                                @p dbg_trace_buffer is not found, not
   *                                initialized or corrupted.
   */
  public LinkedHashMap<String, HashMap<String, String>> readTraceBuffer()
      throws DebugProxyException {
    
    // Trace buffer size.
    String s;
    try {
      s = evaluateExpression("(uint32_t)dbg_trace_buffer.tb_size");
      if (s == null)
        return null;
    } catch (DebugProxyException e) {
      throw new DebugProxyException("trace buffer not found on target");
    } catch (Exception e) {
      return null;
    }

    int tbsize = (int)HexUtils.parseNumber(s);
    int tbrecsize = (int)HexUtils.parseNumber(evaluateExpression("(uint32_t)sizeof (ch_swc_event_t)"));
    long tbstart = HexUtils.parseNumber(evaluateExpression("(uint32_t)dbg_trace_buffer.tb_buffer"));
    long tbend = HexUtils.parseNumber(evaluateExpression("(uint32_t)&dbg_trace_buffer.tb_buffer[" + tbsize + "]"));
    long tbptr = HexUtils.parseNumber(evaluateExpression("(uint32_t)dbg_trace_buffer.tb_ptr"));

    // Scanning the trace buffer from the oldest event to the newest.
    LinkedHashMap<String, HashMap<String, String>> lhm =
        new LinkedHashMap<String, HashMap<String, String>>(64);
    int n = tbsize;
    int i = -tbsize + 1;
    while (n > 0) {
      // Hash of timers fields.
      HashMap<String, String> map = new HashMap<String, String>(16);

      String time = evaluateExpression("(uint32_t)(((ch_swc_event_t *)" + tbptr + ")->se_time)");
      map.put("time", time);

      String tp = evaluateExpression("(uint32_t)(((ch_swc_event_t *)" + tbptr + ")->se_tp)");
      map.put("tp", tp);

      String wtobjp = evaluateExpression("(uint32_t)(((ch_swc_event_t *)" + tbptr + ")->se_wtobjp)");
      map.put("wtobjp", wtobjp);

      long state = HexUtils.parseNumber(evaluateExpression("(uint32_t)(((ch_swc_event_t *)" + tbptr + ")->se_state)"));
      map.put("state", Long.toString(state));
      if ((state >= 0) && (state < threadStates.length))
        map.put("state_s", threadStates[(int)state]);
      else
        map.put("state_s", "unknown");

      // Inserting the new event map into the events list.
      if (tp.compareTo("0") != 0)
        lhm.put(Integer.toString(i), map);

      tbptr += tbrecsize;
      if (tbptr >= tbend)
        tbptr = tbstart;
      n--;
      i++;
    }
    return lhm;
  }
  
  /**
   * @brief   Return the list of the system global variables.
   *
   * @return  A @p LinkedHashMap object whose keys are the variable names and
   *          the values are the variable values.
   *
   * @retval null                   If the debugger encountered an error or
   *                                the target is running.
   *
   * @throws DebugProxyException    If the debugger is active but the structure
   *                                @p dbg_trace_buffer is not found, not
   *                                initialized or corrupted.
   */
  public LinkedHashMap<String, String> readGlobalVariables()
      throws DebugProxyException {

    LinkedHashMap<String, String> map = new LinkedHashMap<String, String>(16);
    
    try {
      String vt_systime = evaluateExpression("(uint32_t)vtlist.vt_systime");
      if (vt_systime == null)
        return null;
      map.put("vt_systime", vt_systime);
    } catch (DebugProxyException e) {
      throw new DebugProxyException("ChibiOS/RT not found on target");
    } catch (Exception e) {
      return null;
    }

    try {
      long r_current = HexUtils.parseNumber(evaluateExpression("(uint32_t)rlist.r_current"));
      if (r_current != 0) {
        String name;
        try {
          long n = HexUtils.parseNumber(evaluateExpression("(uint32_t)((Thread *)" + r_current + ")->p_name"));
          if (n == 0)
            name = "<no name>";
          else
            name = readCString(n, 16);
        } catch (DebugProxyException e) {
          name = "-";
        }
        map.put("r_current", HexUtils.dword2HexString((int)r_current) + " \"" + name + "\"");
      }
      else
        map.put("r_current", "0");
    } catch (DebugProxyException e) {}

    try {
      String r_preempt = evaluateExpression("(uint32_t)rlist.r_preempt");
      map.put("r_preempt", r_preempt);
    } catch (DebugProxyException e) {}

    try {
      Long addr = HexUtils.parseNumber(evaluateExpression("(uint32_t)dbg_panic_msg"));
      if (addr == 0)
        map.put("dbg_panic_msg", "<NULL>");
      else
        map.put("dbg_panic_msg", readCString(addr, 32));
    } catch (DebugProxyException e) {
      map.put("dbg_panic_msg", "<not enabled>");
    }

    try {
      Long isr_cnt = HexUtils.parseNumber(evaluateExpression("(uint32_t)dbg_isr_cnt"));
      if (isr_cnt == 0)
        map.put("dbg_isr_cnt", "not within ISR");
      else
        map.put("dbg_isr_cnt", "within ISR");
    } catch (DebugProxyException e) {
      map.put("dbg_isr_cnt", "<not enabled>");
    }

    try {
      Long lock_cnt = HexUtils.parseNumber(evaluateExpression("(uint32_t)dbg_lock_cnt"));
      if (lock_cnt == 0)
        map.put("dbg_lock_cnt", "not within lock");
      else
        map.put("dbg_lock_cnt", "within lock");
    } catch (DebugProxyException e) {
      map.put("dbg_lock_cnt", "<not enabled>");
    }

    return map;
  }
}
