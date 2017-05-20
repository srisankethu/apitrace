# Copyright (c) 2017 Intel Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 2.8)

include_directories (
  ${CMAKE_SOURCE_DIR}/retrace/daemon/ui/new-gui
  )

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Concurrent REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${Qt5Widgets_DEFINITIONS})

qt5_add_resources(RESOURCE_ADDED_NG resources.qrc)

set (UI_HDRS
  mainwindow.hpp
  opendialog.hpp
  imageview.hpp
  graphwindow.hpp
  uimodel.hpp
  ../glframe_qutil.hpp
  metricmodel.hpp
  zoombar.hpp
  shadertab.hpp
  shaderdisplay.hpp
  shadermodel.hpp
  rendershaders.hpp
  tabwidget.hpp
  findwidget.hpp
  shadertextwidget.hpp
  apitab.hpp
  rendertab.hpp
)

set (UI_SRC
  main.cpp
  mainwindow.cpp
  opendialog.cpp
  imageview.cpp
  graphwindow.cpp
  uimodel.cpp
  ../glframe_qutil.cpp
  metricmodel.cpp
  zoombar.cpp
  shadertab.cpp
  shaderdisplay.cpp
  shadermodel.cpp
  rendershaders.cpp
  tabwidget.cpp
  findwidget.cpp
  shadertextwidget.cpp
  apitab.cpp
  rendertab.cpp
)

add_executable(fr-new-gui
  ${UI_SRC}
  ${UI_HDRS}
  ${RESOURCE_ADDED_NG}
)

if (WIN32)
set (GL_LIB opengl32)
set (DL_LIB "")
else ()
set (GL_LIB GL)
set (DL_LIB dl)
endif ()

target_link_libraries(fr-new-gui
  retrace_daemon
  retrace_bargraph
  glretrace_common
  glhelpers
  glproc_gl
  Qt5::Core Qt5::Gui Qt5::Concurrent Qt5::Widgets
  libprotobuf
  ${GL_LIB}
  ${X11_X11_LIB}
  ${DL_LIB}
)

