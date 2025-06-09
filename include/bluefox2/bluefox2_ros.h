#ifndef BLUEFOX2_ROS_H_
#define BLUEFOX2_ROS_H_

#include "bluefox2/bluefox2.h"
#include "bluefox2/camera_ros_base.h"

namespace bluefox2 {

class Bluefox2Ros : public camera_base::CameraRosBase {
 public:
  explicit Bluefox2Ros(const rclcpp::Node::SharedPtr nh,
                       const std::string& prefix = std::string());

  void RequestSingle() const { bluefox2_.RequestSingle(); }
  Bluefox2& camera() { return bluefox2_; }

  bool Grab(const sensor_msgs::msg::Image::SharedPtr image_msg,
            const sensor_msgs::msg::CameraInfo::SharedPtr cinfo_msg = nullptr) override;

 private:
  Bluefox2 bluefox2_;
};

}  // namespace bluefox2

#endif  // BLUEFOX2_ROS_H_
