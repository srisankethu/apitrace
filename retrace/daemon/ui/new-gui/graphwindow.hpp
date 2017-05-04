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


#ifndef _GRAPHWINDOW_HPP_
#define _GRAPHWINDOW_HPP_

#include <QList>
#include <QMouseEvent>
#include <QOpenGLWindow>
#include <QPointF>
#include <QWheelEvent>
#include <QWindow>

#include <vector>

#include "glframe_bargraph.hpp"
#include "glframe_qselection.hpp"

namespace glretrace {

class GraphWindow : public QOpenGLWindow,
                    public glretrace::BarGraphSubscriber {
  Q_OBJECT
 public:
  // UpdateBehavior options:
  // NoPartialUpdate (0):  this class paints whole window.
  // PartialUpdateBlit (1): class paints only some of window with blit
  // PartialUpdateBlend (2): only some of window, but alpha blend
  // rather than blit
  // http://doc.qt.io/qt-5/qopenglwindow.html#UpdateBehavior-enum
  explicit GraphWindow(UpdateBehavior updateBehavior = NoPartialUpdate,
                       QWindow *parent = 0);
  virtual ~GraphWindow();

  // OpenGL functions
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  // Graph-specific functions
  void setBars(QVector<BarMetrics> bars);
  void mouseWheel(int degrees, float zoom_point_x);
  void mouseDrag(float x1, float y1, float x2, float y2);
  void setTranslation(float value);
  void setSelection(QList<int> sel);

  // QWindow functions
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void wheelEvent(QWheelEvent *e);

  // Subscriber function
  void onBarSelect(const std::vector<int> selection);

 public slots:
  void printSelection(QList<int> sel);

 signals:
  void translationChanged(float value);
  void zoomChanged(float value);
  void printMessage(QString msg);
  void barSelect(QList<int> sel);

 protected:
  BarGraphRenderer *renderer;
  QPointF startPos;
  float zoom;  // range [1.0..]
  float translation;  // after zoom, translate to keep zoom point in
                      // place [0..1.0] coordinate system
  bool clicked;
  bool shift;
  QSelection selection;
  std::vector<float> savedArea;
};

}  // namespace glretrace

#endif  // _GRAPHWINDOW_HPP_
