/**************************************************************************
 *
 * Copyright 2017 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Laura Ekstrand <laura@jlekstrand.net>
 **************************************************************************/

#include "opendialog.hpp"

#include <QCoreApplication>
#include <QFileDialog>
#include <QIcon>
#include <QPixmap>
#include <QSize>
#include <QString>

using glretrace::ImageView;
using glretrace::OpenDialog;
using glretrace::UiModel;

OpenDialog::OpenDialog(QWidget *parent) : QDialog(parent) {
  model = NULL;

  // UI setup
  layout = new QVBoxLayout(this);
  setLayout(layout);
  setModal(true);  // Defer to main GUI.

  view = new ImageView(this);
  view->setImage(QPixmap(":/images/fr.png"));
  layout->addWidget(view);

  // Controls area
  controls = new QWidget(this);
  controlsLayout = new QGridLayout(controls);
  controls->setLayout(controlsLayout);

  // Filename area
  fileLabel = new QLabel("Filename:", controls);
  fileLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  controlsLayout->addWidget(fileLabel, 0, 0);
  lineEdit = new QLineEdit(controls);
  lineEditCompleter = new QCompleter(lineEdit);
  lineEditCompleter->setCompletionMode(
    QCompleter::UnfilteredPopupCompletion);
  lineEditModel = new QFileSystemModel(lineEdit);
  lineEditModel->setRootPath("/home");
  lineEditCompleter->setModel(lineEditModel);
  lineEdit->setCompleter(lineEditCompleter);
  controlsLayout->addWidget(lineEdit, 0, 1);
  fileButton = new QToolButton(controls);
  fileButton->setIcon(QIcon(":/images/document-open.png"));
  fileButton->setToolTip("Open trace");
  fileButton->setIconSize(QSize(24, 24));
  connect(fileButton, SIGNAL(clicked()), this, SLOT(getFilename()));
  controlsLayout->addWidget(fileButton, 0, 2);

  // Frame area.
  frameLabel = new QLabel("Frame:", controls);
  frameLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  controlsLayout->addWidget(frameLabel, 1, 0);
  frameBox = new QSpinBox(controls);
  frameBox->setMinimum(0);
  frameBox->setMaximum(10000);
  frameBox->setValue(100);
  frameBox->setAlignment(Qt::AlignRight);
  frameBox->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  controlsLayout->addWidget(frameBox, 1, 1);

  // Host area.
  hostLabel = new QLabel("Host:", controls);
  hostLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  controlsLayout->addWidget(hostLabel, 2, 0);
  hostEdit = new QLineEdit("localhost", controls);
  controlsLayout->addWidget(hostEdit, 2, 1, 1, -1);

  // Dialog bottom.
  layout->addWidget(controls);
  dialogButtons = new QDialogButtonBox(QDialogButtonBox::Cancel |
                                       QDialogButtonBox::Ok);
  connect(dialogButtons, SIGNAL(rejected()), QCoreApplication::instance(),
          SLOT(quit()));
  connect(dialogButtons, SIGNAL(accepted()), this, SLOT(openFile()));
  layout->addWidget(dialogButtons);

  setWindowTitle("Set Trace File and Frame Number");
  setGeometry(300, 200, 600, 500);
}

OpenDialog::~OpenDialog() {
}

void
OpenDialog::getFilename() {
    QString currentFname = lineEdit->text();
    if (currentFname.isEmpty()) {
        currentFname = "/home";
    }
    QString fname = QFileDialog::getOpenFileName(
                      this, "Open File", currentFname,
                      "Trace files (*.trace)");
    // If user presses cancel, string will be null
    if (!fname.isEmpty())
        lineEdit->setText(fname);
}

void
OpenDialog::openFile() {
  if (model == NULL)
    return;

  if (!model->setFrame(lineEdit->text(), frameBox->value(),
                       hostEdit->text())) {
    // There was an error.

  } else {
    // No-op for now
    close();
  }
}
