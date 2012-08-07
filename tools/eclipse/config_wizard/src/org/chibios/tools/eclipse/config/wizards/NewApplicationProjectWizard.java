package org.chibios.tools.eclipse.config.wizards;

import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.IWizardPage;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;

public class NewApplicationProjectWizard extends Wizard implements INewWizard {

  private ISelection selection;
  private IWizardPage page;

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

    return true;
  }

}
