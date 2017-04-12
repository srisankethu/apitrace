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


#ifndef _OPENDIALOG_HPP_
#define _OPENDIALOG_HPP_

#include <QCompleter>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include "imageview.hpp"
#include "uimodel.hpp"

namespace glretrace {

class OpenDialog : public QDialog {
  Q_OBJECT
 public:
  explicit OpenDialog(QWidget *parent = 0);
  virtual ~OpenDialog();

  UiModel* getModel() { return model; }
  void setModel(UiModel* mdl) { model = mdl; }

 public slots:
  void getFilename();
  void openFile();

 protected:
  // Ui objects
  QVBoxLayout *layout;
  ImageView *view;
  QWidget *controls;
  QGridLayout *controlsLayout;
  QLabel *fileLabel;
  QLineEdit *lineEdit;
  QCompleter *lineEditCompleter;
  QFileSystemModel *lineEditModel;
  QToolButton *fileButton;
  QLabel *frameLabel;
  QSpinBox *frameBox;
  QLabel *hostLabel;
  QLineEdit *hostEdit;
  QDialogButtonBox *dialogButtons;

  // Model
  UiModel* model;
};

}  // namespace glretrace

#endif  // _OPENDIALOG_HPP_
