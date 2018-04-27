cmake_minimum_required(VERSION 3.9)
project(SSDdemo)

include_directories(/usr/local/include)
include_directories(/usr/local/cuda/include)
include_directories(/home/pengshanzhen/ssd/caffe-ssd/build/install/include)
include_directories(/home/pengshanzhen/ssd/caffe-ssd/build/src)
###
link_directories(/usr/local/lib)
link_directories(/home/pengshanzhen/ssd/caffe-ssd/build/install/lib)
link_directories(/usr/local/cuda/lib64)


####
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
add_executable(SSDdemo main.cpp)


target_link_libraries(SSDdemo opencv_highgui opencv_imgcodecs opencv_imgproc opencv_video opencv_videoio opencv_core caffe glog gflags boost_system boost_filesystem)