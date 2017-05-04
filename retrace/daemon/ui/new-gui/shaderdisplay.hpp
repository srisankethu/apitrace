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


#ifndef _SHADERDISPLAY_HPP_
#define _SHADERDISPLAY_HPP_

#include <QWidget>

#include "rendershaders.hpp"
#include "tabwidget.hpp"
#include "shadertextwidget.hpp"

namespace glretrace {

class ShaderDisplay : public TabWidget {
  Q_OBJECT
 public:
  explicit ShaderDisplay(QWidget *parent = 0);
  virtual ~ShaderDisplay();

  void setText(ShaderTextWidget *edit, QString text);
  void populate(RenderShaders *rs, QString shaderType);
  bool hasText();

 signals:
  void printMessage(QString msg);

 private:
  ShaderTextWidget* initTab(QString name);

 protected:
  ShaderTextWidget *source;
  ShaderTextWidget *ir;
  ShaderTextWidget *ssa;
  ShaderTextWidget *nir;
  ShaderTextWidget *simd8;
  ShaderTextWidget *simd16;
  ShaderTextWidget *simd32;
  ShaderTextWidget *beforeUnification;
  ShaderTextWidget *afterUnification;
  ShaderTextWidget *beforeOptimization;
  ShaderTextWidget *constCoalescing;
  ShaderTextWidget *genIrLowering;
  ShaderTextWidget *layoutState;
  ShaderTextWidget *optimized;
  ShaderTextWidget *pushAnalysis;
  ShaderTextWidget *codeHoisting;
  ShaderTextWidget *codeSinking;
};

}  // namespace glretrace

#endif  // _SHADERDISPLAY_HPP_
