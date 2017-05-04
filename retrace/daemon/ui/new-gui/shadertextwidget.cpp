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

#include "shadertextwidget.hpp"

using glretrace::ShaderTextWidget;

// From Qt style sheets examples "Customizing QAbstractScrollArea"
const char *
ShaderTextWidget::editStyleSheet =
    "QTextEdit {\n"
    "  background-color: White;\n"
    "  border: 1px solid DarkGray;\n"
    "  selection-background-color: LightSteelBlue;\n"
    "}";

const char *
ShaderTextWidget::normalStyleSheet =
    "QTextEdit {\n"
    "  background-color: transparent;\n"
    "  border: 1px solid LightGray;\n"
    "  selection-background-color: LightSteelBlue;\n"
    "}";

ShaderTextWidget::ShaderTextWidget(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  setLayout(layout);
  layout->setSpacing(0);
  text = new QTextEdit(this);
  text->setFontFamily("monospace");
  text->setStyleSheet(normalStyleSheet);
  text->setReadOnly(true);
  layout->addWidget(text);

  toolsArea = new QWidget(this);
  toolsLayout = new QHBoxLayout(toolsArea);
  toolsArea->setLayout(toolsLayout);
  toolsLayout->setSpacing(0);
  findWidget = new FindWidget(toolsArea);
  findWidget->setTextEdit(text);
  compileButton = new QPushButton("Compile", this);
  compileButton->setEnabled(false);
  compileButton->setVisible(false);
  toolsLayout->addWidget(findWidget);
  toolsLayout->addStretch();
  toolsLayout->addWidget(compileButton);
  layout->addWidget(toolsArea);

  connect(findWidget, &FindWidget::printMessage,
          this, &ShaderTextWidget::printMessage);
}

ShaderTextWidget::~ShaderTextWidget() {
}

void
ShaderTextWidget::setCompileEnabled(bool enabled) {
  compileButton->setVisible(enabled);
  text->setReadOnly(!enabled);
  if (enabled)
    text->setStyleSheet(editStyleSheet);
  else
    text->setStyleSheet(normalStyleSheet);
}

void
ShaderTextWidget::setPlainText(const QString &plainText) {
  text->setPlainText(plainText);
}

QString
ShaderTextWidget::toPlainText() {
  return text->toPlainText();
}
