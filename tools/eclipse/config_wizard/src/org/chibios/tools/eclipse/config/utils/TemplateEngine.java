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

package org.chibios.tools.eclipse.config.utils;

import java.io.IOException;
import java.util.HashMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.Display;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.IWorkbenchPage;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.PartInitException;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.console.ConsolePlugin;
import org.eclipse.ui.console.IConsole;
import org.eclipse.ui.console.IConsoleConstants;
import org.eclipse.ui.console.IConsoleManager;
import org.eclipse.ui.console.IConsoleView;
import org.eclipse.ui.console.MessageConsole;
import org.eclipse.ui.console.MessageConsoleStream;
import org.osgi.framework.Bundle;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import config_wizard.Activator;

import fmpp.Engine;
import fmpp.ProcessingException;
import fmpp.ProgressListener;
import fmpp.setting.SettingException;
import fmpp.setting.Settings;
import freemarker.ext.dom.NodeModel;
import freemarker.template.TemplateNodeModel;

/**
 * FMPP wrapper class.
 */
public class TemplateEngine {

  private final static String CONSOLE_NAME = "ChibiOS/RT Configuration Tool";

  private final static Color DEFAULT_MESSAGE = new Color(Display.getDefault(), new RGB(0, 0, 255));
  private final static Color DEFAULT_OUTPUT = new Color(Display.getDefault(), new RGB(0, 0, 0));
  private final static Color DEFAULT_WARNING = new Color(Display.getDefault(), new RGB(255, 255, 0));
  private final static Color DEFAULT_ERROR = new Color(Display.getDefault(), new RGB(255, 0, 0));

  private static Settings settings;
  private static MessageConsoleStream msg;
  private static MessageConsoleStream out;
  private static MessageConsoleStream err;
  private static MessageConsoleStream warn;

  /**
   * Runs the templates engine.
   * 
   * @param xmldata
   *          absolute path to XML data file
   * @param sourcedir
   *          absolute path to templates directory
   * @param outputdir
   *          output directory relative to xmldata
   * 
   * @throws SettingException
   * @throws ProcessingException
   */
  public static void process(java.io.File xmldata,
                             java.io.File sourcedir,
                             java.io.File outputdir) throws TemplateException {

    /* Finds or creates a named console. */
    MessageConsole console = findConsole(CONSOLE_NAME);
    activateConsole(console);
    console.clearConsole();
    msg = console.newMessageStream();
    msg.setColor(DEFAULT_MESSAGE);
    out = console.newMessageStream();
    out.setColor(DEFAULT_OUTPUT);
    err = console.newMessageStream();
    err.setColor(DEFAULT_ERROR);
    warn = console.newMessageStream();
    warn.setColor(DEFAULT_WARNING);

    /*
     * Calculates the path for FTL libraries.
     */
    IPath libpath = new Path("resources/gencfg/lib");
    Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
    try {
      libpath = new Path(FileLocator.toFileURL(FileLocator.find(bundle, libpath, null)).getFile());
    }
    catch (IOException e1) {
      err.println(": FTL libraries path not found.");
      return;
    }

    /*
     * Instantiates the FMPP Settings engine and associates a listener for
     * events to be logged.
     */
    try {
      settings = new Settings(xmldata.getParentFile());
    } catch (SettingException e) {
      e.printStackTrace();
      return;
    }
    settings.addProgressListener(new ProgressListener() {
      public void notifyProgressEvent(Engine engine, int event,
                                      java.io.File src, int pMode,
                                      java.lang.Throwable error,
                                      java.lang.Object param) {

        if (error != null) {
          err.println(": " + error.getMessage());
          return;
        }

        if (pMode == Engine.PMODE_IGNORE)
          return;

        switch (event) {
        case EVENT_BEGIN_PROCESSING_SESSION:
          msg.println("Starting session");
          msg.println();
          break;
        case EVENT_END_PROCESSING_SESSION:
          msg.println();
          msg.println("Finished");
          break;
        case EVENT_BEGIN_FILE_PROCESSING:
          out.println("> Processing " + src.getName());
          break;
        case EVENT_END_FILE_PROCESSING:
          break;
        case EVENT_IGNORING_DIR:
          out.println("> Ignoring directory " + src.getName());
          break;
        case EVENT_SOURCE_NOT_MODIFIED:
          out.println("> Skipping " + src.getName());
          break;
        case EVENT_WARNING:
          warn.println(": " + (String)param);
          break;
        }
      }
    });

    /* Loading data model. */
    DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
    dbf.setNamespaceAware(true);
    dbf.setValidating(false);
    DocumentBuilder db;
    try {
      db = dbf.newDocumentBuilder();
    } catch (ParserConfigurationException e) {
      throw new TemplateException(e.getMessage());
    }
    Document dom1, dom_snippets;
    try {
      dom1 = db.parse(xmldata);
      java.io.File xmlsnippets = libpath.addTrailingSeparator().append("code_snippets.xml").toFile();
      dom_snippets = db.parse(xmlsnippets);
    } catch (SAXException e) {
      throw new TemplateException(e.getMessage());
    } catch (IOException e) {
      throw new TemplateException(e.getMessage());
    }

    NodeModel.removeComments(dom1);
    NodeModel.mergeAdjacentText(dom1);
    NodeModel.removeComments(dom_snippets);
    TemplateNodeModel doc1 = NodeModel.wrap(dom1);
    NodeModel.mergeAdjacentText(dom_snippets);
    TemplateNodeModel doc_snippets = NodeModel.wrap(dom_snippets);

    HashMap<String, TemplateNodeModel> xmls = new HashMap<String, TemplateNodeModel>();
    xmls.put("doc1", doc1);
    xmls.put("doc_snippets", doc_snippets);

    /* Setting libraries path. */
    HashMap<String, String> libs = new HashMap<String, String>();
    libs.put("lib", libpath.toString());

    /* Other settings. */
    try {
      settings.add("freemarkerLinks", libs);
      settings.add("data", xmls);
      settings.add("sourceRoot", sourcedir.toString());
      settings.add("outputRoot", outputdir.toString());
      settings.add("ignoreCvsFiles", true);
      settings.add("ignoreSvnFiles", true);
    } catch (SettingException e) {
      throw new TemplateException(e.getMessage());
    }

    /* Processor execution. */
    try {
      settings.execute();
    } catch (SettingException e) {
      throw new TemplateException(e.getMessage());
    } catch (ProcessingException e) {
      throw new TemplateException(e.getMessage());
    }
  }

  private static MessageConsole findConsole(String name) {
    ConsolePlugin plugin = ConsolePlugin.getDefault();
    IConsoleManager conMan = plugin.getConsoleManager();
    IConsole[] existing = conMan.getConsoles();
    for (int i = 0; i < existing.length; i++)
      if (name.equals(existing[i].getName()))
        return (MessageConsole) existing[i];

    /* No console found, so create a new one. */
    MessageConsole myConsole = new MessageConsole(name, null);
    conMan.addConsoles(new IConsole[] { myConsole });
    return myConsole;
  }

  private static void activateConsole(MessageConsole console) {
    IWorkbench wb = PlatformUI.getWorkbench();
    IWorkbenchWindow win = wb.getActiveWorkbenchWindow();
    IWorkbenchPage page = win.getActivePage();
    IConsoleView view;
    try {
      view = (IConsoleView) page.showView(IConsoleConstants.ID_CONSOLE_VIEW);
    } catch (PartInitException e) {
      e.printStackTrace();
      return;
    }
    view.display(console);
  }
}
