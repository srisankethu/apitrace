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


#ifndef _RENDERTAB_HPP_
#define _RENDERTAB_HPP_

#include <QCheckBox>
#include <QPixmap>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

#include "imageview.hpp"
#include "glframe_retrace_interface.hpp"
#include "uimodel.hpp"

namespace glretrace {

class RenderTab : public QSplitter {
  Q_OBJECT
 public:
  explicit RenderTab(QWidget *parent = 0);
  virtual ~RenderTab();

  void setModel(UiModel *mdl);

 private:
  RenderTargetType getRenderTargetType();
  RenderOptions getRenderOptions();

 signals:
  void needRenderTarget(int idx, RenderOptions opt, RenderTargetType rtt);

 public slots:
  void setRenderImage(QPixmap p);
  void requestRenderTarget(int idx);
  void updateRender();

 protected:
  static const char *handleStyleSheet;
  QWidget *controlsArea;
  QVBoxLayout *controlsLayout;
  QCheckBox *clearBox;
  QCheckBox *stopBox;
  QCheckBox *highlightBox;
  ImageView *view;
  int currentIndex;

  // Model
  UiModel* model;
};

}  // namespace glretrace

#endif  // _RENDERTAB_HPP_
