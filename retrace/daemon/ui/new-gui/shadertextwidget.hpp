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
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANEDITILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Laura Ekstrand <laura@jlekstrand.net>
 **************************************************************************/


#ifndef _SHADERTEXTWIDGET_HPP_
#define _SHADERTEXTWIDGET_HPP_

#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "findwidget.hpp"

namespace glretrace {

class ShaderTextWidget : public QWidget {
  Q_OBJECT
 public:
  explicit ShaderTextWidget(QWidget *parent = 0);
  virtual ~ShaderTextWidget();

  void setCompileEnabled(bool enabled);
  void setPlainText(const QString &plainText);
  QString toPlainText();

 signals:
  void printMessage(QString msg);
  void compileNeeded();

 protected:
  QVBoxLayout *layout;
  static const char *normalStyleSheet;
  static const char *editStyleSheet;
  QTextEdit *text;
  QWidget *toolsArea;
  QHBoxLayout *toolsLayout;
  FindWidget *findWidget;
  QPushButton *compileButton;
};

}  // namespace glretrace

#endif  // _SHADERTEXTWIDGET_HPP_
