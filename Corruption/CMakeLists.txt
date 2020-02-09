cmake_minimum_required(VERSION 2.8)
project( corrupt )
find_package( OpenCV REQUIRED )


find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ QUIET zmq)
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )
        
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

include_directories( ${OpenCV_INCLUDE_DIRS} ${ZeroMQ_INCLUDE_DIR})
add_executable( corrupt Distortion.hpp Distortion.cpp Freeze.hpp Freeze.cpp White.hpp White.cpp Translate.hpp Translate.cpp PracticalSocket.cpp Compare.hpp Compare.cpp main.cpp)
target_link_libraries( corrupt ${OpenCV_LIBS}  ${ZeroMQ_LIBRARY} pthread)