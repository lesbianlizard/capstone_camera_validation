cmake_minimum_required(VERSION 2.8)
project( corrupt )
find_package( OpenCV REQUIRED )

include(FindProtobuf)
find_package(Protobuf REQUIRED)


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

include_directories( ${OpenCV_INCLUDE_DIRS} ${ZeroMQ_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})

add_executable( corrupt filters/Distortion.hpp filters/Distortion.cpp 
                filters/Freeze.hpp filters/Freeze.cpp 
                filters/White.hpp filters/White.cpp 
                filters/Translate.hpp filters/Translate.cpp 
                Compare.hpp Compare.cpp 
                socket/PracticalSocket.h socket/PracticalSocket.cpp 
                proto/packet.pb.h proto/packet.pb.cc main_reed.cpp)

target_link_libraries( corrupt ${OpenCV_LIBS}  ${ZeroMQ_LIBRARY} ${PROTOBUF_LIBRARIES} pthread)
