cmake_minimum_required(VERSION 3.25)

project(blotverse
  VERSION 0.1.0
  DESCRIPTION "Multiverse robustness analysis for western blot quantification"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(BLOTVERSE_BUILD_TESTS "Build unit and integration tests" ON)
option(BLOTVERSE_BUILD_APPS "Build the command line interface" ON)

include(cmake/Dependencies.cmake)
include(cmake/Warnings.cmake)

configure_file(
  ${PROJECT_SOURCE_DIR}/include/blotverse/version.hpp.in
  ${PROJECT_BINARY_DIR}/generated/blotverse/version.hpp
  @ONLY)

add_library(blotverse
  src/core/image.cpp
  src/core/parallel.cpp
  src/core/rng.cpp
  src/core/numeric.cpp
  src/io/sha256.cpp
  src/io/tiff.cpp
  src/io/image_file.cpp
  src/imaging/background.cpp
  src/imaging/detect.cpp
  src/imaging/quantify.cpp
  src/imaging/qc.cpp
  src/imaging/image_stats.cpp
  src/analysis/spec.cpp
  src/analysis/stats.cpp
  src/analysis/normalization.cpp
  src/analysis/multiverse.cpp
  src/analysis/aggregate.cpp
  src/experiment.cpp
  src/pipeline.cpp
  src/sim/simulator.cpp
  src/report/html_report.cpp
  src/report/repro.cpp
  src/report/tables.cpp
  src/validation/harness.cpp
  src/validation/metrics.cpp
)
add_library(blotverse::blotverse ALIAS blotverse)

target_include_directories(blotverse
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated>)

target_link_libraries(blotverse
  PUBLIC yaml-cpp::yaml-cpp
  PRIVATE TIFF::TIFF Eigen3::Eigen Threads::Threads)

blotverse_set_warnings(blotverse)

if(BLOTVERSE_BUILD_APPS)
  add_subdirectory(apps)
endif()

if(BLOTVERSE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
