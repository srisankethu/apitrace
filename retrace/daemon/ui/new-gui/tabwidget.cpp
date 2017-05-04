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

#include "tabwidget.hpp"

using glretrace::TabWidget;

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
  mask.resize(10);
  tabCount = 0;
}

TabWidget::~TabWidget() {
}

int
TabWidget::addTab(QWidget *page, const QString &label) {
  // Check to see if this widget is already in here.
  // If it's in there, it's unhidden.
  int idx = QTabWidget::indexOf(page);
  if (idx != -1)
    return idx;

  // Add the widget
  idx = QTabWidget::addTab(page, label);
  tabs.append(page);
  names.append(label);
  mask.setBit(tabCount, true);
  tabCount++;

  // Resize the mask, if necessary.
  if (tabCount >= mask.size()) {
    mask.resize(2 * mask.size());
  }

  return idx;
}

void
TabWidget::setTabVisible(QWidget *tab, bool visible) {
  if (visible == isTabVisible(tab))
    return;

  // nidx - "native index" - native to this inherited class
  int nidx = tabs.indexOf(tab);
  if (nidx == -1)
    return;

  // Clear and re-add tabs (to preserve tab order)
  mask.setBit(nidx, visible);
  QTabWidget::clear();
  for (int i = 0; i < tabs.size(); i++) {
    if (mask.testBit(i)) {
      QTabWidget::addTab(tabs.at(i), names.at(i));
    }
  }
}

bool
TabWidget::isTabVisible(QWidget *tab) {
  int nidx = tabs.indexOf(tab);
  if (nidx == -1)
    return false;

  return mask.testBit(nidx);
}
