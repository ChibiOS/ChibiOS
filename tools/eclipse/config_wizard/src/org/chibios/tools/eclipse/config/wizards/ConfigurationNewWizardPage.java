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

import java.io.IOException;

import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Combo;

import org.osgi.framework.Bundle;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.JDOMException;
import org.jdom2.input.SAXBuilder;

import config_wizard.Activator;

/**
 * The "New" wizard page allows setting the container for the new file as well
 * as the file name. The page will only accept file name without the extension
 * OR with the extension that matches the expected one (chcfg).
 */

public class ConfigurationNewWizardPage extends WizardPage {

  private ISelection selection;
  private IContainer resourceContainer;

  private Document processorsDocument;
  private String currentDefaultDataFile;

  private Composite container;
  private Combo configurationTemplatesCombo;
  private Text confProjectFilenameText;

  /**
   * Constructor for SampleNewWizardPage.
   */
  public ConfigurationNewWizardPage(ISelection selection) {

    super("wizardPage");
    setTitle("ChibiOS/RT Configuration Project File");
    setDescription("This wizard creates a ChibiOS/RT configuration resource. Configuration resources allow to generate project resources starting from high level descriptions written in XML.");
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
    layout.numColumns = 2;
    layout.verticalSpacing = 9;

    Label lbl1 = new Label(container, SWT.NULL);
    lbl1.setText("Configuration template:");

    configurationTemplatesCombo = new Combo(container, SWT.READ_ONLY);
    configurationTemplatesCombo.setLayoutData(new GridData(SWT.FILL,
                                                           SWT.CENTER, true,
                                                           false, 1, 1));

    Label lbl2 = new Label(container, SWT.NULL);
    lbl2.setText("Configuration project filename:");

    confProjectFilenameText = new Text(container, SWT.BORDER | SWT.SINGLE);
    confProjectFilenameText.setText("config.chcfg");
    confProjectFilenameText
        .setLayoutData(new GridData(GridData.FILL_HORIZONTAL));
    confProjectFilenameText.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        confProjectFilenameUpdated();
      }
    });

    /* Note, it must stay after the creation of the text fields. */
    configurationTemplatesCombo.addModifyListener(new ModifyListener() {
      public void modifyText(ModifyEvent e) {
        Element processor = getSelectedTemplate();
        String basefilename = processor.getChildText("basefilename");
        confProjectFilenameText.setText(basefilename.concat(".chcfg"));
        currentDefaultDataFile = processor.getChildText("default");
      }
    });

    initialize();
    setControl(container);
  }

  public String getContainerName() {

    return resourceContainer.getFullPath().toString();
  }

  public String getProjectFileName() {

    return confProjectFilenameText.getText();
  }

  public String getDefaultDataFile() {

    return currentDefaultDataFile;
  }

  public boolean canFinish() {

    return container.getEnabled();
  }

  /**
   * Tests if the current workbench selection is a suitable container to use.
   */
  private void initialize() {

    if (selection != null && selection.isEmpty() == false
        && selection instanceof IStructuredSelection) {
      IStructuredSelection ssel = (IStructuredSelection) selection;
      if (ssel.size() > 1)
        return;
      Object obj = ssel.getFirstElement();
      if (obj instanceof IResource) {
        if (obj instanceof IContainer)
          resourceContainer = (IContainer) obj;
        else
          resourceContainer = ((IResource) obj).getParent();
      }
    }
    else {
      MessageDialog.openError(getShell(),
                              "Error", "Container for the resource not selected.");
      container.setEnabled(false);
      updateStatus("Container for the resource not selected.");
    }

    /* Retrieving the resource path of the processors.xml file. */
    String fpath;
    try {
      Bundle bundle = Platform.getBundle(Activator.PLUGIN_ID);
      IPath path = new Path("resources/gencfg/processors/processors.xml");
      fpath = FileLocator.toFileURL(FileLocator.find(bundle, path, null))
          .getFile();
    } catch (IOException e) {
      e.printStackTrace();
      return;
    }

    /* DOM tree creation. */
    SAXBuilder builder = new SAXBuilder();
    try {
      processorsDocument = builder.build(fpath);
    } catch (JDOMException e) {
      e.printStackTrace();
      return;
    } catch (IOException e) {
      e.printStackTrace();
      return;
    }

    /* Parsing the content of the processors.xml file in order to populate the
       panel objects.*/
    Element root = processorsDocument.getRootElement();
    for (Element processor : root.getChildren("processor")) {
      String name = processor.getChildText("name");
      configurationTemplatesCombo.add(name);
    }
    configurationTemplatesCombo.select(0);
  }

  /**
   * Checks the content of the confProjectFilenameText field.
   */
  private void confProjectFilenameUpdated() {
    String fileName = getProjectFileName();

    if (fileName.length() == 0) {
      updateStatus("File name must be specified");
      return;
    }
    if (fileName.replace('\\', '/').indexOf('/', 1) > 0) {
      updateStatus("File name must be valid");
      return;
    }
    int dotLoc = fileName.lastIndexOf('.');
    if (dotLoc != -1) {
      String ext = fileName.substring(dotLoc + 1);
      if (ext.equalsIgnoreCase("chcfg") == false) {
        updateStatus("Configuration project filename extension must be \"chcfg\"");
        return;
      }
    }
    updateStatus(null);
  }

  /**
   * Returns the XML Element associated to the current selection in the combo
   * box.
   * 
   * @return An Element Object.
   */
  private Element getSelectedTemplate() {

    for (Element processor : processorsDocument.getRootElement()
        .getChildren("processor")) {
      String name = processor.getChildText("name");
      String item = configurationTemplatesCombo
          .getItem(configurationTemplatesCombo.getSelectionIndex());
      if (name.compareTo(item) == 0) {
        return processor;
      }
    }
    return null;
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
}
