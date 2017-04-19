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


#ifndef _ZOOMBAR_HPP_
#define _ZOOMBAR_HPP_

#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QScrollBar>

namespace glretrace {

class ZoomBar : public QWidget {
  Q_OBJECT
 public:
  explicit ZoomBar(QWidget *parent = 0);
  virtual ~ZoomBar();

 signals:
  void zoomIn();
  void zoomOut();
  void translationChanged(float value);

 public slots:
  void positionHandle();
  void setZoom(float value);
  void setTranslation(float value);
  void updateTranslation(int value);

 protected:
  QHBoxLayout *layout;
  QToolButton *zoomInButton;
  QScrollBar *scroll;
  QToolButton *zoomOutButton;
  static const char *scrollStyleSheet;
  float zoom;
  float translation;
};

}  // namespace glretrace

#endif  // _ZOOMBAR_HPP_
