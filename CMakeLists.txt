cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

project(nifty)

set (NIFTY_VERSION_MAJOR 0)
set (NIFTY_VERSION_MINOR 2)
set (NIFTY_VERSION_PATCH 0)

set (NIFTY_VERSION_SHORT_STR "${NIFTY_VERSION_MAJOR}.${NIFTY_VERSION_MINOR}")
set (NIFTY_VERSION_STR "${NIFTY_VERSION_MAJOR}.${NIFTY_VERSION_MINOR}.${NIFTY_VERSION_PATCH}")


include(CheckCXXCompilerFlag)


option(WITH_BOOST_COMPUTE OFF)
option(WITH_GUROBI OFF)
option(WITH_CPLEX OFF)
option(WITH_GLPK OFF)
option(WITH_HDF5 OFF)
option(WITH_FASTFILTERS OFF)


option(BUILD_CPP_TEST ON)
option(BUILD_NIFTY_PYTHON OFF)
option(BUILD_DOCS ON)
option(BUILD_CPP_EXAMPLES ON)
option(BUILD_PYTHON_DOCS "Build the Python documentation with Sphinx" OFF)



#-------------------------------------------------------------------------------------------------------------------
# check for c++ 11 support
#-------------------------------------------------------------------------------------------------------------------
# FIXME c++14 checks are broken!
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)
    if (HAS_CPP14_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif (HAS_CPP11_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "Unsupported compiler -- nifty  requires C++11 support!")
    endif()
endif()

#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#-------------------------------------------------------------------------------------------------------------------
# Set a default build configuraton if none is specified. 'MinSizeRel' produces the smallest binaries
#-------------------------------------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'MinSizeRel' as none was specified.")
    set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)



#-------------------------------------------------------------------------------------------------------------------
# thread libs
#-------------------------------------------------------------------------------------------------------------------
find_package(Threads)


#-------------------------------------------------------------------------------------------------------------------
# rt lib
#-------------------------------------------------------------------------------------------------------------------
if(UNIX AND NOT APPLE)
  find_library(RT rt) 
  set(LINK_RT true) 
  #message(STATUS "Linking to RT is enabled")
else()
  set(LINK_RT false) 
  #message(STATUS "Linking to RT is diabled")
endif()


#-------------------------------------------------------------------------------------------------------------------
# externals
#-------------------------------------------------------------------------------------------------------------------
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/externals/marray/include")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/externals/pybind11/include")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/externals/vigra/include")
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/externals/graph/include")


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/externals/pybind11")



#-------------------------------------------------------------------------------------------------------------------
# hdf5 libs
#-------------------------------------------------------------------------------------------------------------------

if(WITH_HDF5)
    find_package(HDF5)
    include_directories(${HDF5_INCLUDE_DIR})
    add_definitions(-DWITH_HDF5)
endif()

if(WITH_BOOST_COMPUTE)
    #----------------------------------------------------------------------------------------------------------------
    # find OpenGL
    #----------------------------------------------------------------------------------------------------------------
    find_package(OpenCL REQUIRED)
    include_directories(${OpenCL_INCLUDE_DIRS})

    #----------------------------------------------------------------------------------------------------------------
    # find BOOST_COMPUTE
    #----------------------------------------------------------------------------------------------------------------
    find_package(BOOST_COMPUTE REQUIRED)
    include_directories(${BOOST_COMPUTE_INCLUDE_DIR})
endif()


#-------------------------------------------------------------------------------------------------------------------
# find boost
#-------------------------------------------------------------------------------------------------------------------
find_package(Boost  REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
MESSAGE( STATUS ${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
MESSAGE( STATUS ${Boost_INCLUDE_DIR})


if(WITHIN_TRAVIS)
    add_definitions(-DWITHIN_TRAVIS)
endif()


#-------------------------------------------------------------------------------------------------------------------
# find LP SOLVERS
#-------------------------------------------------------------------------------------------------------------------

SET(NIFTY_LP_SOLVER_LIBS "")
#-------------------------------------------------------------------------------------------------------------------
# find cplex
#-------------------------------------------------------------------------------------------------------------------
if(WITH_CPLEX)
    find_package(CPLEX REQUIRED)
    include_directories(${CPLEX_INCLUDE_DIRS})
    add_definitions(-DWITH_CPLEX)
    SET(NIFTY_LP_SOLVER_LIBS "${NIFTY_LP_SOLVER_LIBS};${CPLEX_LIBRARIES}")
endif()

#-------------------------------------------------------------------------------------------------------------------
# find gurobi
#-------------------------------------------------------------------------------------------------------------------
if(WITH_GUROBI)
    find_package(GUROBI REQUIRED)
    include_directories(${GUROBI_INCLUDE_DIR})
    add_definitions(-DWITH_GUROBI)
    SET(NIFTY_LP_SOLVER_LIBS "${NIFTY_LP_SOLVER_LIBS};${GUROBI_LIBRARIES}")
endif()

#-------------------------------------------------------------------------------------------------------------------
# find glpk
#-------------------------------------------------------------------------------------------------------------------
if(WITH_GLPK)
    find_package(GLPK REQUIRED)
    include_directories(${GLPK_INCLUDE_DIR})
    add_definitions(-DWITH_GLPK)
    SET(NIFTY_LP_SOLVER_LIBS "${NIFTY_LP_SOLVER_LIBS};${GLPK_LIBRARY}")
endif()






#-------------------------------------------------------------------------------------------------------------------
# find fastfilters
#-------------------------------------------------------------------------------------------------------------------
if(WITH_FASTFILTERS)
    find_package(FASTFILTERS REQUIRED)
    include_directories(${FASTFILTERS_INCLUDE_DIR})
    add_definitions(-DWITH_FASTFILTERS)
endif()







#-------------------------------------------------------------------------------------------------------------------
# enable_testing
#-------------------------------------------------------------------------------------------------------------------
enable_testing()

#-------------------------------------------------------------------------------------------------------------------
# global headers
#-------------------------------------------------------------------------------------------------------------------
file(GLOB_RECURSE headers include/*.hxx)
file(GLOB_RECURSE headers ${CMAKE_INSTALL_PREFIX}/include/*.hxx)
include_directories(include)


#-------------------------------------------------------------------------------------------------------------------
# src folder
#-------------------------------------------------------------------------------------------------------------------
add_subdirectory(src)


#-------------------------------------------------------------------------------------------------------------------
# docsrc folder
#-------------------------------------------------------------------------------------------------------------------
add_subdirectory(docsrc)
