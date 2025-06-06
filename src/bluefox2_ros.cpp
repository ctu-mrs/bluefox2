#include "bluefox2/bluefox2_ros.h"

namespace bluefox2
{

Bluefox2Ros::Bluefox2Ros(const rclcpp::Node::SharedPtr nh, const std::string &prefix)
    : CameraRosBase(nh, prefix), bluefox2_(identifier())
{
  //  bluefox2_.OpenDevice();
  SetHardwareId(bluefox2_.serial());

  // Some hack for getting hardware sync to work
  //rclcpp::Node::SharedPtr cnh(nh, prefix);
  std::string mode;
  nh->declare_parameter("mode", "");
  nh->get_parameter("mode", mode);
  //cnh.param<std::string>("mode", mode, "");

  if (mode == "master") {
    bluefox2_.SetMaster();
  } else if (mode == "slave") {
    bluefox2_.SetSlave();
  }

  // Set mirror mode on construction
  int mm;
  //nh->declare_parameter("mm", 0);
  //nh->declare_parameter("mm", mm);
  //cnh.param<int>("mm", mm, 0);
  bluefox2_.SetMM(mm);

  // Set the upper exposure limit
  /* int exposure_upper_limit_us; */
  /* cnh.param<int>("exposure_upper_limit_us", exposure_upper_limit_us, 7500);
   */
  /* bluefox2_.SetExposeUpperLimit(exposure_upper_limit_us); */
}

bool Bluefox2Ros::Grab(const sensor_msgs::msg::Image::SharedPtr image_msg,
                       const sensor_msgs::msg::CameraInfo::SharedPtr cinfo_msg)
{
  // Add expose time to current time stamp
  return bluefox2_.GrabImage(image_msg);
}

} // namespace bluefox2
