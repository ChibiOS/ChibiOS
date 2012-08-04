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

package org.chibios.tools.eclipse.config.handlers;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;

import org.chibios.tools.eclipse.config.utils.TemplateEngine;
import org.chibios.tools.eclipse.config.utils.TemplateException;
import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.handlers.HandlerUtil;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.osgi.framework.Bundle;

import config_wizard.Activator;

/**
 * Our sample handler extends AbstractHandler, an IHandler base class.
 * 
 * @see org.eclipse.core.commands.IHandler
 * @see org.eclipse.core.commands.AbstractHandler
 */
public class GenerateFiles extends AbstractHandler {

  /**
   * The constructor.
   */
  public GenerateFiles() {
  }

  /**
   * the command has been executed, so extract extract the needed information
   * from the application context.
   */
  public Object execute(ExecutionEvent event) throws ExecutionException {

    IWorkbenchWindow window = HandlerUtil
        .getActiveWorkbenchWindowChecked(event);
    ISelection selection = window.getSelectionService().getSelection();
    if (selection instanceof IStructuredSelection) {

      /* Retrieves the full path of the configuration file. */
      IPath cfgfilepath = ((IFile) ((IStructuredSelection) selection)
          .getFirstElement()).getLocation();
      
      /* Determines the base path as the parent of the configuration file.*/
      IPath basepath = cfgfilepath.removeLastSegments(1);

      /* Reads the configuration file into a Properties object. */
      Properties cfgfile = new Properties();
      try {
        cfgfile.load(new FileReader(cfgfilepath.toFile()));
      } catch (IOException e) {
        MessageDialog.openInformation(window.getShell(), "I/O Error",
            e.getMessage());
        return null;
      }

      /* Retrieves source property. */
      String source = cfgfile.getProperty("source");
      if (source == null) {
        MessageDialog.openInformation(window.getShell(), "Properties Error",
            "Property \"source\" not found in configuration file.");
        return null;
      }

      /* Retrieves xmlfile property. */
      String xmlfile = cfgfile.getProperty("xmlfile");
      if (xmlfile == null) {
        MessageDialog.openInformation(window.getShell(), "Properties Error",
            "Property \"xmlfile\" not found in configuration file.");
        return null;
      }

      /* Retrieves output property. */
      String output = cfgfile.getProperty("output");
      if (output == null) {
        MessageDialog.openInformation(window.getShell(), "Properties Error",
            "Property \"output\" not found in configuration file.");
        return null;
      }

      /* Calculating derived paths. */
      IPath sourcepath = new Path(source);
      IPath libpath = new Path("resources/gencfg/lib");
      try {
        Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
        sourcepath = new Path(FileLocator.toFileURL(
            FileLocator.find(bundle, sourcepath, null)).getFile());
        libpath = new Path(FileLocator.toFileURL(
            FileLocator.find(bundle, libpath, null)).getFile());
      } catch (IOException e) {
        MessageDialog.openInformation(window.getShell(), "Path Error",
            e.getMessage());
        return null;
      }

      /* Templates execution. */
      try {
        TemplateEngine.process(basepath.addTrailingSeparator().append(xmlfile)
            .toFile(), libpath.toFile(), sourcepath.toFile(),
            basepath.toFile(), new File(output));
      } catch (TemplateException e) {
        MessageDialog.openInformation(window.getShell(), "Processing Error",
            e.getMessage());
        return null;
      }
    }
    return null;
  }
}
