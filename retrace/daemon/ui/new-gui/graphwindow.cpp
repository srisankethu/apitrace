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

#include "graphwindow.hpp"

#include <QVector>

#include <algorithm>
#include <set>

using glretrace::BarGraphRenderer;
using glretrace::GraphWindow;

GraphWindow::GraphWindow(UpdateBehavior updateBehavior,
                         QWindow* parent) :
                         QOpenGLWindow(updateBehavior, parent),
                         renderer(NULL),
                         zoom(1.0),
                         translation(0.0),
                         clicked(false),
                         shift(false),
                         savedArea(4) {
  startPos = QPointF(-1.0, -1.0);
  connect(this, &GraphWindow::barSelect,
          &selection, &QSelection::select);
  connect(&selection, &QSelection::onSelect,
          this, &GraphWindow::setSelection);
  connect(this, &GraphWindow::barSelect,
          this, &GraphWindow::printSelection);
}

GraphWindow::~GraphWindow() {
  delete renderer;
}

void
GraphWindow::initializeGL() {
  delete renderer;
  renderer = new BarGraphRenderer();
  renderer->subscribe(this);
}

void
GraphWindow::paintGL() {
  if (renderer) {
    renderer->setMouseArea(savedArea[0],
                           savedArea[1],
                           savedArea[2],
                           savedArea[3]);

    renderer->setZoom(zoom, translation);

    if (clicked) {
      renderer->selectMouseArea(shift);
      renderer->setMouseArea(0, 0, 0, 0); // Act to clear?
      savedArea[0] = -1.0;
      savedArea[1] = -1.0;
      savedArea[2] = -1.0;
      savedArea[3] = -1.0;
      clicked = false;
      shift = false;
    }

    renderer->render();
  }
}

void
GraphWindow::resizeGL(int w, int h) {
}

void
GraphWindow::setBars(QVector<BarMetrics> bars) {
  if (renderer) {
    renderer->setBars(bars.toStdVector());
    update();
  }
}

void
GraphWindow::onBarSelect(const std::vector<int> selection) {
  // emit printMessage("bar select");
  QVector<int> vec = QVector<int>::fromStdVector(selection);
  emit barSelect(vec.toList());
  if (selection.size() >= 1)
    emit firstSelected(selection[0]);
}

void
GraphWindow::mousePressEvent(QMouseEvent *e) {
  // emit printMessage("press");
  startPos = QPointF(e->pos());
  QPointF winSize = QPointF(width(), height());
  startPos.setX(startPos.x()/winSize.x());
  startPos.setY((winSize.y()-startPos.y())/winSize.y());
  mouseDrag(startPos.x(), startPos.y(),
            startPos.x(), startPos.y());
  update();
}

void
GraphWindow::mouseMoveEvent(QMouseEvent *e) {
  if (e->buttons() & Qt::LeftButton) {
    // emit printMessage("move");
    QPointF endPos, mousePos, winSize;
    winSize = QPointF(width(), height());
    mousePos = QPointF(e->pos());
    endPos.setX(mousePos.x()/winSize.x());
    endPos.setY((winSize.y()-mousePos.y())/winSize.y());
    mouseDrag(startPos.x(), startPos.y(),
              endPos.x(), endPos.y());
    update();
  }
}

void
GraphWindow::mouseReleaseEvent(QMouseEvent *e) {
  // emit printMessage("release");
  clicked = true;
  shift = e->modifiers() & Qt::ShiftModifier;
  update();
}

void
GraphWindow::wheelEvent(QWheelEvent *e) {
  // emit printMessage("wheel");
  float wheelx = 1.0;
  wheelx = ((float) e->x())/((float) width());
  mouseWheel(((float) e->angleDelta().y())/5, wheelx);
  update();
}

void
GraphWindow::mouseWheel(int degrees, float zoom_point_x) {
  float new_zoom = zoom + degrees / 360.0;
  if (new_zoom < 1.0) {
    new_zoom = 1.0;
  }

  float unzoomed_point = (zoom_point_x - translation) / zoom;
  float new_translate = zoom_point_x - unzoomed_point * new_zoom;
  // float new_translate = zoom_point_x * (1 - new_zoom);

  if (new_translate < 1 - new_zoom)
    new_translate = 1 - new_zoom;
  else if (new_translate > 0)
    new_translate = 0;

  translation = new_translate;
  emit translationChanged(translation);
  zoom = new_zoom;
  emit zoomChanged(zoom);

  update();
}

void
GraphWindow::mouseDrag(float x1, float y1, float x2, float y2) {
  savedArea[0] = std::min(x1, x2);
  savedArea[1] = std::min(y1, y2);
  savedArea[2] = std::max(x1, x2);
  savedArea[3] = std::max(y1, y2);

  update();
}

void
GraphWindow::setTranslation(float value) {
  translation = value;
  update();
}

void
GraphWindow::setSelection(QList<int> sel) {
  std::set<int> s;
  for (auto i : sel)
    s.insert(i);
  renderer->setSelection(s);
}

void
GraphWindow::printSelection(QList<int> sel) {
  QString msg;
  for (int i = 0; i < sel.length(); i++) {
    msg.append(QString::number(sel.at(i)));
    if (i != sel.length() - 1)
      msg.append(", ");
  }
  emit printMessage(msg);
}

void
GraphWindow::setOneSelection(int sel) {
  QList<int> selList;
  selList.append(sel);
  selection.select(selList);
  printSelection(selList);
  update();
}
