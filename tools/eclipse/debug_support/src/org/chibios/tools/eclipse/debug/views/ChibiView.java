/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package org.chibios.tools.eclipse.debug.views;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map.Entry;
import java.util.Set;

import org.chibios.tools.eclipse.debug.utils.DebugProxy;
import org.chibios.tools.eclipse.debug.utils.DebugProxyException;
import org.chibios.tools.eclipse.debug.utils.HexUtils;

import org.eclipse.ui.internal.IWorkbenchThemeConstants;
import org.eclipse.ui.part.*;
import org.eclipse.ui.themes.ITheme;
import org.eclipse.jface.action.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.ui.*;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.TableItem;

import org.eclipse.debug.core.DebugEvent;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.IDebugEventSetListener;
import org.eclipse.cdt.debug.internal.core.model.CDebugTarget;
import org.eclipse.wb.swt.SWTResourceManager;
import org.eclipse.wb.swt.ResourceManager;
import org.eclipse.swt.events.FocusAdapter;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;

/**
 * This sample class demonstrates how to plug-in a new workbench view. The view
 * shows data obtained from the model. The sample creates a dummy model on the
 * fly, but a real implementation would connect to the model available either in
 * this or another plug-in (e.g. the workspace). The view is connected to the
 * model using a content provider.
 * <p>
 * The view uses a label provider to define how model objects should be
 * presented in the view. Each view can present the same model objects using
 * different labels and icons, if needed. Alternatively, a single label provider
 * can be shared between views in order to ensure that objects of the same type
 * are presented in the same way everywhere.
 * <p>
 */

@SuppressWarnings("restriction")
public class ChibiView extends ViewPart implements IDebugEventSetListener {

  /**
   * The ID of the view as specified by the extension.
   */
  public static final String ID = "org.chibios.tools.eclipse.debug.views.ChibiView";

  private CTabFolder tabFolder;
  private CTabItem tbtmGlobal;
  private CTabItem tbtmThreads;
  private CTabItem tbtmTimers;
  private CTabItem tbtmTraceBuffer;

  private Action refreshAction;
  private Table threadsTable;
  private Table timersTable;

  private DebugProxy debugger;
  private Table tbTable;
  private Table globalTable;

  private ITheme theme;

  private FocusAdapter focus = new FocusAdapter() {
    @Override
    public void focusLost(FocusEvent e) {
      setInactive();
    }
    @Override
    public void focusGained(FocusEvent e) {
      setActive();
    }
  };

  /**
   * The constructor.
   */
  public ChibiView() {
    
    theme = PlatformUI.getWorkbench().getThemeManager().getCurrentTheme();
  }

  private void setActive() {
    tabFolder.setSelectionBackground(
        new org.eclipse.swt.graphics.Color[] {
            theme.getColorRegistry().get(IWorkbenchThemeConstants.ACTIVE_TAB_BG_START),
            theme.getColorRegistry().get(IWorkbenchThemeConstants.ACTIVE_TAB_BG_END)
        },
        new int[] {100},
        true);
    tabFolder.setSelectionForeground(theme.getColorRegistry().get(IWorkbenchThemeConstants.ACTIVE_TAB_TEXT_COLOR));
  }

  private void setInactive() {
    tabFolder.setSelectionBackground(
        new org.eclipse.swt.graphics.Color[] {
            theme.getColorRegistry().get(IWorkbenchThemeConstants.INACTIVE_TAB_BG_START),
            theme.getColorRegistry().get(IWorkbenchThemeConstants.INACTIVE_TAB_BG_END)
        },
        new int[] {theme.getInt(IWorkbenchThemeConstants.ACTIVE_TAB_PERCENT)},
        true);
    tabFolder.setSelectionForeground(theme.getColorRegistry().get(IWorkbenchThemeConstants.INACTIVE_TAB_TEXT_COLOR));
  }

  /**
   * This is a callback that will allow us to create the viewer and initialize
   * it.
   */
  public void createPartControl(Composite parent) {

    tabFolder = new CTabFolder(parent, SWT.BORDER | SWT.BOTTOM);
    tabFolder.setFont(theme.getFontRegistry().get(IWorkbenchThemeConstants.TAB_TEXT_FONT));
    tabFolder.setBackground(theme.getColorRegistry().get(IWorkbenchThemeConstants.INACTIVE_TAB_BG_END));
    tabFolder.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        setActive();
        tabFolder.getSelection().getControl().setFocus();
      }
    });
    tabFolder.addFocusListener(new FocusAdapter() {
      @Override
      public void focusGained(FocusEvent e) {
        tabFolder.getSelection().getControl().setFocus();
      }
    });
    setInactive();
    tabFolder.setSimple(false);

    tbtmGlobal = new CTabItem(tabFolder, SWT.NONE);
    tbtmGlobal.setText("Global");
    
    globalTable = new Table(tabFolder, SWT.FULL_SELECTION);
    globalTable.addFocusListener(focus);
    globalTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    tbtmGlobal.setControl(globalTable);
    globalTable.setHeaderVisible(true);

    TableColumn tblclmnGlobalHidden = new TableColumn(globalTable, SWT.RIGHT);
    tblclmnGlobalHidden.setWidth(0);
    tblclmnGlobalHidden.setText("");

    TableColumn tblclmnGlobalVariableName = new TableColumn(globalTable, SWT.LEFT);
    tblclmnGlobalVariableName.setWidth(150);
    tblclmnGlobalVariableName.setText("Variable");

    TableColumn tblclmnGlobalVariableValue = new TableColumn(globalTable, SWT.LEFT);
    tblclmnGlobalVariableValue.setWidth(300);
    tblclmnGlobalVariableValue.setText("Value");

    tbtmThreads = new CTabItem(tabFolder, SWT.NONE);
    tbtmThreads.setText("Threads");

    threadsTable = new Table(tabFolder, SWT.FULL_SELECTION);
    threadsTable.addFocusListener(focus);
    tbtmThreads.setControl(threadsTable);
    threadsTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    threadsTable.setHeaderVisible(true);

    TableColumn tblclmnThreadAddress = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadAddress.setWidth(72);
    tblclmnThreadAddress.setText("Address");

    TableColumn tblclmnThreadLimit = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadLimit.setWidth(72);
    tblclmnThreadLimit.setText("StkLimit");

    TableColumn tblclmnThreadStack = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadStack.setWidth(72);
    tblclmnThreadStack.setText("Stack");

    TableColumn tblclmnThreadUsed = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadUsed.setWidth(72);
    tblclmnThreadUsed.setText("StkUnused");

    TableColumn tblclmnThreadName = new TableColumn(threadsTable, SWT.LEFT);
    tblclmnThreadName.setWidth(144);
    tblclmnThreadName.setText("Name");

    TableColumn tblclmnThreadState = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadState.setWidth(72);
    tblclmnThreadState.setText("State");

    TableColumn tblclmnThreadFlags = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadFlags.setWidth(40);
    tblclmnThreadFlags.setText("Flgs");

    TableColumn tblclmnThreadPriority = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadPriority.setWidth(40);
    tblclmnThreadPriority.setText("Prio");

    TableColumn tblclmnThreadRefs = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadRefs.setWidth(40);
    tblclmnThreadRefs.setText("Refs");

    TableColumn tblclmnThreadTime = new TableColumn(threadsTable, SWT.RIGHT);
    tblclmnThreadTime.setWidth(64);
    tblclmnThreadTime.setText("Time");

    TableColumn tblclmnThreadShared = new TableColumn(threadsTable, SWT.LEFT);
    tblclmnThreadShared.setWidth(72);
    tblclmnThreadShared.setText("Obj/Msg");

    tbtmTimers = new CTabItem(tabFolder, SWT.NONE);
    tbtmTimers.setText("Timers");

    timersTable = new Table(tabFolder, SWT.FULL_SELECTION);
    timersTable.addFocusListener(focus);
    tbtmTimers.setControl(timersTable);
    timersTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    timersTable.setHeaderVisible(true);

    TableColumn tblclmnTimerAddress = new TableColumn(timersTable, SWT.RIGHT);
    tblclmnTimerAddress.setWidth(72);
    tblclmnTimerAddress.setText("Address");

    TableColumn tblclmnTimerTime = new TableColumn(timersTable, SWT.RIGHT);
    tblclmnTimerTime.setWidth(72);
    tblclmnTimerTime.setText("Time");

    TableColumn tblclmnTimerDelta = new TableColumn(timersTable, SWT.RIGHT);
    tblclmnTimerDelta.setWidth(72);
    tblclmnTimerDelta.setText("Delta");

    TableColumn tblclmnTimerCallback = new TableColumn(timersTable, SWT.RIGHT);
    tblclmnTimerCallback.setWidth(72);
    tblclmnTimerCallback.setText("Callback");

    TableColumn tblclmnTimerParameter = new TableColumn(timersTable, SWT.LEFT);
    tblclmnTimerParameter.setWidth(72);
    tblclmnTimerParameter.setText("Param");

    tbtmTraceBuffer = new CTabItem(tabFolder, SWT.NONE);
    tbtmTraceBuffer.setText("TraceBuffer");
    
    tbTable = new Table(tabFolder, SWT.FULL_SELECTION);
    tbTable.addFocusListener(focus);
    tbTable.setFont(SWTResourceManager.getFont("Courier New", 8, SWT.NORMAL));
    tbtmTraceBuffer.setControl(tbTable);
    tbTable.setHeaderVisible(true);

    TableColumn tblclmnTraceBufferHidden = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferHidden.setWidth(0);
    tblclmnTraceBufferHidden.setText("");

    TableColumn tblclmnTraceBufferIndex = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferIndex.setWidth(48);
    tblclmnTraceBufferIndex.setText("Event");

    TableColumn tblclmnTraceBufferTime = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferTime.setWidth(64);
    tblclmnTraceBufferTime.setText("Time");

    TableColumn tblclmnTraceBufferPrevAddress = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferPrevAddress.setWidth(72);
    tblclmnTraceBufferPrevAddress.setText("Previous");

    TableColumn tblclmnTraceBufferPrevName = new TableColumn(tbTable, SWT.LEFT);
    tblclmnTraceBufferPrevName.setWidth(144);
    tblclmnTraceBufferPrevName.setText("Previous Name");

    TableColumn tblclmnTraceBufferState = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferState.setWidth(72);
    tblclmnTraceBufferState.setText("State");

    TableColumn tblclmnTraceBufferShared = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferShared.setWidth(72);
    tblclmnTraceBufferShared.setText("Obj/Msg");

    TableColumn tblclmnTraceBufferCurrentAddress = new TableColumn(tbTable, SWT.RIGHT);
    tblclmnTraceBufferCurrentAddress.setWidth(72);
    tblclmnTraceBufferCurrentAddress.setText("Current");

    TableColumn tblclmnTraceBufferCurrentName = new TableColumn(tbTable, SWT.LEFT);
    tblclmnTraceBufferCurrentName.setWidth(144);
    tblclmnTraceBufferCurrentName.setText("Current Name");

    makeActions();
    hookContextMenu();
    contributeToActionBars();

    tabFolder.setSelection(tbtmGlobal);

    DebugPlugin.getDefault().addDebugEventListener(this);

    try {
      debugger = new DebugProxy();
    } catch (DebugProxyException e) {}
  }

  /**
   * @brief Handling events from the debugger.
   */
  @Override
  public void handleDebugEvents(DebugEvent[] events) {
    for (DebugEvent event : events) {
      switch (event.getKind()) {
      case DebugEvent.CREATE:
        Object source = event.getSource();
        if (source instanceof CDebugTarget) {
          try {
            debugger = new DebugProxy((CDebugTarget)source);
          } catch (DebugProxyException e) {}
        }
        break;
      }
    }
  }

  private void hookContextMenu() {
    MenuManager menuMgr = new MenuManager("#PopupMenu");
    menuMgr.setRemoveAllWhenShown(true);
    menuMgr.addMenuListener(new IMenuListener() {
      public void menuAboutToShow(IMenuManager manager) {
        ChibiView.this.fillContextMenu(manager);
      }
    });
  }

  private void contributeToActionBars() {
    IActionBars bars = getViewSite().getActionBars();
    fillLocalPullDown(bars.getMenuManager());
    fillLocalToolBar(bars.getToolBarManager());
  }

  private void fillLocalPullDown(IMenuManager manager) {
    manager.add(refreshAction);
/*    manager.add(new Separator());
    manager.add(refreshAction);*/
  }

  private void fillContextMenu(IMenuManager manager) {
    manager.add(refreshAction);
    // Other plug-ins can contribute there actions here
    manager.add(new Separator(IWorkbenchActionConstants.MB_ADDITIONS));
  }

  private void fillLocalToolBar(IToolBarManager manager) {
    manager.add(refreshAction);
  }

  private void fillGlobalTable() {
    LinkedHashMap<String, String> lhm;

    // If the debugger is not yet present then do nothing.
    if (debugger == null)
      return;

    // Reading the list of global variables, null can be returned if the debugger
    // does not respond.
    try {
      lhm = debugger.readGlobalVariables();
      if (lhm == null)
        return;
    } catch (DebugProxyException e) {
      showMessage("Error: " + e.getMessage() + ".");
      return;
    }

    globalTable.removeAll();
    
    Set<Entry<String, String>> set = lhm.entrySet();
    for (Entry<String, String> entry : set) {
      TableItem tableItem = new TableItem(globalTable, SWT.NONE);
      tableItem.setText(new String[] {
        "",
        entry.getKey(),
        entry.getValue()
      });
    }
  }

  private String makeHex(String s) {
    try {
      s = HexUtils.dword2HexString((int)HexUtils.parseNumber(s));
    } catch (Exception e) {}
    return s;
  }

  private void fillThreadsTable() {
    LinkedHashMap<String, HashMap<String, String>> lhm;

    // If the debugger is not yet present then do nothing.
    if (debugger == null)
      return;

    // Reading the list of threads, null can be returned if the debugger
    // does not respond.
    try {
      lhm = debugger.readThreads();
      if (lhm == null)
        return;
    } catch (DebugProxyException e) {
      showMessage("Error: " + e.getMessage() + ".");
      return;
    }

    threadsTable.removeAll();

    Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
    for (Entry<String, HashMap<String, String>> entry : set) {
      HashMap<String, String> map = entry.getValue();
      TableItem tableItem = new TableItem(threadsTable, SWT.NONE);
      tableItem.setText(new String[] {
        makeHex(entry.getKey()),
        makeHex(map.get("stklimit")),
        makeHex(map.get("stack")),
        map.get("stkunused"),
        map.get("name"),
        map.get("state_s"),
        HexUtils.byte2HexString((int)HexUtils.parseNumber(map.get("flags"))),
        map.get("prio"),
        map.get("refs"),
        map.get("time"),
        makeHex(map.get("wtobjp"))
      });
    }
  }

  private void fillTimersTable() {
    LinkedHashMap<String, HashMap<String, String>> lhm;

    // If the debugger is not yet present then do nothing.
    if (debugger == null)
      return;

    // Reading the list of threads, null can be returned if the debugger
    // does not respond.
    try {
      lhm = debugger.readTimers();
      if (lhm == null)
        return;
    } catch (DebugProxyException e) {
      showMessage("Error: " + e.getMessage() + ".");
      return;
    }

    timersTable.removeAll();

    Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
    long time = 0;
    for (Entry<String, HashMap<String, String>> entry : set) {
      HashMap<String, String> map = entry.getValue();
      time = time + HexUtils.parseNumber(map.get("delta"));
      TableItem tableItem = new TableItem(timersTable, SWT.NONE);
      tableItem.setText(new String[] {
        makeHex(entry.getKey()),
        Long.toString(time),
        "+" + HexUtils.parseNumber(map.get("delta")),
        makeHex(map.get("func")),
        makeHex(map.get("par"))
      });          
    }
  }

  private void fillTraceBufferTable() {
    LinkedHashMap<String, HashMap<String, String>> lhm, lhmthreads;

    // If the debugger is not yet present then do nothing.
    if (debugger == null)
      return;

    // Read active threads for retrieving names.
    try {
      lhmthreads = debugger.readThreads();
      if (lhmthreads == null)
        return;
    } catch (DebugProxyException e) {
      lhmthreads = new LinkedHashMap<String, HashMap<String, String>>(0);
    }

    // Reading the list of threads, null can be returned if the debugger
    // does not respond.
    try {
      lhm = debugger.readTraceBuffer();
      if (lhm == null)
        return;
    } catch (DebugProxyException e) {
      showMessage("Error: " + e.getMessage() + ".");
      return;
    }

    tbTable.removeAll();

    Set<Entry<String, HashMap<String, String>>> set = lhm.entrySet();
    String prev = "";
    String prevname = "";
    for (Entry<String, HashMap<String, String>> entry : set) {
      HashMap<String, String> map = entry.getValue();
      TableItem tableItem = new TableItem(tbTable, SWT.NONE);
      
      // Searches the current thread into the threads map.
      String currentaddr = map.get("tp");
      HashMap<String, String> thread = lhmthreads.get(currentaddr);
      String currentname;
      if (thread != null)
        currentname = thread.get("name");
      else
        currentname = "";

      String current = makeHex(currentaddr);
      tableItem.setText(new String[] {
        "",
        entry.getKey(),
        map.get("time"),
        prev,
        prevname,
        map.get("state_s"),
        makeHex(map.get("wtobjp")),
        current,
        currentname
      });
      prev = current;
      prevname = currentname;
    }
  }

  private void makeActions() {
    
    // Refresh action.
    refreshAction = new Action() {
      public void run() {
        CTabItem tabitem = tabFolder.getSelection();
        if (tabitem == null)
          return;
        if (tabitem == tbtmGlobal)
          fillGlobalTable();
        else if (tabitem == tbtmThreads)
          fillThreadsTable();
        else if (tabitem == tbtmTimers)
          fillTimersTable();
        else if (tabitem == tbtmTraceBuffer)
          fillTraceBufferTable();
      }
    };
    refreshAction.setDisabledImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/dlcl16/refresh_nav.gif"));
    refreshAction.setImageDescriptor(ResourceManager.getPluginImageDescriptor("org.eclipse.cdt.ui", "/icons/elcl16/refresh_nav.gif"));
    refreshAction.setText("Refresh");
    refreshAction.setToolTipText("Refresh timers list");
  }

  private void showMessage(String message) {
    MessageDialog.openInformation(tabFolder.getShell(),
        "ChibiOS/RT Views", message);
  }

  /**
   * Passing the focus request to the viewer's control.
   */
  public void setFocus() {
    tabFolder.setFocus();
  }
}
