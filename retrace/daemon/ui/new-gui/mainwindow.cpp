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

#include "mainwindow.hpp"

#include <QGuiApplication>
#include <QRect>
#include <QScreen>
#include <QSizePolicy>
#include <QStatusBar>

using glretrace::GraphWindow;
using glretrace::MainWindow;
using glretrace::OpenDialog;
using glretrace::UiModel;

// From Qt style sheets examples "Customizing QSplitter"
const char *
MainWindow::handleStyleSheet =
    "QSplitter::handle {\n"
    "  image: url(:images/drag-me.png);\n"
    "}\n"
    "\n"
    "QSplitter::handle:horizontal {\n"
    "  width: 11px;\n"
    "}\n"
    "\n"
    "QSplitter::handle:vertical {\n"
    "  height: 2px;\n"
    "}\n"
    "\n"
    "QSplitter::handle:pressed {\n"
    "  image: url(:images/drag-me-pressed.png);\n"
    "}";

MainWindow::MainWindow() {
  model = NULL;

  // UI setup
  // Create a placeholder widget
  splitter = new QSplitter(Qt::Vertical, this);
  setCentralWidget(splitter);
  splitter->setStyleSheet(handleStyleSheet);

  // Graph
  graphArea = new QWidget(this);
  graphAreaLayout = new QVBoxLayout(graphArea);
  graphArea->setLayout(graphAreaLayout);
  graph = new GraphWindow();
  graphContainer = QWidget::createWindowContainer(graph, this);
  graphContainer->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
  graphAreaLayout->addWidget(graphContainer);

  // Tool bar.
  metricsBar = new QWidget(this);
  metricsBarLayout = new QHBoxLayout(metricsBar);
  metricsBar->setLayout(metricsBarLayout);
  ylabel = new QLabel("Vertical Metric:", this);
  ylabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  xlabel = new QLabel("Horizontal Metric:", this);
  xlabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  metrics << "No Metric" << "Pixel Shader Active Time" <<
                          "Fragment Shader Active Time";
  yComboBox = new QComboBox(this);
  yComboBox->addItems(metrics);
  xComboBox = new QComboBox(this);
  xComboBox->addItems(metrics);
  filterLabel = new QLabel("Metrics Filter:", this);
  filterLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  filter = new QLineEdit(this);
  filter->setClearButtonEnabled(true);
  metricsBarLayout->addWidget(filterLabel);
  metricsBarLayout->addWidget(filter);
  metricsBarLayout->addWidget(ylabel);
  metricsBarLayout->addWidget(yComboBox);
  metricsBarLayout->addWidget(xlabel);
  metricsBarLayout->addWidget(xComboBox);
  graphAreaLayout->addWidget(metricsBar);
  splitter->addWidget(graphArea);

  // Tab Widget
  tabs = new QTabWidget(this);
  tabs->setSizePolicy(QSizePolicy::Expanding,
                      QSizePolicy::Expanding);
  tabs->addTab(new QWidget(this), "Shaders");
  tabs->addTab(new QWidget(this), "RenderTarget");
  tabs->addTab(new QWidget(this), "API Calls");
  tabs->addTab(new QWidget(this), "Metrics");
  splitter->addWidget(tabs);

  // Window finalization.
  QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
  screenGeometry.moveTo(0, 0);
  setGeometry(screenGeometry);
  setWindowTitle("Frame Retrace");
  statusBar()->showMessage("Ready");

  // Create the dialog.
  dialog = new OpenDialog(this);
  show();
  dialog->exec();
}

MainWindow::~MainWindow() {
  delete graph;
}

void
MainWindow::setModel(UiModel* mdl) {
  model = mdl;
  dialog->setModel(mdl);
}
