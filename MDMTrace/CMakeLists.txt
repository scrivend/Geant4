cmake_minimum_required (VERSION 2.6)
project (MDMTrace)

enable_language (Fortran)
set (CMAKE_Fortran_FLAGS "-fPIC -finit-local-zero -falign-commons -fno-automatic")
set (CMAKE_CXX_FLAGS "-fPIC")

include_directories(include)

add_library (MDMTrace SHARED src/MDMTrace.cpp src/RAYTKIN1.F)
add_executable (mdmTrace src/mdmTraceApp.cpp src/jsoncpp.cpp)


target_link_libraries(MDMTrace c gfortran)
target_link_libraries(mdmTrace MDMTrace)

