# Third-party dependencies.
#  - libtiff and Eigen come from the system (Homebrew / apt).
#  - yaml-cpp and doctest are fetched at pinned versions.

find_package(Threads REQUIRED)
find_package(TIFF REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

include(FetchContent)

# Older upstream CMakeLists declare cmake_minimum_required < 3.5, which CMake 4 rejects.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# The pinned yaml-cpp is used by default so builds do not silently pick up
# whatever copy is on the machine (e.g. inside a conda environment).
option(BLOTVERSE_USE_SYSTEM_YAML_CPP "Use an installed yaml-cpp instead of the pinned one" OFF)
if(BLOTVERSE_USE_SYSTEM_YAML_CPP)
  find_package(yaml-cpp 0.8 REQUIRED)
else()
  set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
  set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
  set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)
  set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0
    GIT_SHALLOW TRUE
    SYSTEM)
  FetchContent_MakeAvailable(yaml-cpp)
endif()

if(BLOTVERSE_BUILD_TESTS)
  FetchContent_Declare(doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
    GIT_SHALLOW TRUE
    SYSTEM)
  FetchContent_MakeAvailable(doctest)
endif()
