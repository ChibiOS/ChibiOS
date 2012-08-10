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

import org.eclipse.cdt.core.CCorePlugin;
import org.eclipse.cdt.core.model.CoreModel;
import org.eclipse.cdt.core.settings.model.ICProjectDescription;
import org.eclipse.cdt.core.settings.model.ICProjectDescriptionManager;
import org.eclipse.cdt.core.settings.model.extension.CConfigurationData;
import org.eclipse.cdt.managedbuilder.core.IBuilder;
import org.eclipse.cdt.managedbuilder.core.ManagedBuildManager;
import org.eclipse.cdt.managedbuilder.internal.core.Configuration;
import org.eclipse.cdt.managedbuilder.internal.core.ManagedProject;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.IWorkspaceRoot;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.operation.IRunnableWithProgress;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;

@SuppressWarnings("restriction")
public class NewApplicationProjectWizard extends Wizard implements INewWizard {

  private NewApplicationProjectWizardPage page;
  private ISelection  selection;

  private String projectName;
  private String finalProjectPath;
  private String platform;

  /**
   * Constructor for ConfigurationNewWizard.
   */
  public NewApplicationProjectWizard() {
    super();
    setNeedsProgressMonitor(true);
  }

  /**
   * Adding the page to the wizard.
   */
  public void addPages() {
    page = new NewApplicationProjectWizardPage(selection);
    addPage(page);
  }

  @Override
  public void init(IWorkbench workbench, IStructuredSelection selection) {
    this.selection = selection;
  }

  @Override
  public boolean performFinish() {

    projectName = page.getProjectName();
    finalProjectPath = page.getFinalProjectPath();
    platform = page.getPlatform();

    IRunnableWithProgress op = new IRunnableWithProgress() {
      public void run(IProgressMonitor monitor)
          throws InvocationTargetException {
        try {
          doFinish(monitor);
        }
        catch (CoreException e) {
          throw new InvocationTargetException(e);
        }
        finally {
          monitor.done();
        }
      }
    };
    try {
      getContainer().run(true, false, op);
    }
    catch (InterruptedException e) {
      return false;
    }
    catch (InvocationTargetException e) {
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

    IWorkspace workspace = ResourcesPlugin.getWorkspace();
    IWorkspaceRoot root = workspace.getRoot();

    monitor.beginTask("Creating " + projectName, 3);

    /* Step #1, creates the project file.*/
    IProject project = root.getProject(projectName);
    if (project.exists()) {
      monitor.done();
      MessageDialog.openError(getShell(), "Error", "Project " + projectName +
                                                   " already exists in workspace");
      return;
    }

    IProjectDescription desc = project.getWorkspace().newProjectDescription(projectName);
    if (finalProjectPath != null)
      desc.setLocation(new Path(finalProjectPath));
    project.create(desc, null);

    /* Step #2, makes it a CDT project.*/
    desc = workspace.newProjectDescription(projectName);
    project = CCorePlugin.getDefault().createCDTProject(desc, project, null);

    ICProjectDescriptionManager mngr = CoreModel.getDefault().getProjectDescriptionManager();
    ICProjectDescription cdesc = mngr.createProjectDescription(project, false);
    ManagedProject mproject = new ManagedProject(cdesc);
    Configuration cfg = new Configuration(mproject, null, "Default", "Default");
    IBuilder bld = cfg.getEditableBuilder();
    bld.setManagedBuildOn(false);
    CConfigurationData data = cfg.getConfigurationData();
    cdesc.createConfiguration(ManagedBuildManager.CFG_DATA_PROVIDER_ID, data);

    mngr.setProjectDescription(project, cdesc);
    monitor.worked(1);
  }
}
