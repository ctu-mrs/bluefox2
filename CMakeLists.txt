cmake_minimum_required(VERSION 3.8)
project(bluefox2)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# Remove this line:
# find_package(mvIMPACT REQUIRED)

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

# Set path to your local mvIMPACT installation
set(LOCAL_MVIMPACT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/install/x86_64/mvIMPACT_acquire-x86_64-2.48.0")
set(LOCAL_MVIMPACT_LIB_DIR "${LOCAL_MVIMPACT_DIR}/lib/x86_64")

# Set include directories
include_directories(
  include
  ${LOCAL_MVIMPACT_DIR}  # Your local includes
)

# Define local libraries explicitly
set(LOCAL_MVIMPACT_LIBRARIES
  ${LOCAL_MVIMPACT_LIB_DIR}/libmvDeviceManager.so.2.48.0
  ${LOCAL_MVIMPACT_LIB_DIR}/libmvBlueFOX.so.2.48.0
  ${LOCAL_MVIMPACT_LIB_DIR}/libmvPropHandling.so.2.48.0
  ${LOCAL_MVIMPACT_LIB_DIR}/libmvImageProcessing.so
  # Add any other libraries you need
)

# Optional: Add toolkit libraries if needed
set(LOCAL_TOOLKIT_LIBRARIES
  ${LOCAL_MVIMPACT_DIR}/Toolkits/expat/bin/x86_64/lib/libexpat.so.0.5.0
  ${LOCAL_MVIMPACT_DIR}/Toolkits/FreeImage3160/bin/Release/FreeImage/x86_64/libfreeimage-3.16.0.so
  # Add others if your code uses them
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

# Replace ${mvIMPACT_LIBRARIES} with your local libraries
target_link_libraries(${PROJECT_NAME} 
  ${LOCAL_MVIMPACT_LIBRARIES}
  ${LOCAL_TOOLKIT_LIBRARIES}
)

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

# Use ament_cmake to handle RPATH automatically
# This is the cleanest ROS2 way - no manual RPATH needed

# Alternative: Let ament handle RPATH for bundled libraries
# Only needed if the above doesn't work
# set_target_properties(${PROJECT_NAME} ${PROJECT_NAME}_single_node PROPERTIES
#   INSTALL_RPATH_USE_LINK_PATH TRUE
# )

# Install the local libraries with your package - ROS2 way
install(DIRECTORY ${LOCAL_MVIMPACT_LIB_DIR}/
  DESTINATION lib
  FILES_MATCHING PATTERN "*.so*"
)

# Install toolkit libraries - ROS2 way  
install(FILES 
  ${LOCAL_MVIMPACT_DIR}/Toolkits/expat/bin/x86_64/lib/libexpat.so.0.5.0
  ${LOCAL_MVIMPACT_DIR}/Toolkits/FreeImage3160/bin/Release/FreeImage/x86_64/libfreeimage-3.16.0.so
  DESTINATION lib
)

# # Create symbolic link for libexpat
# install(CODE "
#   execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
#     libexpat.so.0.5.0 
#     \${CMAKE_INSTALL_PREFIX}/lib/libexpat.so.0)
# ")

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

# Export dependencies and libraries for ROS2
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

# Export the bundled libraries so other packages can find them
ament_export_libraries(
  mvDeviceManager
  mvBlueFOX  
  mvPropHandling
  mvImageProcessing
  expat
  freeimage-3.16.0
)

ament_package()