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

#include "shaderdisplay.hpp"

using glretrace::ShaderDisplay;
using glretrace::RenderShaders;
using glretrace::ShaderTextWidget;

ShaderDisplay::ShaderDisplay(QWidget *parent) : TabWidget(parent) {
  source = initTab("Source");
  source->setCompileEnabled(true);
  ir = initTab("IR");
  ssa = initTab("SSA");
  nir = initTab("NIR");
  simd8 = initTab("Simd8");
  simd16 = initTab("Simd16");
  simd32 = initTab("Simd32");
  beforeUnification = initTab("Before Unification");
  afterUnification = initTab("After Unification");
  beforeOptimization = initTab("Before Optimization");
  constCoalescing = initTab("Const Coalescing");
  genIrLowering = initTab("Gen IR Lowering");
  layoutState = initTab("Layout");
  optimized = initTab("Optimized");
  pushAnalysis = initTab("Push Analysis");
  codeHoisting = initTab("Code Hoisting");
  codeSinking = initTab("Code Sinking");

  connect(source, &ShaderTextWidget::printMessage,
          this, &ShaderDisplay::printMessage);
  connect(nir, &ShaderTextWidget::printMessage,
          this, &ShaderDisplay::printMessage);
  connect(source, &ShaderTextWidget::compileNeeded,
          this, &ShaderDisplay::compileNeeded);
}

ShaderDisplay::~ShaderDisplay() {
}

ShaderTextWidget*
ShaderDisplay::initTab(QString name) {
  ShaderTextWidget *tab = new ShaderTextWidget(this);
  addTab(tab, name);
  setTabVisible(tab, false); // Hide tab unless text.

  return tab;
}

void
ShaderDisplay::setText(ShaderTextWidget *edit, QString text) {
  edit->setPlainText(text);

  // Don't hide the tab if it's a currently visible
  // source tab, because the user might be editing it.
  if (!isTabVisible(edit) || (edit != source))
    setTabVisible(edit, !text.isEmpty());
}

void
ShaderDisplay::populate(RenderShaders *rs, QString shaderType) {
  setText(source, rs->getShaderText(shaderType, "shader"));
  setText(ir, rs->getShaderText(shaderType, "ir"));
  setText(ssa, rs->getShaderText(shaderType, "ssa"));
  setText(nir, rs->getShaderText(shaderType, "nir"));
  setText(simd8, rs->getShaderText(shaderType, "simd8"));
  setText(simd16, rs->getShaderText(shaderType, "simd16"));
  setText(simd32, rs->getShaderText(shaderType, "simd32"));
  setText(beforeUnification,
          rs->getShaderText(shaderType, "beforeUnification"));
  setText(afterUnification,
          rs->getShaderText(shaderType, "afterUnification"));
  setText(beforeOptimization,
          rs->getShaderText(shaderType, "beforeOptimization"));
  setText(constCoalescing,
          rs->getShaderText(shaderType, "constCoalescing"));
  setText(genIrLowering,
          rs->getShaderText(shaderType, "genIrLowering"));
  setText(layoutState, rs->getShaderText(shaderType, "layout"));
  setText(optimized, rs->getShaderText(shaderType, "optimized"));
  setText(pushAnalysis,
          rs->getShaderText(shaderType, "pushAnalysis"));
  setText(codeHoisting,
          rs->getShaderText(shaderType, "codeHoisting"));
  setText(codeSinking,
          rs->getShaderText(shaderType, "codeSinking"));
}

bool
ShaderDisplay::hasText() {
  if (!source->toPlainText().isEmpty())
    return true;
  else if (!ir->toPlainText().isEmpty())
    return true;
  else if (!ssa->toPlainText().isEmpty())
    return true;
  else if (!nir->toPlainText().isEmpty())
    return true;
  else if (!simd8->toPlainText().isEmpty())
    return true;
  else if (!simd16->toPlainText().isEmpty())
    return true;
  else if (!simd32->toPlainText().isEmpty())
    return true;
  else if (!beforeUnification->toPlainText().isEmpty())
    return true;
  else if (!afterUnification->toPlainText().isEmpty())
    return true;
  else if (!beforeOptimization->toPlainText().isEmpty())
    return true;
  else if (!constCoalescing->toPlainText().isEmpty())
    return true;
  else if (!genIrLowering->toPlainText().isEmpty())
    return true;
  else if (!layoutState->toPlainText().isEmpty())
    return true;
  else if (!optimized->toPlainText().isEmpty())
    return true;
  else if (!pushAnalysis->toPlainText().isEmpty())
    return true;
  else if (!codeHoisting->toPlainText().isEmpty())
    return true;
  else if (!codeSinking->toPlainText().isEmpty())
    return true;
  else
    return false;
}
