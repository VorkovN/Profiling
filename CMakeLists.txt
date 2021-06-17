cmake_minimum_required(VERSION 3.19)
project(Lab4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-pthread -lprofiler")# -Wall -pg

include_directories(Other/include/ ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/include)



set(SRC
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src/atomic.c
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src/common.c
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src/log.c
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src/process.c
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src/registry.c
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src/shm.c
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/CmxException.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/CmxSupport.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/CmxTypes.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/Component.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/ImmutableComponent.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/Log.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/ProcessComponent.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/Registry.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/impl/CmxSupportImpl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/demo/Demo1.cpp
        ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/src-cpp/demo/Demo2.cpp
        )

add_executable(${PROJECT_NAME} main.cpp ${SRC})
add_executable(cmw-cmx-control ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/tools/cmw-cmx-control.cpp ${SRC})
add_executable(cmw-cmx-dumper  ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/tools/cmw-cmx-dumper.cpp ${SRC})
add_executable(cmw-cmx-reader  ${CMAKE_CURRENT_LIST_DIR}/cmw-cmx/tools/cmw-cmx-reader.cpp ${SRC})
target_link_libraries(${PROJECT_NAME} -lrt -lprofiler)
target_link_libraries(cmw-cmx-control -lrt)
target_link_libraries(cmw-cmx-dumper -lrt)
target_link_libraries(cmw-cmx-reader -lrt)