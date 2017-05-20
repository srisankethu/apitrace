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


#ifndef _MAINWINDOW_HPP_
#define _MAINWINDOW_HPP_

#include <QAction>
#include <QComboBox>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QSplitter>
#include <QStringList>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QWidget>

#include "opendialog.hpp"
#include "graphwindow.hpp"
#include "uimodel.hpp"
#include "glframe_bargraph.hpp"
#include "shadertab.hpp"
#include "zoombar.hpp"
#include "tabwidget.hpp"
#include "apitab.hpp"
#include "rendertab.hpp"

namespace glretrace {

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow();
  ~MainWindow();

  UiModel* getModel() { return model; }
  void setModel(UiModel* mdl);

  void execDialog();

 private:
  void connectSignals();

 public slots:
  void initMetricsTools(QStringList names);
  void openFile(QString filename, int frameCount, QString hostname);
  void requestGraphData(QString name);
  void updateProgress(int count);
  void propagateFileData();
  void updateGraphData(QString name, QVector<float> data);

  void zoomIn();
  void zoomOut();

  void printMessage(QString msg);
  void errorMessage(QString text, QString details, bool fatal = false);

 protected:
  // Ui objects
  QSplitter *splitter;
  static const char *handleStyleSheet;
  OpenDialog *dialog;
  QWidget *graphArea;
  QVBoxLayout *graphAreaLayout;
  GraphWindow *graph;
  QWidget *graphContainer;
  ZoomBar *zoomBar;
  QWidget *metricsBar;
  QHBoxLayout *metricsBarLayout;
  QLabel *ylabel;
  QLabel *xlabel;
  QStringListModel *metrics;
  QSortFilterProxyModel *metricsProxy;
  QComboBox *yComboBox;
  QComboBox *xComboBox;
  QLabel *filterLabel;
  QLineEdit *filter;
  TabWidget *tabs;
  ShaderTab *shaderTab;
  ApiTab *apiTab;
  RenderTab *renderTab;
  QProgressBar *pbar;

  // Model
  UiModel* model;

  // Gui Data
  // Later, we'll move this so it better corresponds to
  // the model-view-controller framework.
  QVector<BarMetrics> graphData;
};

}  // namespace glretrace

#endif  // _MAINWINDOW_HPP_
