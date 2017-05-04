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


#ifndef _RENDERSHADERS_HPP_
#define _RENDERSHADERS_HPP_

#include <QHash>
#include <QObject>
#include <QString>

#include <string>

#include "glframe_retrace_interface.hpp"

namespace glretrace {

class RenderShaders : public QObject {
  Q_OBJECT

 public:
  RenderShaders();
  ~RenderShaders();

  void addShader(QString shaderType, ShaderAssembly s);
  QString getShaderText(QString shaderType, QString asmType);
  void setShaderText(QString shaderType, QString asmType, QString text);

  static std::string *getAssemblyMember(ShaderAssembly *s,
                                        QString asmType);

 protected:
  QHash<QString, ShaderAssembly> shaders;
};

}  // namespace glretrace

#endif  // _RENDERSHADERS_HPP_
