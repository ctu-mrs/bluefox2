cmake_minimum_required(VERSION 3.1.5)
project(bluefox2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(catkin REQUIRED COMPONENTS
  roscpp nodelet camera_base
  message_generation
  )
find_package(mvIMPACT REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

# dynamic reconfigure
generate_dynamic_reconfigure_options(cfg/Bluefox2Dyn.cfg)

# services
add_service_files(FILES SetExposeSrv.srv)
generate_messages(DEPENDENCIES std_msgs)

set(LIBRARIES
  bluefox2
  )

catkin_package(
  INCLUDE_DIRS include ${mvIMPACT_INCLUDE_DIRS}
  LIBRARIES ${LIBRARIES}
  CATKIN_DEPENDS message_runtime
  )

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${mvIMPACT_INCLUDE_DIRS}
  )

add_subdirectory(src)

add_dependencies(bluefox2
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  ${PROJECT_NAME}_gencpp
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  )

# Install

install(TARGETS ${LIBRARIES}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
  )

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  )

install(DIRECTORY ./
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  FILES_MATCHING PATTERN "*.xml"
  )
