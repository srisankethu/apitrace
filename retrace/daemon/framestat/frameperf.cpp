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

#include <getopt.h>

#include <string>
#include <vector>

#include "glframe_glhelper.hpp"
#include "perfplayer.hpp"

using glretrace::PerfPlayer;

int main(int argc, char *argv[]) {
  std::string frame_file;
  const char *usage = "USAGE: frameperf -g {metrics_group} -f {trace_file} \n";
  int opt, group=-1;
  while ((opt = getopt(argc, argv, "g:f:h")) != -1) {
    switch (opt) {
      case 'f':
        frame_file = optarg;
        continue;
      case 'g':
        group = atoi(optarg);
        continue;
      case 'h':
      default: /* '?' */
        printf("%s", usage);
        return 0;
    }
  }
  if (frame_file == "") {
    printf("ERROR: -f required\n");
    printf("%s", usage);
    return -1;
  }
  if (group == -1) {
    printf("ERROR: -g required\n");
    printf("%s", usage);
    return -1;
  }

  if (FILE *file = fopen(frame_file.c_str(), "r")) {
    fclose(file);
  } else {
    printf("ERROR: frame file not found: %s\n", frame_file.c_str());
    printf("%s", usage);
    return -1;
  }
  glretrace::GlFunctions::Init();
  PerfPlayer p(frame_file, group);
  p.play();
  return 0;
}
