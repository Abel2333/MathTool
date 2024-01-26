set(HEADER_LIST "${MathTool_SOURCE_DIR}/include/math_tool/state_machine.h")

add_library(state_machine basic_states.cc ${HEADER_LIST})

target_include_directories(state_machine PUBLIC ../include)

# All users of this library will need at least C++11
target_compile_features(state_machine PUBLIC cxx_std_11)
