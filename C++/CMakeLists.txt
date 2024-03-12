# Set minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set Project Name and language used
project(polygonCut LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Google Testing Environment
####################################### Start
add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)
include(GoogleTest)

if ($ENV{GOOGLETEST_DIR})
    set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    if (NOT ${CMAKE_CURRENT_SOURCE_DIR}/googletest STREQUAL "")
        message(WARNING "Using googletest src dir specified at Qt Creator wizard")
    endif ()
    set(GOOGLETEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest)
endif ()

if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

include_directories(${GTestIncludes})


# Add source code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/GeometryCore.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/DataSet.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/GeometryFactory.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/GeometryFactory.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonViewer.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonViewer.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonConcaveCutter.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonConcaveCutter.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonEdit.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonEdit.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/SegmentIntersector.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/SegmentIntersector.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonConvexCutter.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonConvexCutter.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonAdvanced.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/PolygonAdvanced.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/ReferenceElt.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/ReferenceElt.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/AdvancedMesh.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/AdvancedMesh.cpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/AdvancedPolygonViewer.hpp)
list(APPEND source_code ${CMAKE_CURRENT_SOURCE_DIR}/src/AdvancedPolygonViewer.cpp)


# Add Unit Tests code
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_geometryfactory.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_RectangleCut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_viewer.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_PentagonCut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_MeshExagon.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_MeshPentagon.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_MeshDefinitive.hpp)

list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_TriangleCut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ConcavePolygonCut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ConcaveAdvanced1Cut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_ConcaveAdvanced2Cut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_DefinitivePolygonCut.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_MeshConcavePolygon.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_MeshTrianglePolygon.hpp)
list(APPEND test_code ${CMAKE_CURRENT_SOURCE_DIR}/test/test_MeshConcaveAdvanced.hpp)


# Add Eigen Library
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/eigen/include)

#add input text files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/input_files)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input_files/triangle_input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB MY_PUBLIC_INPUT
  "${CMAKE_CURRENT_SOURCE_DIR}/input_files/*.txt"
)
file(COPY ${MY_PUBLIC_INPUT} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Build executable
add_executable(${PROJECT_NAME} main.cpp ${source_code} ${test_code} ${GTestFiles})

# Set Unit Tests
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
gtest_discover_tests(${PROJECT_NAME})

