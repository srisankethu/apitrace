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
 *
 *   This was styled after the Mozilla Firefox find bar.
 **************************************************************************/


#ifndef _FINDWIDGET_HPP_
#define _FINDWIDGET_HPP_

#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>

namespace glretrace {

class FindWidget : public QWidget {
  Q_OBJECT
 public:
  explicit FindWidget(QWidget *parent = 0);
  virtual ~FindWidget();

  void setTextEdit(QTextEdit *te);

 signals:
  void printMessage(QString msg);

 private:
  void makeConnections();
  bool find(const QString &text,
    QTextDocument::FindFlags options = QTextDocument::FindFlags());
  void moveCursor(QTextCursor::MoveOperation operation,
    QTextCursor::MoveMode mode = QTextCursor::MoveAnchor);

 public slots:
  void findRegExp(const QString &text);
  void findPrevious();
  void findNext();

 protected:
  QHBoxLayout *layout;
  QLineEdit *findEdit;
  static const char *findEditNormalStyleSheet;
  static const char *findEditRedStyleSheet;
  static const char *findButtonStyleSheet;
  QToolButton *findUp;
  QToolButton *findDown;
  QTextEdit *edit;
};

}  // namespace glretrace

#endif  // _FINDWIDGET_HPP_
