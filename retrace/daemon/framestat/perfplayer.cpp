// Copyright (C) Intel Corp.  2017.  All Rights Reserved.

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice (including the
// next paragraph) shall be included in all copies or substantial
// portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

//  **********************************************************************/
//  * Authors:
//  *   Mark Janes <mark.a.janes@intel.com>
//  **********************************************************************/

#include "perfplayer.hpp"

#include <sstream>

#include <GLES2/gl2.h>
#include "glretrace.hpp"
#include "glframe_retrace_render.hpp"
#include "perfmetrics.hpp"

using glretrace::FrameMetricsCallback;
using glretrace::FrameMetrics;
using glretrace::PerfPlayer;
using retrace::parser;

extern retrace::Retracer retracer;

class PerfCallback : public FrameMetricsCallback {
  void onMetricList(const std::vector<std::string> &names) {
    printf("frame");
    for (auto n : names)
      printf("\t%s", n.c_str());
    printf("\n");
  }
  void onMetrics(int frame,
                 const std::vector<float> &data) {
    printf("%d", frame);
    for (auto d : data)
      printf("\t%f", d);
    printf("\n");
  }
};

PerfPlayer::PerfPlayer(const std::string &trace) {
  retrace::debug = 0;
  retrace::verbosity = -1;
  retracer.addCallbacks(glretrace::gl_callbacks);
  retracer.addCallbacks(glretrace::glx_callbacks);
  retracer.addCallbacks(glretrace::wgl_callbacks);
  retracer.addCallbacks(glretrace::cgl_callbacks);
  retracer.addCallbacks(glretrace::egl_callbacks);

  retrace::setUp();
  parser->open(trace.c_str());

  // play through the first frame to ensure we have a context
  trace::Call *call;
  while ((call = parser->parse_call())) {
    std::stringstream call_stream;
    retracer.retrace(*call);
    const bool frame_boundary = RetraceRender::endsFrame(*call);
    delete call;
    if (frame_boundary)
      break;
  }
  m_cb = new PerfCallback;
  m_metrics = new FrameMetrics(m_cb);
  m_metrics->selectGroup(1);
}

void
PerfPlayer::play() {
  int frame = 1;
  m_metrics->begin(frame);
  trace::Call *call;
  while ((call = parser->parse_call())) {
    retracer.retrace(*call);
    const bool frame_boundary = RetraceRender::endsFrame(*call);
    delete call;
    if (frame_boundary) {
      m_metrics->end(frame);
      m_metrics->publish();
      ++frame;
      m_metrics->begin(frame);
    }
  }
  m_metrics->end(frame);
  m_metrics->publish(true);
}
