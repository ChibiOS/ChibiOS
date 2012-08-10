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
import java.io.IOException;

import org.chibios.tools.eclipse.config.utils.TemplateEngine;
import org.chibios.tools.eclipse.config.utils.TemplateException;
import org.eclipse.core.commands.AbstractHandler;
import org.eclipse.core.commands.ExecutionEvent;
import org.eclipse.core.commands.ExecutionException;
import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.ui.IWorkbenchWindow;
import org.eclipse.ui.handlers.HandlerUtil;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.JDOMException;
import org.jdom2.input.SAXBuilder;
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
      IPath cfgfilepath = ((IFile)((IStructuredSelection)selection)
          .getFirstElement()).getLocation();

      /* DOM tree creation. */
      SAXBuilder builder = new SAXBuilder();
      Document document;
      try {
        document = builder.build(cfgfilepath.toFile());
      } catch (JDOMException e) {
        return null;
      } catch (IOException e) {
        return null;
      }

      /* Retrieving configuration settings info.*/
      Element settings = document.getRootElement().getChild("configuration_settings");
      String templates_path = settings.getChildText("templates_path");
      String output_path = settings.getChildText("output_path");

      /* Calculating derived paths. */
      IPath tpath = new Path(templates_path);
      try {
        Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
        tpath = new Path(FileLocator.toFileURL(
            FileLocator.find(bundle, tpath, null)).getFile());
      } catch (IOException e) {
        MessageDialog.openInformation(window.getShell(), "Path Error",
            e.getMessage());
        return null;
      }

      /* Templates execution. */
      try {
        TemplateEngine.process(cfgfilepath.toFile(),
                               tpath.toFile(),
                               new File(output_path));
      } catch (TemplateException e) {
        MessageDialog.openInformation(window.getShell(), "Processing Error",
            e.getMessage());
        return null;
      }

      /* Destination directory refresh.*/
      IContainer container = ResourcesPlugin.getWorkspace().getRoot().getFileForLocation(cfgfilepath).getParent();
      try {
    	  container.refreshLocal(IResource.DEPTH_INFINITE, null);
      } catch (CoreException e) {
        e.printStackTrace();
      }
    }
    return null;
  }
}
