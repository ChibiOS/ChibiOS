package org.chibios.tools.eclipse.config.wizards;

import java.io.File;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.ModifyEvent;

public class NewApplicationProjectWizardPage extends WizardPage {

  private ISelection selection;
  private Composite container;
  private Text projectParentPathText;
  private Button btnBrowse;
  private Label lblProjectName;
  private Text projectNameText;
  private Button useCustomPathButton;
  private Label lblFinalProjectPath;
  private Text projectFinalPathText;

  /**
   * Constructor for SampleNewWizardPage.
   */
  public NewApplicationProjectWizardPage(ISelection selection) {

    super("wizardPage");
    setTitle("ChibiOS/RT New Application Project Wizard");
    setDescription("This wizard creates a new ChibiOS/RT application project.");
    this.selection = selection;
  }

  /**
   * @see IDialogPage#createControl(Composite)
   */
  @Override
  public void createControl(Composite parent) {
    container = new Composite(parent, SWT.NULL);
    GridLayout layout = new GridLayout();
    container.setLayout(layout);
    layout.numColumns = 3;
    layout.verticalSpacing = 9;
    
    lblProjectName = new Label(container, SWT.NONE);
    lblProjectName.setText("Project name:");

    projectNameText = new Text(container, SWT.BORDER);
    projectNameText.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        projectNameUpdated();
      }
    });
    projectNameText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
        new Label(container, SWT.NONE);
    
    useCustomPathButton = new Button(container, SWT.CHECK);
    useCustomPathButton.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        if (useCustomPathButton.getSelection()) {
          projectParentPathText.setText(ResourcesPlugin.getWorkspace().getRoot().getLocation().toString());
          projectParentPathText.setEnabled(false);
          updateFinalProjectPathText();
        }
        else {
          projectParentPathText.setText(ResourcesPlugin.getWorkspace().getRoot().getLocation().toString());
          projectParentPathText.setEnabled(true);
          updateFinalProjectPathText();
        }
      }
    });
    useCustomPathButton.setText("Use default location");
    new Label(container, SWT.NONE);
    new Label(container, SWT.NONE);

    Label lbl1 = new Label(container, SWT.NULL);
    lbl1.setText("Project parent path:");

    projectParentPathText = new Text(container, SWT.BORDER | SWT.SINGLE);
    projectParentPathText.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        projectPathUpdated();
      }
    });
    projectParentPathText.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));

    btnBrowse = new Button(container, SWT.NONE);
    btnBrowse.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
      }
    });
    btnBrowse.setText("Browse...");
    
    lblFinalProjectPath = new Label(container, SWT.NONE);
    lblFinalProjectPath.setText("Final project path:");
    
    projectFinalPathText = new Text(container, SWT.BORDER);
    projectFinalPathText.setEditable(false);
    projectFinalPathText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));

    populateWizardPanel();
    initialize();
    setControl(container);
  }

  /**
   * Tests if the current workbench selection is a suitable container to use.
   */
  private void initialize() {
    
    /* Initial state of the check box and project path text.*/
    useCustomPathButton.setSelection(true);
    projectParentPathText.setText(ResourcesPlugin.getWorkspace().getRoot().getLocation().toString());
    projectParentPathText.setEnabled(false);

    /* Update checks on the fields.*/
    projectNameUpdated();
  }

  /**
   * Fills the wizard configuration panel from XML data.
   * 
   * @param configurationTemplateCombo
   *          the combo box to be populated
   */
  private void populateWizardPanel() {

  }

  private void projectNameUpdated() {
    String name = projectNameText.getText();

    updateFinalProjectPathText();

    if (!isValidFilename(name)) {
      updateStatus("Invalid project name.");
      return;
    }

    IProject project = ResourcesPlugin.getWorkspace().getRoot().getProject(name);
    if (project.exists()) {
      updateStatus("Project exists.");
      return;
    }
    updateStatus(null);
  }

  private void projectPathUpdated() {
    File path = new File(projectParentPathText.getText());

    updateFinalProjectPathText();

    if (!path.exists()) {
      updateStatus("Project path is not valid.");
      return;
    }
    if (!path.isDirectory()) {
      updateStatus("Project path is a directory.");
      return;
    }
    updateStatus(null);
  }

  /**
   * Updates the status text in the Wizard page.
   * 
   * @param message
   *          the message to be shown
   */
  private void updateStatus(String message) {

    setErrorMessage(message);
    setPageComplete(message == null);
  }

  private void updateFinalProjectPathText() {

    IPath parent = new Path(projectParentPathText.getText());
    IPath project = parent.addTrailingSeparator().append(projectNameText.getText());
    projectFinalPathText.setText(project.toString());
  }
  
  private boolean isValidFilename(String name) {
    
    if (name.length() == 0)
      return false;
    if ((name.indexOf("`") >= 0) || (name.indexOf("?") >= 0) ||
        (name.indexOf("*") >= 0) || (name.indexOf("<") >= 0) ||
        (name.indexOf(">") >= 0) || (name.indexOf("|") >= 0) ||
        (name.indexOf("\"") >= 0) || (name.indexOf(":") >= 0) ||
        (name.indexOf("#") >= 0) || (name.indexOf("\\") >= 0) ||
        (name.indexOf("/") >= 0) || (name.indexOf("|") >= 0))
      return false;
    return true;
  }
}
