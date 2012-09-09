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

package org.chibios.tools.eclipse.config.wizards;

import java.lang.reflect.InvocationTargetException;
import java.io.*;

import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;
import org.eclipse.core.runtime.*;
import org.eclipse.jface.operation.*;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.core.resources.*;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.ui.*;
import org.eclipse.ui.ide.IDE;
import org.osgi.framework.Bundle;

import config_wizard.Activator;

/**
 * This is a sample new wizard. Its role is to create a new file resource in the
 * provided container. If the container resource (a folder or a project) is
 * selected in the workspace when the wizard is opened, it will accept it as the
 * target container. The wizard creates one file with the extension "chcfg". If
 * a sample multi-page editor (also available as a template) is registered for
 * the same extension, it will be able to open it.
 */

public class ConfigurationNewWizard extends Wizard implements INewWizard {
  private ConfigurationNewWizardPage page;
  private ISelection selection;

  private String containerName;
  private String projectFileName;
  private String defaultDataFile;

  /**
   * Constructor for ConfigurationNewWizard.
   */
  public ConfigurationNewWizard() {
    super();
    setNeedsProgressMonitor(true);
  }

  /**
   * Adding the page to the wizard.
   */
  public void addPages() {
    page = new ConfigurationNewWizardPage(selection);
    addPage(page);
  }

  /**
   * Checks if the wizard page allows to finish.
   */
  public boolean canFinish() {

    return page.canFinish();
  }

  /**
   * This method is called when 'Finish' button is pressed in the wizard. We
   * will create an operation and run it using wizard as execution context.
   */
  public boolean performFinish() {

    containerName = page.getContainerName();
    projectFileName = page.getProjectFileName();
    defaultDataFile = page.getDefaultDataFile();

    IRunnableWithProgress op = new IRunnableWithProgress() {
      public void run(IProgressMonitor monitor)
          throws InvocationTargetException {
        try {
          doFinish(monitor);
        } catch (CoreException e) {
          throw new InvocationTargetException(e);
        } finally {
          monitor.done();
        }
      }
    };
    try {
      getContainer().run(true, false, op);
    } catch (InterruptedException e) {
      return false;
    } catch (InvocationTargetException e) {
      Throwable realException = e.getTargetException();
      MessageDialog.openError(getShell(), "Error", realException.getMessage());
      return false;
    }
    return true;
  }

  /**
   * The worker method. It will find the container, create the file if missing
   * or just replace its contents, and open the editor on the newly created
   * file.
   */
  private void doFinish(IProgressMonitor monitor) throws CoreException {

    IWorkspaceRoot root = ResourcesPlugin.getWorkspace().getRoot();
    IResource resource = root.findMember(new Path(containerName));
    if (!resource.exists() || !(resource instanceof IContainer)) {
      throwCoreException("Container \"" + containerName + "\" does not exist.");
    }
    IContainer container = (IContainer)resource;
    monitor.beginTask("Creating " + projectFileName, 3);

    /* Step #1, creates the project file.*/
    final IFile projectFile = container.getFile(new Path(projectFileName));
    Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
    IPath path = new Path(defaultDataFile);
    String s;
    try {
      s = FileLocator.toFileURL(FileLocator.find(bundle, path, null)).getFile();
      InputStream stream = new FileInputStream(s);
      if (projectFile.exists()) {
        projectFile.setContents(stream, true, true, monitor);
      } else {
        projectFile.create(stream, true, monitor);
      }
      stream.close();
    } catch (IOException e) {
    }
    monitor.worked(1);

    /* Step #2, opens the XML data file.*/
    monitor.setTaskName("Opening XML data file for editing...");
    getShell().getDisplay().asyncExec(new Runnable() {
      public void run() {
        IWorkbenchPage page = PlatformUI.getWorkbench()
            .getActiveWorkbenchWindow().getActivePage();
        try {
          IDE.openEditor(page, projectFile, true);
        } catch (PartInitException e) {
        }
      }
    });
    monitor.worked(1);

    /* Step #3, refreshing local resources.*/
    container.refreshLocal(IResource.DEPTH_INFINITE, monitor);
    monitor.worked(1);
  }

  private void throwCoreException(String message) throws CoreException {
    IStatus status = new Status(IStatus.ERROR,
                                "org.chibios.tools.eclipse.config", IStatus.OK,
                                message, null);
    throw new CoreException(status);
  }

  /**
   * We will accept the selection in the workbench to see if we can initialize
   * from it.
   * 
   * @see IWorkbenchWizard#init(IWorkbench, IStructuredSelection)
   */
  public void init(IWorkbench workbench, IStructuredSelection selection) {
    this.selection = selection;
  }
}
