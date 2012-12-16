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

import java.io.File;
import java.io.IOException;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.DirectoryDialog;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.widgets.Combo;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.JDOMException;
import org.jdom2.input.SAXBuilder;
import org.osgi.framework.Bundle;

import config_wizard.Activator;

public class NewApplicationProjectWizardPage extends WizardPage {

  private Composite container;
  private Text projectParentPathText;
  private Button btnBrowse;
  private Label lbl1;
  private Text projectNameText;
  private Button useCustomPathButton;
  private Label lbl3;
  private Text projectFinalPathText;
  private Label lbl4;
  private Combo platformCombo;

  /**
   * Constructor for SampleNewWizardPage.
   */
  public NewApplicationProjectWizardPage(ISelection selection) {

    super("wizardPage");
    setTitle("ChibiOS/RT New Application Project Wizard");
    setDescription("This wizard creates a new ChibiOS/RT application project.");
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

    /* Layout row 1.*/
    lbl1 = new Label(container, SWT.NONE);
    lbl1.setText("Project name:");
    projectNameText = new Text(container, SWT.BORDER);
    projectNameText.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        projectPageUpdated();
      }
    });
    projectNameText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
        new Label(container, SWT.NONE);

    /* Layout row 2.*/
    useCustomPathButton = new Button(container, SWT.CHECK);
    useCustomPathButton.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        if (useCustomPathButton.getSelection()) {
          projectParentPathText.setText(ResourcesPlugin.getWorkspace().getRoot().getLocation().toString());
          projectParentPathText.setEnabled(false);
          btnBrowse.setEnabled(false);
          updateFinalProjectPathText();
        }
        else {
          projectParentPathText.setText(ResourcesPlugin.getWorkspace().getRoot().getLocation().toString());
          projectParentPathText.setEnabled(true);
          btnBrowse.setEnabled(true);
          updateFinalProjectPathText();
        }
      }
    });
    useCustomPathButton.setText("Use default location");
    new Label(container, SWT.NONE);
    new Label(container, SWT.NONE);

    /* Layout row 3.*/
    Label lbl2 = new Label(container, SWT.NULL);
    lbl2.setText("Project parent path:");
    projectParentPathText = new Text(container, SWT.BORDER | SWT.SINGLE);
    projectParentPathText.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        projectPageUpdated();
      }
    });
    projectParentPathText.setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
    btnBrowse = new Button(container, SWT.NONE);
    btnBrowse.addSelectionListener(new SelectionAdapter() {
      @Override
      public void widgetSelected(SelectionEvent e) {
        /* Prompts the user for a parent directory for the new project.*/
        DirectoryDialog dlg = new DirectoryDialog(getShell());
        dlg.setFilterPath(projectParentPathText.getText());
        dlg.setText("New ChibiOS/RT Project");
        dlg.setMessage("Select the parent directory for the new ChibiOS/RT application project.");
        String parent = dlg.open();
        if (parent != null) {
          projectParentPathText.setText(parent);
          projectPageUpdated();
        }
      }
    });
    btnBrowse.setText("Browse...");
    
    /* Layout row 4.*/
    lbl3 = new Label(container, SWT.NONE);
    lbl3.setText("Final project path:");
    projectFinalPathText = new Text(container, SWT.BORDER);
    projectFinalPathText.setEditable(false);
    projectFinalPathText.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
    new Label(container, SWT.NONE);

    /* Layout row 5.*/
    lbl4 = new Label(container, SWT.NONE);
    lbl4.setText("Target platform:");
    platformCombo = new Combo(container, SWT.READ_ONLY);
    platformCombo.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false, 1, 1));
    new Label(container, SWT.NONE);

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
    btnBrowse.setEnabled(false);

    /* Retrieving the resource path of the processors.xml file. */
    String fpath;
    try {
      Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
      IPath path = new Path("resources/gencfg/processors/processors.xml");
      fpath = FileLocator.toFileURL(FileLocator.find(bundle, path, null)).getFile();
    } catch (IOException e) {
      e.printStackTrace();
      return;
    }

    /* DOM tree creation. */
    SAXBuilder builder = new SAXBuilder();
    Document document;
    try {
      document = builder.build(fpath);
    } catch (JDOMException e) {
      e.printStackTrace();
      return;
    } catch (IOException e) {
      e.printStackTrace();
      return;
    }

    /* Parsing the content of the processors.xml file in order to populate the
       panel objects.*/
    Element root = document.getRootElement();
    for (Element processor : root.getChildren("processor")) {
      String class_attr = processor.getAttributeValue("class", "");
      if (class_attr.compareToIgnoreCase("build") == 0) { 
        String name = processor.getAttributeValue("target", "internal error");
        platformCombo.add(name);
      }
    }
    platformCombo.select(0);

    /* Update checks on the fields.*/
    projectPageUpdated();

    /* Focus on the first editable field.*/
    projectNameText.setFocus();
  }

  public String getProjectName() {

    return projectNameText.getText();
  }

  public String getProjectParentPath() {

    return projectParentPathText.getText();
  }

  public String getFinalProjectPath() {

    if (useCustomPathButton.getSelection())
      return null;
    return projectFinalPathText.getText();
  }

  public String getPlatform() {

    return platformCombo.getText();
  }

  private void projectPageUpdated() {

    updateFinalProjectPathText();

    /* Checks the project location.*/
    File path = new File(projectParentPathText.getText());
    if (!path.exists()) {
      updateStatus("Project path is not valid.");
      return;
    }
    if (!path.isDirectory()) {
      updateStatus("Project path is a directory.");
      return;
    }

    /* Checks the project name.*/
    String name = projectNameText.getText();
    if (!isValidFilename(name)) {
      updateStatus("Invalid project name.");
      return;
    }

    /* Checks if the project already exists in the workspace.*/
    IProject project = ResourcesPlugin.getWorkspace().getRoot().getProject(name);
    if (project.exists()) {
      updateStatus("Project exists.");
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
