package org.chibios.tools.eclipse.config.wizards;

import java.lang.reflect.InvocationTargetException;

import org.eclipse.cdt.core.CProjectNature;
import org.eclipse.cdt.core.CCorePlugin;
import org.eclipse.cdt.managedbuilder.core.BuildException;
import org.eclipse.cdt.managedbuilder.core.IManagedBuildInfo;
import org.eclipse.cdt.managedbuilder.core.IManagedProject;
import org.eclipse.cdt.managedbuilder.core.IProjectType;
import org.eclipse.cdt.managedbuilder.core.ManagedBuildManager;
import org.eclipse.cdt.managedbuilder.core.ManagedBuilderCorePlugin;
import org.eclipse.cdt.managedbuilder.core.ManagedCProjectNature;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
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

    monitor.beginTask("Creating " + projectName, 3);

    /* Step #1, creates the project file.*/
    IProject newProject = ResourcesPlugin.getWorkspace().getRoot().getProject(projectName);
    if (newProject.exists()) {
      monitor.done();
      MessageDialog.openError(getShell(), "Error", "Project " + projectName +
                                                   " already exists in workspace");
      return;
    }
    
    IProjectDescription desc = newProject.getWorkspace().newProjectDescription(newProject.getName());
    if (finalProjectPath != null)
      desc.setLocation(new Path(finalProjectPath));
    CCorePlugin corePlugin = CCorePlugin.getDefault();
    newProject = corePlugin.createCProject(desc, newProject, null,
                                           ManagedBuilderCorePlugin.MANAGED_MAKE_PROJECT_ID);
    IManagedBuildInfo managedBuildInfo = ManagedBuildManager.createBuildInfo(newProject);
    ManagedCProjectNature.addManagedNature(newProject, null);
    ManagedCProjectNature.addManagedBuilder(newProject, null);

    IProjectType parentProjectType;
    try {
      IManagedProject newManagedProject = ManagedBuildManager
          .createManagedProject(newProject, ManagedBuildManager.getProjectType(""));
    }
    catch (BuildException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }

    return;
    
    
/*    
    IProjectDescription desc = newProject.getWorkspace().newProjectDescription(newProject.getName());
    if (finalProjectPath != null)
      desc.setLocation(new Path(finalProjectPath));
    try {
      newProject.create(desc, null);
      if (!newProject.isOpen()) {
          newProject.open(null);
      }
    } catch (CoreException e) {
      monitor.done();
      MessageDialog.openError(getShell(), "Error", "Project " + projectName +
          " creation failed: " + e.getMessage());
      return;
    }
    monitor.worked(1);*/
    
    /* Step #2, adding builders.*/
/*    desc = newProject.getDescription();
    String[] build_configs = new String[] {ManagedCProjectNature.BUILDER_ID,
                                           "org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder"};
    desc.setBuildConfigs(build_configs);
    newProject.setDescription(desc, null);*/
//    CProjectNature.addCNature(newProject, null);
//    ManagedCProjectNature.addNature(newProject, ManagedCProjectNature.MNG_NATURE_ID, null);
//    ManagedCProjectNature.addNature(newProject, "org.eclipse.cdt.managedbuilder.core.ScannerConfigNature", null);
//    ManagedCProjectNature.addManagedBuilder(newProject, null);
//    monitor.worked(1);
  }
}
