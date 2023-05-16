cmake_minimum_required(VERSION 3.25)
project(os)

set(CMAKE_CXX_STANDARD 20)
set(SRCS
    test1
    test2
    test3
    )

foreach(SRC ${SRCS})
add_executable(${SRC}
            ${SRC}.cpp
            PCB.h PCB.cpp
            ProducerAndConsumer.cpp ProducerAndConsumer.h
        MemoryAllocation.h MemoryAllocation.cpp)
endforeach()

