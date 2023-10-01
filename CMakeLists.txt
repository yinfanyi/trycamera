cmake_minimum_required(VERSION 3.0.0)
project(camera VERSION 0.1.0 LANGUAGES C CXX)

IF(UNIX)
    set(CMAKE_CXX_FLAGS "-std=c++17 -pthread ${CMAKE_CXX_FLAGS}")
ENDIF()

set(PROJECT_NAME "camera")     # Create project name
project(${PROJECT_NAME})                        # Create project

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(OrbbecSDK_ROOT_DIR ${PROJECT_SOURCE_DIR}/thirdParty/SDK)
set(OrbbecSDK_LIBRARY_DIRS ${OrbbecSDK_ROOT_DIR}/lib)
set(OrbbecSDK_INCLUDE_DIR ${OrbbecSDK_ROOT_DIR}/include)
include_directories(${OrbbecSDK_INCLUDE_DIR})
link_directories(${OrbbecSDK_LIBRARY_DIRS})

set(HEADERS ${PROJECT_SOURCE_DIR}/include)
include_directories(${HEADERS})

aux_source_directory("${PROJECT_SOURCE_DIR}/src" SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})


set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX)
#linux system set sample rpath
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN:${CMAKE_INSTALL_RPATH}:${OrbbecSDK_LIBRARY_DIRS}")
    if(NOT APPLE)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")
    endif()
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(${PROJECT_NAME} OrbbecSDK ${OpenCV_LIBS})


