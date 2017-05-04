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


#ifndef _SHADERMODEL_HPP_
#define _SHADERMODEL_HPP_

#include <QObject>
#include <QStringList>
#include <QVector>

#include "glframe_retrace_interface.hpp"
#include "rendershaders.hpp"

namespace glretrace {

class ShaderModel : public QObject {
  Q_OBJECT

 public:
  ShaderModel(int count);
  ~ShaderModel();

  QStringList getRenderStrings() { return renderStrings; }
  void setAssembly(RenderId renderId,
                   SelectionId selectionCount,
                   const ShaderAssembly &vertex,
                   const ShaderAssembly &fragment,
                   const ShaderAssembly &tess_control,
                   const ShaderAssembly &tess_eval,
                   const ShaderAssembly &geom,
                   const ShaderAssembly &comp);

 signals:
  void shaderTextObject(RenderShaders *rs);

 public slots:
  void getShaderText(int renderIndex);

 protected:
  QStringList renderStrings;
  QVector<RenderId> renders;
  QVector<RenderShaders*> renderData;
};

}  // namespace glretrace

#endif  // _SHADERMODEL_HPP_
