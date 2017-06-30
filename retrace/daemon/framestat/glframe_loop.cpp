/**************************************************************************
 *
 * Copyright 2016 Intel Corporation
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
 *   Mark Janes <mark.a.janes@intel.com>
 **************************************************************************/

#include "glframe_loop.hpp"

#include <GLES2/gl2.h>

#include <sstream>
#include <string>
#include <vector>

#include "glframe_glhelper.hpp"
#include "glretrace.hpp"
#include "retrace.hpp"

using glretrace::FrameLoop;

using retrace::parser;
using trace::Call;

extern retrace::Retracer retracer;

#ifdef _MSC_VER
#include "Windows.h"
inline unsigned int
get_ms_time() {
  LARGE_INTEGER frequency;
  ::QueryPerformanceFrequency(&frequency);

  LARGE_INTEGER start;
  ::QueryPerformanceCounter(&start);

  return start.QuadPart / (frequency.QuadPart / 1000);
}
#else
inline unsigned int
get_ms_time() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t);
  unsigned int ms = t.tv_sec * 1000;
  ms += (t.tv_nsec / 1000000);
  return ms;
}
#endif

FrameLoop::FrameLoop(const std::string filepath,
                     const std::string out_path,
                     int loop_count,
                     bool flush_after_parse,
                     bool flush_after_draw)
    : m_of(), m_out(NULL),
      m_current_frame(1),
      m_loop_count(loop_count),
      m_flush_parse(flush_after_parse),
      m_flush_draw(flush_after_draw) {
  if (out_path.size()) {
    m_of.open(out_path);
    m_out = new std::ostream(m_of.rdbuf());
  } else {
    m_out = new std::ostream(std::cout.rdbuf());
  }
  *m_out << filepath << std::endl;
  *m_out << "frame";
  for (int i = 0; i < loop_count; ++i)
    *m_out << "\t" << i;
  *m_out << std::endl;
  *m_out << std::endl;
  *m_out << std::endl;
  *m_out << "start: " << get_ms_time() << std::endl;

  retrace::debug = 0;
  retracer.addCallbacks(glretrace::gl_callbacks);
  retracer.addCallbacks(glretrace::glx_callbacks);
  retracer.addCallbacks(glretrace::wgl_callbacks);
  retracer.addCallbacks(glretrace::cgl_callbacks);
  retracer.addCallbacks(glretrace::egl_callbacks);
  retrace::setUp();
  parser->open(filepath.c_str());
}

FrameLoop::~FrameLoop() {
  *m_out << "end: " << get_ms_time() << std::endl;
  if (m_of.is_open())
    m_of.close();
  delete m_out;
}

bool frame_boundary(const trace::Call &c) {
  const bool end_frame = c.flags & trace::CALL_FLAG_END_FRAME;
  if (!end_frame)
    return false;
  // do not count bogus frame terminators
  if (strncmp("glFrameTerminatorGREMEDY", c.sig->name,
              strlen("glFrameTerminatorGREMEDY")) != 0)
    return true;
  return false;
}

void
FrameLoop::advanceToFrame(int f) {
  // 1st value: frame number
  // 2nd value: start parse time
  // *m_out << std::endl << f << "\t" << get_ms_time();
  for (auto c : m_calls)
    delete c;
  m_calls.clear();

  trace::Call *call;
  while ((call = parser->parse_call()) && m_current_frame < f) {
    // retracer.retrace(*call);
    const bool end_frame = frame_boundary(*call);
    delete call;
    if (end_frame) {
      ++m_current_frame;
      if (m_current_frame == f)
        break;
    }
  }

  while ((call = parser->parse_call())) {
    m_calls.push_back(call);
    if (frame_boundary(*call)) {
      ++m_current_frame;
      break;
    }
  }
  // 3rd value: end of parse time
  // *m_out << "\t" << get_ms_time();
  if (m_flush_parse)
    GlFunctions::Finish();
  // 4rd value: end of parse flush time
  // *m_out << "\t" << get_ms_time();
}

void
FrameLoop::loop() {
  // for (auto c : m_calls) {
  //   // retracer.retrace(*c);
  // }
  // 5th value: end of retrace time
  // *m_out << "\t" << get_ms_time();

  if (m_flush_draw)
    GlFunctions::Finish();

  // 6th value: end of finish time
  // *m_out << "\t" << get_ms_time();
}

