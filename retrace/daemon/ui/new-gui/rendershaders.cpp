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

#include "rendershaders.hpp"

using glretrace::RenderShaders;

RenderShaders::RenderShaders() {
}

RenderShaders::~RenderShaders() {
}

void
RenderShaders::addShader(QString shaderType, ShaderAssembly s) {
  // If key already exists, this will replace that entry.
  shaders[shaderType] = s;
}

QString
RenderShaders::getShaderText(QString shaderType, QString asmType) {
  if (!shaders.contains(shaderType))
    return QString();

  // Normally, you'd use shaders.value() to get at the data
  // because shaders[] can silently insert a default value
  // if the key doesn't exist.  But we already checked for that
  // case using shaders.contains() above, and we need to use []
  // to get a mutable reference.
  std::string* sptr = getAssemblyMember(&shaders[shaderType],
                                        asmType);

  if (sptr) {
    // Need deep copy?
    return QString(QString::fromStdString(*sptr));
  }
  else
    return QString();
}

void
RenderShaders::setShaderText(QString shaderType, QString asmType,
                             QString text) {
  if (!shaders.contains(shaderType))
    return;

  // Normally, you'd use shaders.value() to get at the data
  // because shaders[] can silently insert a default value
  // if the key doesn't exist.  But we already checked for that
  // case using shaders.contains() above, and we need to use []
  // to get a mutable reference.
  std::string* sptr = getAssemblyMember(&shaders[shaderType],
                                        asmType);

  if (!sptr)
    return;

  *sptr = text.toStdString();
}

std::string *
RenderShaders::getAssemblyMember(ShaderAssembly *s,
                                 QString asmType) {
  if (asmType == "shader")
    return &s->shader;
  else if (asmType == "ir")
    return &s->ir;
  else if (asmType == "ssa")
    return &s->ssa;
  else if (asmType == "nir")
    return &s->nir;
  else if (asmType == "simd8")
    return &s->simd8;
  else if (asmType == "simd16")
    return &s->simd16;
  else if (asmType == "simd32")
    return &s->simd32;
  else if (asmType == "beforeUnification")
    return &s->beforeUnification;
  else if (asmType == "afterUnification")
    return &s->afterUnification;
  else if (asmType == "beforeOptimization")
    return &s->beforeOptimization;
  else if (asmType == "constCoalescing")
    return &s->constCoalescing;
  else if (asmType == "genIrLowering")
    return &s->genIrLowering;
  else if (asmType == "layout")
    return &s->layout;
  else if (asmType == "optimized")
    return &s->optimized;
  else if (asmType == "pushAnalysis")
    return &s->pushAnalysis;
  else if (asmType == "codeHoisting")
    return &s->codeHoisting;
  else if (asmType == "codeSinking")
    return &s->codeSinking;
  else
    return NULL;
}
