###
project(OnvifTestServer)
cmake_minimum_required(VERSION 2.8)

# SIGRLOG_CURRLEVEL:
# level of verboseness of the logger range [0..4]
add_definitions(-DSIGRLOG_CURRLEVEL=4)
# SIGRLOG_OUTPUT:
# destinations of the logger output
# 0 - none, 1 - console only, 2 - file only, 3 - console and file
add_definitions(-DSIGRLOG_OUTPUT=1)
# SIGRLOG_FILENAME
# output filename for logger
add_definitions(-DSIGRLOG_FILENAME=\"oxxServerLog.ini\")

if("${SDK}" STREQUAL "")
    message(FATAL_ERROR "Set SDK variable. OnvifSDK library wasn't provided")
endif()

find_library(SDKLIB OnvifSDK ${SDK})
if( SDKLIB STREQUAL "SDKLIB-NOTFOUND" )
    message(FATAL_ERROR "OnvifSDK library wasn't found in ${SDK}")
endif()
message(STATUS "OSDK Library : ${SDKLIB}")

include_directories(${PROJECT_SOURCE_DIR}/../../common
                    ${PROJECT_SOURCE_DIR}/../../OnvifSDK/include
                    ${PROJECT_SOURCE_DIR}
                    ${SDK})


set(INC ${PROJECT_SOURCE_DIR}/../../OnvifSDK/include/OnvifSDK.h
        ${PROJECT_SOURCE_DIR}/../../common/sigrlog.h)

file( GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h )
file( GLOB SOURCES ${PROJECT_SOURCE_DIR}/*.cpp )
add_executable(OnvifTestServer  ${INC} ${HEADERS} ${SOURCES})
target_link_libraries(OnvifTestServer ${SDKLIB} pthread)

