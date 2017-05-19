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

#include "apitab.hpp"

#include <QSizePolicy>

using glretrace::ApiTab;
using glretrace::ShaderTextWidget;

// From Qt style sheets examples "Customizing QListView"
const char *
ApiTab::listStyleSheet =
    "QListView {\n"
    "  background: transparent;\n"
    "  border: 0px;\n"
    "}\n"
    "\n"
    "QListView::item:selected:active {\n"
    "  background: LightSteelBlue;\n"
    "  border: 0px transparent;\n"
    "}\n"
    "\n"
    "QListView::item:selected:!active {\n"
    "  background: LightSteelBlue;\n"
    "  border: 0px transparent;\n"
    "}\n"
    "\n"
    "QListView::item:hover {\n"
    "  background: DarkGray;\n"
    "}";

ApiTab::ApiTab(QWidget *parent) : QWidget(parent) {
  layout = new QHBoxLayout(this);
  setLayout(layout);

  // Renders list.
  rendersModel = new QStringListModel(this);
  renderSelection = new QListView(this);
  renderSelection->setModel(rendersModel);
  renderSelection->setStyleSheet(listStyleSheet);
  renderSelection->setSizePolicy(QSizePolicy::Maximum,
                                 QSizePolicy::Expanding);
  layout->addWidget(renderSelection);

  // Api Text
  text = new ShaderTextWidget(this);
  layout->addWidget(text);

  makeConnections();
}

void
ApiTab::makeConnections() {
  connect(renderSelection, &QListView::clicked,
          this, &ApiTab::convertActivation);
}

ApiTab::~ApiTab() {
}

void
ApiTab::setRenders(QStringList r) {
  renders = r;
  rendersModel->setStringList(renders);
}

void
ApiTab::setModel(UiModel* mdl) {
  model = mdl;

  connect(model, &UiModel::renderStrings,
          this, &ApiTab::setRenders);
  connect(this, &ApiTab::shaderActivated,
          model, &UiModel::getApiText);
  connect(model, &UiModel::apiTextObject,
          this, &ApiTab::populate);
}

void
ApiTab::convertActivation(const QModelIndex &index) {
  QVariant varIndex = rendersModel->data(index, Qt::DisplayRole);
  QString stringIndex = varIndex.toString();
  int intIndex = stringIndex.toInt();
  QString msg = "activated ";
  msg.append(stringIndex);
  emit printMessage(msg);
  emit shaderActivated(intIndex);
}

void
ApiTab::activateShader(int index) {
  QModelIndex midx = rendersModel->index(index, 0);
  renderSelection->setCurrentIndex(midx);
  if (model)
    model->getApiText(index);
}

void
ApiTab::populate(QStringList textList) {
  text->setPlainText(textList.join("\n"));
}
