project(Slicer3ThicknessSpharm)

cmake_minimum_required(VERSION 2.6)

# Slicer3
find_package(Slicer3 REQUIRED)
include(${Slicer3_USE_FILE})

# Default install prefix
slicer3_set_default_install_prefix_for_external_projects()

FIND_PACKAGE(GenerateCLP REQUIRED)
IF(GenerateCLP_FOUND)
	INCLUDE(${GenerateCLP_USE_FILE})
ELSE(GenerateCLP_FOUND)
	MESSAGE(FATAL_ERROR "GenerateCLP not found. Please set GenerateCLP_DIR")
ENDIF(GenerateCLP_FOUND)

FIND_PACKAGE(BatchMake REQUIRED)
IF(BatchMake_FOUND)
	INCLUDE(${BatchMake_USE_FILE})
ELSE(BatchMake_FOUND)
	MESSAGE(FATAL_ERROR "BatchMake not found. Please set BatchMake_DIR")
ENDIF(BatchMake_FOUND)


include_directories(${CMAKE_BINARY_DIRECTORY})
#####################

set (CLP Slicer3ThicknessSpharm)
set (${CLP}_SOURCE ${CLP}.cpp)
generateclp(${CLP}_SOURCE ${CLP}.xml)

add_executable(${CLP} ${${CLP}_SOURCE} ${CLP}CLP.h)
slicer3_set_plugins_output_path(${CLP})
target_link_libraries (${CLP} ${ITK_LIBRARIES}  ${BatchMake_LIBRARIES})

#####################


#####################
# Tests for the plugin

enable_testing()


#####################
