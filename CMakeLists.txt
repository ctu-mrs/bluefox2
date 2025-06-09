cmake_minimum_required(VERSION 3.8)
project(bluefox2)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# find dependencies
find_package(ament_cmake REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(mvIMPACT REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcl REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(diagnostic_msgs REQUIRED)
find_package(image_transport REQUIRED)
find_package(camera_info_manager REQUIRED)
find_package(diagnostic_updater REQUIRED)
find_package(rcl_interfaces REQUIRED)

# Only include custom directories
include_directories(
  include
  ${mvIMPACT_INCLUDE_DIRS}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Create library
add_library(${PROJECT_NAME}
    src/bluefox2.cpp
    src/bluefox2_ros.cpp
    src/bluefox2_settings.cpp
    src/single/single_node.cpp
)

target_link_libraries(${PROJECT_NAME} ${mvIMPACT_LIBRARIES})

ament_target_dependencies(${PROJECT_NAME} 
  sensor_msgs 
  std_msgs
  rclcpp 
  rcl 
  lifecycle_msgs 
  rclcpp_lifecycle 
  diagnostic_msgs
  image_transport
  camera_info_manager
)

# Create executable for single node
add_executable(${PROJECT_NAME}_single_node src/single/single_main.cpp)
target_link_libraries(${PROJECT_NAME}_single_node ${PROJECT_NAME})

ament_target_dependencies(${PROJECT_NAME}_single_node 
  sensor_msgs 
  std_msgs
  rclcpp 
  rcl 
  lifecycle_msgs 
  rclcpp_lifecycle 
  diagnostic_msgs
  image_transport
  camera_info_manager
)

# Install library
install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

# Install executables
install(TARGETS
  ${PROJECT_NAME}_single_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers
install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION include/${PROJECT_NAME})

# Install launch files
install(DIRECTORY
  launch/
  DESTINATION share/${PROJECT_NAME}/launch/
)

# Install config files if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config)
  install(DIRECTORY
    config/
    DESTINATION share/${PROJECT_NAME}/config/
  )
endif()

# Export dependencies
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(
  sensor_msgs
  std_msgs
  rclcpp
  rcl
  lifecycle_msgs
  rclcpp_lifecycle
  diagnostic_msgs
  image_transport
  camera_info_manager
  diagnostic_updater
  rcl_interfaces
)
ament_export_include_directories(include)

ament_package()