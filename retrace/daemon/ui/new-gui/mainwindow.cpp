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
#include <QMessageBox>
#include <QRect>
#include <QScreen>
#include <QSizePolicy>
#include <QStatusBar>
#include <QTextStream>

using glretrace::GraphWindow;
using glretrace::MainWindow;
using glretrace::OpenDialog;
using glretrace::TabWidget;
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
  graphAreaLayout->setSpacing(0);
  graphArea->setLayout(graphAreaLayout);
  graph = new GraphWindow();
  graphContainer = QWidget::createWindowContainer(graph, this);
  graphContainer->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
  graphAreaLayout->addWidget(graphContainer);
  zoomBar = new ZoomBar(this);
  graphAreaLayout->addWidget(zoomBar);

  // Tool bar.
  metricsBar = new QWidget(this);
  metricsBarLayout = new QHBoxLayout(metricsBar);
  metricsBar->setLayout(metricsBarLayout);
  metrics = new QStringListModel(this);
  metricsProxy = new QSortFilterProxyModel(this);
  ylabel = new QLabel("Vertical Metric:", this);
  ylabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  xlabel = new QLabel("Horizontal Metric:", this);
  xlabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  yComboBox = new QComboBox(this);
  yComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  xComboBox = new QComboBox(this);
  xComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
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
  tabs = new TabWidget(this);
  tabs->setSizePolicy(QSizePolicy::Expanding,
                      QSizePolicy::Expanding);
  shaderTab = new ShaderTab(this);
  tabs->addTab(shaderTab, "Shaders");
  // Hide Shaders tab until Shaders data exists.
  tabs->setTabVisible(shaderTab, false);
  tabs->addTab(new QWidget(this), "RenderTarget");
  apiTab = new ApiTab(this);
  tabs->addTab(apiTab, "API Calls");
  tabs->addTab(new QWidget(this), "Metrics");
  splitter->addWidget(tabs);

  // Progress bar
  pbar = new QProgressBar(this);
  statusBar()->addPermanentWidget(pbar);
  pbar->setVisible(false);

  // Open dialog
  dialog = new OpenDialog(this);

  // Window finalization.
  QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
  screenGeometry.moveTo(0, 0);
  setGeometry(screenGeometry);
  setWindowTitle("Frame Retrace");
  statusBar()->showMessage("Ready");
  connectSignals();
}

MainWindow::~MainWindow() {
  delete graph;
}

void
MainWindow::connectSignals() {
  connect(dialog, &OpenDialog::fileOpened,
          this, &MainWindow::openFile);
  connect(filter, &QLineEdit::textChanged, metricsProxy,
          static_cast<void (QSortFilterProxyModel::*)(const QString&)>
          (&QSortFilterProxyModel::setFilterRegExp));
  connect(yComboBox, static_cast<void(QComboBox::*)(int)>
                     (&QComboBox::currentIndexChanged),
          [=]() { this->requestGraphData(yComboBox->currentText()); });
  connect(xComboBox, static_cast<void(QComboBox::*)(int)>
                     (&QComboBox::currentIndexChanged),
          [=]() { this->requestGraphData(xComboBox->currentText()); });
  connect(zoomBar, &ZoomBar::zoomIn, this, &MainWindow::zoomIn);
  connect(zoomBar, &ZoomBar::zoomOut, this, &MainWindow::zoomOut);
  connect(graph, &GraphWindow::translationChanged,
          zoomBar, &ZoomBar::setTranslation);
  connect(graph, &GraphWindow::zoomChanged,
          zoomBar, &ZoomBar::setZoom);
  connect(zoomBar, &ZoomBar::translationChanged,
          graph, &GraphWindow::setTranslation);
  connect(graph, &GraphWindow::printMessage,
          this, &MainWindow::printMessage);
  connect(shaderTab, &ShaderTab::printMessage,
          this, &MainWindow::printMessage);
  connect(shaderTab, &ShaderTab::shaderActivated,
          graph, &GraphWindow::setOneSelection);
  connect(graph, &GraphWindow::firstSelected,
          shaderTab, &ShaderTab::activateShader);
  connect(apiTab, &ApiTab::printMessage,
          this, &MainWindow::printMessage);
  connect(apiTab, &ApiTab::shaderActivated,
          graph, &GraphWindow::setOneSelection);
  connect(graph, &GraphWindow::firstSelected,
          apiTab, &ApiTab::activateShader);
  connect(apiTab, &ApiTab::shaderActivated,
          shaderTab, &ShaderTab::activateShader);
  connect(shaderTab, &ShaderTab::shaderActivated,
          apiTab, &ApiTab::activateShader);
}

void
MainWindow::execDialog() {
  // Open the dialog.
  show();
  dialog->exec();
}

void
MainWindow::setModel(UiModel* mdl) {
  model = mdl;
  dialog->setModel(model);
  shaderTab->setModel(model);
  apiTab->setModel(model);
  connect(model, &UiModel::frameCountChanged,
          this, &MainWindow::updateProgress);
  connect(model, &UiModel::fileLoadFinished,
          this, &MainWindow::propagateFileData);
  connect(model, &UiModel::metricNamesReceived,
          this, &MainWindow::initMetricsTools);
  connect(model, &UiModel::graphDataReceived,
          this, &MainWindow::updateGraphData);
  connect(model, &UiModel::generalError,
          this, &MainWindow::errorMessage);
  connect(model, &UiModel::printMessage,
          this, &MainWindow::printMessage);
  connect(model, &UiModel::hasShaders,
          [=]() { tabs->setTabVisible(shaderTab, true); });
}

void
MainWindow::openFile(QString filename, int frameCount,
                     QString hostname) {
  pbar->setMinimum(0);
  pbar->setMaximum(frameCount);
  pbar->setVisible(true);
}

void
MainWindow::updateProgress(int count) {
  pbar->setValue(count);
  QString message;
  QTextStream(&message) << "Retracing frame "
                        << QString::number(count);
  statusBar()->showMessage(message);
}

void
MainWindow::propagateFileData() {
  pbar->setVisible(false);
  statusBar()->clearMessage();
}

void
MainWindow::initMetricsTools(QStringList names) {
  metrics->setStringList(QStringList("None") + names);

  // The proxy model makes filtering easy!
  metricsProxy->setSourceModel(metrics);
  yComboBox->setModel(metricsProxy);
  yComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  xComboBox->setModel(metricsProxy);
  xComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
}

void
MainWindow::requestGraphData(QString name) {
  model->requestGraphData(name);
}

void
MainWindow::updateGraphData(QString name, QVector<float> data) {
  // In the future, graph should just take two std::vectors of data
  // rather than BarMetrics, so updates can be implicitly shared.
  if (name == QString()) {
    name = QString("None");
  }

  if (graphData.isEmpty()) {
     for (int i = 0; i < data.size(); i++) {
       BarMetrics temp;
       temp.height = data[i];
       temp.width = data[i];
       graphData.append(temp);
     }
  } else if ((name == yComboBox->currentText()) &&
             (name == xComboBox->currentText())) {
     // No way of knowing which dimension should be updated,
     // so we update both.
     graphData.clear(); // As of Qt 5.7, this preserves capacity.
     for (int i = 0; i < data.size(); i++) {
       BarMetrics temp;
       temp.height = data[i];
       temp.width = data[i];
       graphData.append(temp);
     }
  } else if (name == yComboBox->currentText()) {
    assert(data.size() == graphData.size());
    for (int i = 0; i < data.size(); i++) {
      graphData[i].height = data[i];
    }
  } else if (name == xComboBox->currentText()) {
    assert(data.size() == graphData.size());
    for (int i = 0; i < data.size(); i++) {
      graphData[i].width = data[i];
    }
  }

  graph->setBars(graphData);
}

void
MainWindow::zoomIn() {
  statusBar()->showMessage("Zoom In");
  graph->mouseWheel(45, 0.5);
}

void
MainWindow::zoomOut() {
  statusBar()->showMessage("Zoom Out");
  graph->mouseWheel(-45, 0.5);
}

void
MainWindow::printMessage(QString msg) {
  statusBar()->showMessage(msg);
}

void
MainWindow::errorMessage(QString text, QString details, bool fatal) {
  QMessageBox msgBox (this);
  msgBox.setText(text);
  msgBox.setDetailedText(details);
  if (fatal) {
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Frame Retrace: Fatal Error");
  }
  else {
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Frame Retrace: Warning");
  }
  msgBox.setStandardButtons(QMessageBox::Ok);
  msgBox.setDefaultButton(QMessageBox::Ok);

  msgBox.exec();

  if (fatal)
    QCoreApplication::instance()->quit();
}
