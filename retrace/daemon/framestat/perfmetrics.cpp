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

#include "perfmetrics.hpp"

#include <GL/gl.h>
#include <GL/glext.h>

#include <map>
#include <assert.h>
#include <string.h>

#include "glframe_glhelper.hpp"

using glretrace::FrameMetrics;
using glretrace::GlFunctions;

namespace glretrace {
class PerfMetric : public NoCopy, NoAssign {
 public:
  PerfMetric(int query_id, int counter_num);
  // MetricId id() const;
  const std::string &name() const { return m_name; }
  const std::string &description() const { return m_description; }
  float getMetric(const std::vector<unsigned char> &data) const;
 private:
  const int m_query_id, m_counter_num;
  GLuint m_offset, m_data_size, m_type,
    m_data_type;
  std::string m_name, m_description;
};



class PerfMetricGroup : public NoCopy, NoAssign {
 public:
  explicit PerfMetricGroup(int query_id);
  ~PerfMetricGroup();
  const std::string &name() const { return m_query_name; }
  void activate(FrameMetricsCallback *callback);
  void begin(int frame);
  void end(int frame);
  void publish(FrameMetricsCallback *callback, bool flush);

 private:
  std::string m_query_name;
  const int m_query_id;
  unsigned int m_data_size;
  std::vector<unsigned char> m_data_buf;

  std::vector<PerfMetric *> m_metrics;

  // represent queries that have not produced results.  key is frame number
  std::map<int, int> m_extant_query_handles;
};



}  // namespace glretrace

using glretrace::PerfMetricGroup;
using glretrace::PerfMetric;

PerfMetricGroup::PerfMetricGroup(int query_id) : m_query_id(query_id) {
  static GLint max_name_len = 0;
  if (max_name_len == 0)
    GlFunctions::GetIntegerv(GL_PERFQUERY_QUERY_NAME_LENGTH_MAX_INTEL,
                             &max_name_len);

  std::vector<GLchar> query_name(max_name_len);
  unsigned int number_instances, capabilities_mask, number_counters;
  GlFunctions::GetPerfQueryInfoINTEL(m_query_id,
                                     query_name.size(), query_name.data(),
                                     &m_data_size, &number_counters,
                                     &number_instances, &capabilities_mask);
  m_data_buf.resize(m_data_size);
  m_query_name = query_name.data();
  for (unsigned int counter_num = 1; counter_num <= number_counters;
       ++counter_num) {
    PerfMetric *p = new PerfMetric(m_query_id, counter_num);
    m_metrics.push_back(p);
  }
}

PerfMetricGroup::~PerfMetricGroup(){
  assert(m_extant_query_handles.empty());
  for (auto i : m_metrics)
    delete i;
  m_metrics.clear();
}

void
PerfMetricGroup::activate(FrameMetricsCallback *callback) {
  std::vector<std::string> names;
  for (auto m : m_metrics)
    names.push_back(m->name());
  callback->onMetricList(names);
}

void
PerfMetricGroup::begin(int frame) {
  GLuint query_handle;
  GlFunctions::CreatePerfQueryINTEL(m_query_id, &query_handle);
  GL::GetError();

  GlFunctions::BeginPerfQueryINTEL(query_handle);
  assert(GL_NO_ERROR == GL::GetError());
  m_extant_query_handles[frame] = query_handle;
}

void
PerfMetricGroup::end(int frame) {
  auto q = m_extant_query_handles.find(frame);
  assert(q != m_extant_query_handles.end());
  GlFunctions::EndPerfQueryINTEL(q->second);
}

void
PerfMetricGroup::publish(FrameMetricsCallback *callback, bool flush) {
  std::vector<int> finished_frames;
  for (auto extant_query : m_extant_query_handles) {
    std::vector<float> metric_data;
    memset(m_data_buf.data(), 0, m_data_buf.size());
    GLuint bytes_written;
    GlFunctions::GetPerfQueryDataINTEL(extant_query.second,
                                       (flush ?
                                        GL_PERFQUERY_WAIT_INTEL:
                                        GL_PERFQUERY_DONOT_FLUSH_INTEL),
                                       m_data_size, m_data_buf.data(),
                                       &bytes_written);
    if (bytes_written == 0)
      return;
    assert(bytes_written == m_data_size);
    finished_frames.push_back(extant_query.first);
    
    for (auto desired_metric : m_metrics)
      metric_data.push_back(desired_metric->getMetric(m_data_buf));

    callback->onMetrics(extant_query.first, metric_data);
  }
  for (auto frame : finished_frames) {
    GlFunctions::DeletePerfQueryINTEL(m_extant_query_handles[frame]);
    m_extant_query_handles.erase(frame);
  }
}


PerfMetric::PerfMetric(int query_id,
                       int counter_num) : m_query_id(query_id),
                                          m_counter_num(counter_num) {
  static GLint max_name_len = 0, max_desc_len = 0;
  if (max_name_len == 0)
    GlFunctions::GetIntegerv(GL_PERFQUERY_COUNTER_NAME_LENGTH_MAX_INTEL,
                             &max_name_len);
  if (max_desc_len == 0)
    GlFunctions::GetIntegerv(GL_PERFQUERY_COUNTER_DESC_LENGTH_MAX_INTEL,
                             &max_desc_len);
  std::vector<GLchar> counter_name(max_name_len);
  std::vector<GLchar> counter_description(max_desc_len);
  GLuint64 max_value;
  GlFunctions::GetPerfCounterInfoINTEL(m_query_id, m_counter_num,
                                       counter_name.size(), counter_name.data(),
                                       counter_description.size(),
                                       counter_description.data(),
                                       &m_offset, &m_data_size, &m_type,
                                       &m_data_type, &max_value);
  m_name = counter_name.data();
  m_description = counter_description.data();
}

float
PerfMetric::getMetric(const std::vector<unsigned char> &data) const {
  const unsigned char *p_value = data.data() + m_offset;
  float fval;
  switch (m_data_type) {
    case GL_PERFQUERY_COUNTER_DATA_UINT32_INTEL: {
      assert(m_data_size == 4);
      const uint32_t val = *reinterpret_cast<const uint32_t *>(p_value);
      fval = static_cast<float>(val);
      break;
    }
    case GL_PERFQUERY_COUNTER_DATA_UINT64_INTEL: {
      assert(m_data_size == 8);
      const uint64_t val = *reinterpret_cast<const uint64_t *>(p_value);
      fval = static_cast<float>(val);
      break;
    }
    case GL_PERFQUERY_COUNTER_DATA_FLOAT_INTEL: {
      assert(m_data_size == 4);
      fval = *reinterpret_cast<const float *>(p_value);
      break;
    }
    case GL_PERFQUERY_COUNTER_DATA_DOUBLE_INTEL: {
      assert(m_data_size == 8);
      const double val = *reinterpret_cast<const double *>(p_value);
      fval = static_cast<float>(val);
      break;
    }
    case GL_PERFQUERY_COUNTER_DATA_BOOL32_INTEL: {
      assert(m_data_size == 4);
      const bool val = *reinterpret_cast<const bool*>(p_value);
      fval = val ? 1.0 : 0.0;
      break;
    }
    default:
      assert(false);
  }
  return fval;
}

FrameMetrics::FrameMetrics(FrameMetricsCallback *cb) {}
FrameMetrics::~FrameMetrics() {}
int
FrameMetrics::groupCount() const { return 0; }
void
FrameMetrics::selectGroup(int index) {}
void
FrameMetrics::begin(int frame) {}
void
FrameMetrics::end() {}
void
FrameMetrics::publish(FrameMetricsCallback *callback, bool flush) {}


