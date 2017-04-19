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

#include "uimodel.hpp"

#include <sstream>
#include <string>
#include <vector>

#include <QtConcurrentRun>
#include <QCoreApplication>
#include <QFileInfo>

#include "glframe_logger.hpp"
#include "glframe_os.hpp"
#include "glframe_socket.hpp"
#include "../glframe_qutil.hpp"

using glretrace::FrameRetraceStub;
using glretrace::FrameState;
using glretrace::MetricModel;
using glretrace::OnFrameRetrace;
using glretrace::QSelection;
using glretrace::UiModel;
using glretrace::RenderSelection;
using glretrace::SelectionId;
using glretrace::ServerSocket;

UiModel::UiModel() : m_state(NULL),
                     m_selection(NULL),
                     m_selection_count(0),
                     m_metric_model(NULL) {
}

UiModel::~UiModel() {
  if (m_state) {
    delete m_state;
    m_state = NULL;
  }
  if (m_metric_model) {
    delete m_metric_model;
    m_metric_model = NULL;
  }
  m_retrace.Shutdown();
}

FrameState *frame_state_off_thread(std::string filename,
                                   int framenumber) {
  return new FrameState(filename, framenumber);
}

static QFuture<FrameState *> future;

void
exec_retracer(int port) {
  std::string server_exe_name, server_exe;
  char sep;

  server_exe = QCoreApplication::applicationDirPath().toStdString();

#ifdef WIN32
  sep = '\\';
  server_exe_name = "frame_retrace_server.exe";
#else
  sep = '/';
  server_exe_name = "frame_retrace_server";
#endif

  server_exe += sep;
  server_exe += server_exe_name;

  std::stringstream port_ss;
  port_ss << port;

  std::string port_str(port_ss.str());

  const char *const args[] = {server_exe.c_str(),
                              "-p",
                              port_str.c_str(),
                              NULL};
  glretrace::fork_execv(server_exe.c_str(), args);
}

bool
UiModel::setFrame(const QString &filename, int framenumber,
                  const QString &host) {
  QFileInfo check_file(filename);
  if (!check_file.exists())
    return false;
  if (!check_file.isFile())
    return false;

  // m_retrace = new FrameRetrace(filename.toStdString(), framenumber);
  future = QtConcurrent::run(frame_state_off_thread,
                             filename.toStdString(), framenumber);
  m_state = future.result();
  int port = 24642;
  if (host == "localhost") {
    {
      ServerSocket sock(0);
      port = sock.GetPort();
    }
    GRLOGF(glretrace::WARN, "using port: %d", port);
    exec_retracer(port);
  }

  m_retrace.Init(host.toStdString().c_str(), port);

  // let the stub calculate the md5 off-thread.  Doing it here
  // conforms better to the interfaces, but blocks the UI.
  std::vector<unsigned char> md5;

  m_target_frame_number = framenumber;
  m_retrace.openFile(filename.toStdString(), md5, 0,
                     framenumber, this);

  RenderSelection sel;
  glretrace::renderSelectionFromList(m_selection_count,
                                     m_cached_selection,
                                     &sel);
  m_retrace.retraceApi(sel, this);
  return true;
}

QSelection *
UiModel::selection() {
  return m_selection;
}

void
UiModel::setSelection(QSelection *s) {
  m_selection = s;
}

void
UiModel::requestGraphData(QString name) {
  std::vector<MetricId> ids;
  ids.push_back(m_metric_model->getId(name));
  m_retrace.retraceMetrics(ids, ExperimentId(0), this);
}

// Automatically called when file is opened to give status updates
void
UiModel::onFileOpening(bool needUpload,
                       bool finished,
                       uint32_t frame_count) {
  emit frameCountChanged(frame_count);
  if (finished) {
    emit fileLoadFinished();

    // Make a request for a set of NULL (width = 1.0) data.
    std::vector<MetricId> ids;
    ids.push_back(MetricId(0));
    m_retrace.retraceMetrics(ids, ExperimentId(0), this);
  }
}

void
UiModel::onShaderAssembly(RenderId renderId,
                          SelectionId selectionCount,
                          const ShaderAssembly &vertex,
                          const ShaderAssembly &fragment,
                          const ShaderAssembly &tess_control,
                          const ShaderAssembly &tess_eval,
                          const ShaderAssembly &geom,
                          const ShaderAssembly &comp) {
}

void
UiModel::onRenderTarget(SelectionId selectionCount,
                        ExperimentId experimentCount,
                        const uvec & pngImageData) {
}

// Automatically called when file is opened to give this model
// a list of the available metrics for the file.
void
UiModel::onMetricList(const std::vector<MetricId> &ids,
                      const std::vector<std::string> &names,
                      const std::vector<std::string> &descriptions) {
  delete m_metric_model;
  m_metric_model = new MetricModel(ids, names, descriptions);
  emit metricNamesReceived(m_metric_model->getNamesList());
}

// When FrameRetraceStub's method retrace is called,
// this is automatically called to receive bar width and height data
// for forwarding to the graph widget.
void
UiModel::onMetrics(const MetricSeries &metricData,
                   ExperimentId experimentCount,
                   SelectionId selectionCount) {
  // Resolve the name and pass on the data.
  QString name = m_metric_model->getName(metricData.metric);
  emit graphDataReceived(name,
    QVector<float>::fromStdVector(metricData.data));
}

void
UiModel::onShaderCompile(RenderId renderId,
                         ExperimentId experimentCount,
                         bool status,
                         const std::string &errorString) {
}

void
UiModel::onApi(SelectionId selectionCount,
               RenderId renderId,
               const std::vector<std::string> &api_calls) {
}

void
UiModel::onError(ErrorSeverity s, const std::string &message) {
}
