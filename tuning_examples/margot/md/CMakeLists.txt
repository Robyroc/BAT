cmake_minimum_required(VERSION 3.10)

project(md)

set (margot_heel_generator_DIR "/home/roberto/Documents/vmargot/core+agora/core/install/lib/cmake/margot")
find_package (margot_heel_generator REQUIRED)
find_package (CUDA REQUIRED)

margot_heel_generate_interface("/home/roberto/Documents/vmargot/BAT/tuning_examples/margot/md/margot.json")

#set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_executable(MD md.cpp)
target_link_libraries(MD margot::margot_heel_interface ${CUDA_LIBRARIES})
target_compile_features(MD PRIVATE cxx_std_17 )