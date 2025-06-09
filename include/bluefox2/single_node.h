#ifndef BLUEFOX2_SINGLE_NODE_H_
#define BLUEFOX2_SINGLE_NODE_H_

#include "bluefox2/Bluefox2DynConfig.h"
#include <bluefox2/camera_node_base.h>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/float32.hpp>

namespace bluefox2 {

class Bluefox2Ros;

class SingleNode : public camera_base::CameraNodeBase<Bluefox2DynConfig> {
 public:
  explicit SingleNode(const rclcpp::Node::SharedPtr pnh);

  virtual void Acquire() override;
  virtual void Setup(Bluefox2DynConfig &config) override;
  virtual Bluefox2DynConfig GetCurrentConfig(rclcpp::Node::SharedPtr nh) override;

  void AcquireOnce();
  void callbackAec(const std_msgs::msg::Int32::ConstSharedPtr &expose_us);
  void callbackAgc(const std_msgs::msg::Float32::ConstSharedPtr &gain_db);
  
  // void PublishExposure(int expose_value, const rclcpp::Time& time, std::string camera_name, int fps_rate){
  //   exposure exposure;
  //   exposure.exposure_value = expose_value;
  //   exposure.header.stamp = time;
  //   exposure.header.frame_id = camera_name;
  //   exposure.header.seq = 0;
  //   exposure.fps = fps_rate;

  //   expose_pub.publish(exposure);
  // }

 private:
  std::shared_ptr<Bluefox2Ros> bluefox2_ros_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_expose_us;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_gain_db;
  rclcpp::Node::SharedPtr nh;
  
  //rclcpp::Publisher<exposure>::SharedPtr expose_pub;
  bool initialized = false;
};

}  // namespace bluefox2

#endif  // BLUEFOX2_SINGLE_NODE_H_
