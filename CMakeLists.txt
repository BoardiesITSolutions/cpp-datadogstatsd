cmake_minimum_required( VERSION 3.7 )
project( DataDogStatsD VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )
set( CMAKE_CXX_STANDARD 11 )

set( SOURCE
  DataDogStatsD.cpp
  DDEvent.cpp
  Helpers.cpp )

include_directories(include)

add_library( ${PROJECT_NAME}_shared SHARED ${SOURCE} )
set_target_properties( ${PROJECT_NAME}_shared PROPERTIES
  OUTPUT_NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION} )

add_library( ${PROJECT_NAME}_static STATIC ${SOURCE} )
set_target_properties( ${PROJECT_NAME}_static PROPERTIES
  OUTPUT_NAME ${PROJECT_NAME} )
