PROJECT(vdb)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(VDB_VERSION_MAJOR 2)
SET(VDB_VERSION_MINOR 1)
SET(VDB_HELP_PATH ../help)
SET(VDB_SOURCE_PATH ../src)

SET(VDIS_ENUMS_PATH ../vdis/enumerations)
SET(VDIS_SOURCE_PATH ../vdis/src)
SET(VDIS_TEST_PATH ../vdis/test)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS "-g -std=c++0x -Wall")

SET(EXECUTABLE_OUTPUT_PATH ../)

CONFIGURE_FILE(
    ${VDB_SOURCE_PATH}/vdb_version.h.in
    ${VDB_SOURCE_PATH}/vdb_version.h
)

# This is needed to suppress the "relative path" warning for the 
# LINK_DIRECTORIES command...
CMAKE_POLICY(SET CMP0015 NEW)

FILE(GLOB VDIS_SOURCES ${VDIS_SOURCE_PATH}/*.cpp)
FILE(GLOB VDIS_TEST_SOURCES ${VDIS_TEST_PATH}/*.cpp)
FILE(GLOB VDB_SOURCES ${VDB_SOURCE_PATH}/*.cpp)

ADD_LIBRARY(vdis STATIC ${VDIS_SOURCES})
ADD_EXECUTABLE(vdis/vdis_test ${VDIS_TEST_SOURCES})
TARGET_LINK_LIBRARIES(vdis/vdis_test vdis)
TARGET_INCLUDE_DIRECTORIES(
    vdis PUBLIC
    ${VDIS_ENUMS_PATH}
    ${VDIS_SOURCE_PATH}
    ${VDIS_SOURCE_PATH}/enumerations
)

ADD_EXECUTABLE(vdb ${VDB_SOURCES})
TARGET_LINK_LIBRARIES(vdb vdis)
TARGET_INCLUDE_DIRECTORIES(
    vdb PUBLIC
    ${VDB_SOURCE_PATH}
    ${VDB_HELP_PATH}
    ${VDIS_SOURCE_PATH}
    ${VDIS_SOURCE_PATH}/enumerations
)
