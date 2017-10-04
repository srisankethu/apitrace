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

#ifndef _PERFMETRICS_HPP_
#define _PERFMETRICS_HPP_

#include <string>
#include <vector>

#include "glframe_traits.hpp"

namespace glretrace {

class FrameMetricsCallback {
 public:
  virtual void onMetricList(const std::vector<std::string> &names) = 0;
  virtual void onMetrics(int frame,
                         const std::vector<float> &data) = 0;
};

class FrameMetrics : NoCopy, NoAssign {
 public:
  explicit FrameMetrics(FrameMetricsCallback *cb);
  ~FrameMetrics();
  int groupCount() const;
  void selectGroup(int index);
  void begin(int frame);
  void end();
  void publish(FrameMetricsCallback *callback, bool flush=false);
 private:
  int m_current_group;
};

}  // namespace glretrace

#endif /* _PERFMETRICS_HPP_ */
