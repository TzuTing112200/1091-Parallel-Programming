
cmake_minimum_required(VERSION 2.8)
project(MedianFilterThread)
find_package( OpenCV REQUIRED )
add_executable( MedianFilter MedianFilter_pthread.cpp )
find_package(Threads)
target_link_libraries( MedianFilter ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
