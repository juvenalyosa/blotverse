# BlotVerse desktop application (Qt Quick). Configure with
#   -DBLOTVERSE_BUILD_GUI=ON -DCMAKE_PREFIX_PATH=<Qt 6 prefix>
# Qt 6.8 links the AGL framework, which recent macOS SDKs no longer ship.
# Point that lookup at OpenGL (linked anyway) so it never resolves to a
# missing framework.
if(APPLE)
  find_library(BLOTVERSE_OPENGL_FRAMEWORK OpenGL)
  if(BLOTVERSE_OPENGL_FRAMEWORK)
    set(WrapOpenGL_AGL "${BLOTVERSE_OPENGL_FRAMEWORK}" CACHE FILEPATH "AGL replacement (see gui/CMakeLists.txt)" FORCE)
  endif()
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Concurrent)
qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(blotverse_gui MACOSX_BUNDLE
  main.cpp
  src/app_controller.cpp
  src/app_controller.hpp
  src/blot_image_provider.cpp
  src/blot_image_provider.hpp
)
set_target_properties(blotverse_gui PROPERTIES
  OUTPUT_NAME BlotVerse
  MACOSX_BUNDLE_BUNDLE_NAME BlotVerse
  MACOSX_BUNDLE_GUI_IDENTIFIER org.blotverse.app
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})

# Must precede qt_add_qml_module.
set_source_files_properties(qml/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(blotverse_gui
  URI BlotVerse
  VERSION 1.0
  QML_FILES
    qml/Main.qml
    qml/Theme.qml
    qml/StartPage.qml
    qml/SetupPage.qml
    qml/RunningPage.qml
    qml/ResultsPage.qml
    qml/GlassCard.qml
    qml/AccentButton.qml
    qml/StatTile.qml
    qml/ProgressRing.qml
    qml/UniverseGrid.qml
    qml/SpecCurve.qml
    qml/InfluenceBars.qml
    qml/BlotView.qml
    qml/StageList.qml
)

# The frozen multiverse specification ships inside the application.
qt_add_resources(blotverse_gui "spec"
  PREFIX "/"
  BASE "${PROJECT_SOURCE_DIR}"
  FILES "${PROJECT_SOURCE_DIR}/spec/multiverse.yaml")

target_include_directories(blotverse_gui PRIVATE src)
target_link_libraries(blotverse_gui PRIVATE blotverse::blotverse Qt6::Quick Qt6::QuickControls2 Qt6::Concurrent)
blotverse_set_warnings(blotverse_gui)

if(BLOTVERSE_BUILD_TESTS)
  find_package(Qt6 REQUIRED COMPONENTS Test)
  qt_add_executable(blotverse_gui_tests tests/test_controller.cpp src/app_controller.cpp src/blot_image_provider.cpp)
  qt_add_resources(blotverse_gui_tests "spec_tests"
    PREFIX "/"
    BASE "${PROJECT_SOURCE_DIR}"
    FILES "${PROJECT_SOURCE_DIR}/spec/multiverse.yaml")
  target_include_directories(blotverse_gui_tests PRIVATE src)
  target_link_libraries(blotverse_gui_tests PRIVATE blotverse::blotverse Qt6::Quick Qt6::Concurrent Qt6::Test)
  add_test(NAME gui_controller COMMAND blotverse_gui_tests)
  set_tests_properties(gui_controller PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()
