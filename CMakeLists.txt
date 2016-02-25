project(lag-monitor)

cmake_minimum_required(VERSION 2.8)

include(CheckCXXCompilerFlag)
include(CheckLibraryExists)
include(FeatureSummary)

find_package(Qt5Widgets)
set_package_properties(Qt5Widgets PROPERTIES
    PURPOSE "Qt5 Gui module"
    URL "http://qt-project.org/"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}")
set(PLUGIN_INSTALL_DIR "${LIB_INSTALL_DIR}/qt4/plugins")

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS -DQT_STRICT_ITERATORS)

if(CMAKE_COMPILER_IS_GNUCXX)
  check_cxx_compiler_flag(-Wunused-but-set-variable HAVE_GCC_UNUSED_BUT_SET)
  check_cxx_compiler_flag(-Wlogical-op HAVE_GCC_LOGICAL_OP)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wextra -Woverloaded-virtual -Winit-self -Wmissing-include-dirs -Wunused -Wno-div-by-zero -Wundef -Wpointer-arith -Wcast-qual -Wcast-align -Wmissing-noreturn -Werror=return-type -fvisibility=hidden")
  if(HAVE_GCC_UNUSED_BUT_SET)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-but-set-variable")
  endif()
  if(HAVE_GCC_LOGICAL_OP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-op")
  endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(
  INSTALL_TARGETS_DEFAULT_ARGS
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Devel
)

add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
