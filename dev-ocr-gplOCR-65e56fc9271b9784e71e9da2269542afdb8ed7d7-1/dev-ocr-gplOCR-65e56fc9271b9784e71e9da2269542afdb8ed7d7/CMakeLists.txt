cmake_minimum_required(VERSION 2.8)
project( gplOCR )
find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )


file(GLOB_RECURSE gplOCRsrc
#    "src/*.h"
    "src/*.cpp"
#    "src/*.c"
)

add_executable(gplOCR  ${gplOCRsrc})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
target_link_libraries(gplOCR  ${OpenCV_LIBS})
