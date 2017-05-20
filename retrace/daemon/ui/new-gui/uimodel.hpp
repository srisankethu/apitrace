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


#ifndef _UIMODEL_HPP_
#define _UIMODEL_HPP_

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>

#include "glframe_qselection.hpp"
#include "glframe_retrace_interface.hpp"
#include "glframe_retrace_stub.hpp"
#include "metricmodel.hpp"
#include "shadermodel.hpp"
#include "rendershaders.hpp"

namespace glretrace {

class UiModel : public QObject,
                public OnFrameRetrace {
  Q_OBJECT
  Q_PROPERTY(glretrace::QSelection* selection
             READ selection WRITE setSelection);

 public:
  UiModel();
  ~UiModel();

  bool setFrame(const QString &filename, int framenumber,
                const QString &host);
  QSelection *selection();
  void setSelection(QSelection *s);

  void requestGraphData(QString name);

  // OnFrameRetrace interface functions.
  void onFileOpening(bool needUpload,
                     bool finished,
                     uint32_t frame_count);
  void onShaderAssembly(RenderId renderId,
                        SelectionId selectionCount,
                        const ShaderAssembly &vertex,
                        const ShaderAssembly &fragment,
                        const ShaderAssembly &tess_control,
                        const ShaderAssembly &tess_eval,
                        const ShaderAssembly &geom,
                        const ShaderAssembly &comp);
  void onRenderTarget(SelectionId selectionCount,
                      ExperimentId experimentCount,
                      const uvec & pngImageData);
  void onMetricList(const std::vector<MetricId> &ids,
                    const std::vector<std::string> &names,
                    const std::vector<std::string> &descriptions);
  void onMetrics(const MetricSeries &metricData,
                 ExperimentId experimentCount,
                 SelectionId selectionCount);
  void onShaderCompile(RenderId renderId,
                       ExperimentId experimentCount,
                       bool status,
                       const std::string &errorString);
  void onApi(SelectionId selectionCount,
             RenderId renderId,
             const std::vector<std::string> &api_calls);
  void onError(ErrorSeverity s, const std::string &message);

 signals:
  void frameCountChanged(int frameCount);
  void fileLoadFinished();
  void metricNamesReceived(QStringList names);
  // QVector is implicitly shared, so no deep copy occurs.
  void graphDataReceived(QString name, QVector<float> data);
  void generalError(QString text, QString details, bool fatal);
  void renderStrings(QStringList renders);
  void needShaderText(int renderIndex);
  void shaderTextObject(RenderShaders *rs);
  void printMessage(QString msg);
  void hasShaders();
  void apiTextObject(QStringList text);
  void renderImage(QPixmap p);

 public slots:
  void getShaderText(int renderIndex);
  void getApiText(int idx);
  void requestRenderTarget(int idx, RenderOptions opt, RenderTargetType rtt);

 private:
  FrameRetraceStub m_retrace;
  FrameState *m_state;
  QSelection *m_selection;
  SelectionId m_selection_count;
  QList<int> m_cached_selection;
  int m_target_frame_number;
  MetricModel *m_metric_model;
  ShaderModel *m_shader_model;
  QVector<QStringList> m_api_calls;
};

}  // namespace glretrace

#endif  // _UIMODEL_HPP_
