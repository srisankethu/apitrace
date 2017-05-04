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

#include "findwidget.hpp"

#include <QRegExp>

using glretrace::FindWidget;

const char *
FindWidget::findButtonStyleSheet =
    "QToolButton {\n"
    "  border: 1px solid DarkGray;\n"
    "  border-radius: 0px;\n"
    "  width: 23px;\n"
    "  height: 23px;\n"
    "}";

const char *
FindWidget::findEditNormalStyleSheet =
    "QLineEdit {\n"
    "  border: 1px solid gray;\n"
    "  border-radius: 0px;\n"
    "  background-color: white;\n"
    "  color: Black;\n"
    "  width: 300px;\n"
    "  height: 26px;\n"
    "}";

const char *
FindWidget::findEditRedStyleSheet =
    "QLineEdit {\n"
    "  border: 1px solid gray;\n"
    "  border-radius: 0px;\n"
    "  background-color: Salmon;\n"
    "  color: White;\n"
    "  width: 300px;\n"
    "  height: 26px;\n"
    "}";

FindWidget::FindWidget(QWidget *parent) : QWidget(parent),
                                          edit(NULL) {
  layout = new QHBoxLayout(this);
  findEdit = new QLineEdit(this);
  findEdit->setClearButtonEnabled(true);
  findEdit->setPlaceholderText("Find RegExp");
  findEdit->setStyleSheet(findEditNormalStyleSheet);
  findUp = new QToolButton(findEdit);
  findUp->setToolTip("Find previous");
  findUp->setArrowType(Qt::UpArrow);
  findUp->setStyleSheet(findButtonStyleSheet);
  findUp->setEnabled(false);
  findDown = new QToolButton(findEdit);
  findDown->setToolTip("Find next");
  findDown->setArrowType(Qt::DownArrow);
  findDown->setStyleSheet(findButtonStyleSheet);
  findDown->setEnabled(false);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(findEdit);
  layout->addWidget(findUp);
  layout->addWidget(findDown);

  makeConnections();
}

void
FindWidget::makeConnections() {
  connect(findEdit, &QLineEdit::textChanged,
          this, &FindWidget::findRegExp);
  connect(findUp, &QToolButton::pressed,
          this, &FindWidget::findPrevious);
  connect(findDown, &QToolButton::pressed,
          this, &FindWidget::findNext);
}

FindWidget::~FindWidget() {
}

void
FindWidget::setTextEdit(QTextEdit *te) {
  edit = te;
}

bool
FindWidget::find(const QString &text,
                 QTextDocument::FindFlags options) {
  if (edit) {
    emit printMessage(text);
    return edit->find(QRegExp(text), options);
  } else {
    return true;
  }
}

void
FindWidget::moveCursor(QTextCursor::MoveOperation operation,
                       QTextCursor::MoveMode mode) {
  if (edit)
    edit->moveCursor(operation, mode);
}

void
FindWidget::findRegExp(const QString &text) {

  findUp->setEnabled(true);
  findDown->setEnabled(true);

  if (find(text)) {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
  } else {
    findNext();
  }

  if (text.isEmpty()) {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
    findUp->setEnabled(false);
    findDown->setEnabled(false);
  }
}

void
FindWidget::findPrevious() {
  if (!find(findEdit->text(), QTextDocument::FindBackward)) {
    findEdit->setStyleSheet(findEditRedStyleSheet);

    //Go to the end of the document and check again.
    moveCursor(QTextCursor::End);
    if (find(findEdit->text(), QTextDocument::FindBackward)) {
      findEdit->setStyleSheet(findEditNormalStyleSheet);
    }

  } else {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
  }
}

void
FindWidget::findNext() {
  if (!find(findEdit->text())) {
    findEdit->setStyleSheet(findEditRedStyleSheet);

    // Go to the beginning of the document and check again.
    moveCursor(QTextCursor::Start);
    if (find(findEdit->text()))
      findEdit->setStyleSheet(findEditNormalStyleSheet);
  } else {
    findEdit->setStyleSheet(findEditNormalStyleSheet);
  }
}
