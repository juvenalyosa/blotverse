add_executable(blotverse_tests
  test_core.cpp
  test_io.cpp
  test_imaging.cpp
  test_stats.cpp
  test_analysis.cpp
  test_sim.cpp
  test_pipeline.cpp
  test_validation.cpp
  test_forensics.cpp
)
target_link_libraries(blotverse_tests PRIVATE blotverse::blotverse doctest::doctest_with_main TIFF::TIFF)
target_compile_definitions(blotverse_tests PRIVATE BLOTVERSE_SPEC_PATH="${PROJECT_SOURCE_DIR}/spec/multiverse.yaml")
blotverse_set_warnings(blotverse_tests)

add_test(NAME blotverse_tests COMMAND blotverse_tests)

# CLI smoke test: simulate -> run -> reproduce.
if(BLOTVERSE_BUILD_APPS)
  set(_smoke ${CMAKE_CURRENT_BINARY_DIR}/smoke)
  add_test(NAME cli_simulate COMMAND blotverse_cli simulate --out ${_smoke}/sim --level moderate --seed 3)
  add_test(NAME cli_run COMMAND blotverse_cli run ${_smoke}/sim/experiment.yaml --out ${_smoke}/out
                                --spec ${PROJECT_SOURCE_DIR}/spec/multiverse.yaml --permutations 100)
  add_test(NAME cli_reproduce COMMAND blotverse_cli reproduce ${_smoke}/out/repro.yaml)
  set_tests_properties(cli_run PROPERTIES DEPENDS cli_simulate)
  set_tests_properties(cli_reproduce PROPERTIES DEPENDS cli_run)
endif()
