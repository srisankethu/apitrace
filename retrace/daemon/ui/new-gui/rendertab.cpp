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

#include "rendertab.hpp"

#include <QSizePolicy>

using glretrace::RenderTab;
using glretrace::RenderTargetType;
using glretrace::RenderOptions;

// From Qt style sheets examples "Customizing QSplitter"
const char *
RenderTab::handleStyleSheet =
    "QSplitter::handle {\n"
    "  image: url(:images/drag-me-transposed.png);\n"
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
    "  image: url(:images/drag-me-pressed-transposed.png);\n"
    "}";

RenderTab::RenderTab(QWidget *parent) : QSplitter(parent),
                                        currentIndex(0) {
  setOrientation(Qt::Horizontal);
  setStyleSheet(handleStyleSheet);

  controlsArea = new QWidget(this);
  controlsLayout = new QVBoxLayout(controlsArea);
  controlsArea->setLayout(controlsLayout);
  clearBox = new QCheckBox("Clear before render", controlsArea);
  stopBox = new QCheckBox("Stop at render", controlsArea);
  highlightBox = new QCheckBox("Highlight selected render",
                               controlsArea);
  controlsLayout->addWidget(clearBox);
  controlsLayout->addWidget(stopBox);
  controlsLayout->addWidget(highlightBox);
  controlsLayout->addStretch(1);
  controlsArea->setSizePolicy(QSizePolicy::Maximum,
                              QSizePolicy::Maximum);
  addWidget(controlsArea);

  view = new ImageView(this);
  view->setImage(QPixmap(":/images/no-render-target.png"));
  view->setMinimumSize(QSize(480, 360));
  addWidget(view);

  connect(clearBox, &QCheckBox::clicked,
          this, &RenderTab::updateRender);
  connect(stopBox, &QCheckBox::clicked,
          this, &RenderTab::updateRender);
  connect(highlightBox, &QCheckBox::clicked,
          this, &RenderTab::updateRender);
}

RenderTab::~RenderTab() {
}

void
RenderTab::setRenderImage(QPixmap p) {
  view->setImage(p);
}

RenderTargetType
RenderTab::getRenderTargetType() {
  if (highlightBox->isChecked())
    return HIGHLIGHT_RENDER;
  else
    return NORMAL_RENDER;
}

RenderOptions
RenderTab::getRenderOptions() {
  RenderOptions opt = DEFAULT_RENDER;
  if (clearBox->isChecked())
    opt = (RenderOptions) (opt | CLEAR_BEFORE_RENDER);
  if (stopBox->isChecked())
    opt = (RenderOptions) (opt | STOP_AT_RENDER);

  return opt;
}

void
RenderTab::setModel(UiModel *mdl) {
  model = mdl;
  connect(this, &RenderTab::needRenderTarget,
          model, &UiModel::requestRenderTarget);
}

void
RenderTab::requestRenderTarget(int idx) {
  currentIndex = idx;
  emit needRenderTarget(idx, getRenderOptions(), getRenderTargetType());
}

void
RenderTab::updateRender() {
  requestRenderTarget(currentIndex);
}
