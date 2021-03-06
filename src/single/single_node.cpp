#include "bluefox2/single_node.h"
#include "bluefox2/bluefox2_ros.h"

namespace bluefox2 {

SingleNode::SingleNode(const ros::NodeHandle& pnh)
    : CameraNodeBase(pnh),
      bluefox2_ros_(boost::make_shared<Bluefox2Ros>(pnh)),
      nh(pnh)    
      {
        sub_expose_us = nh.subscribe("expose_us", 1, &bluefox2::SingleNode::callbackAec, this);
        sub_gain_db = nh.subscribe("gain_db", 1, &bluefox2::SingleNode::callbackAgc, this);
      }

void SingleNode::Acquire() {
  while (is_acquire() && ros::ok()) {
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    const auto expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    const auto time = ros::Time::now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
    Sleep();
  }
}

void SingleNode::AcquireOnce() {
  if (is_acquire() && ros::ok()) {
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    const auto expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    const auto time = ros::Time::now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
  }
}

void SingleNode::Setup(Bluefox2DynConfig& config) {
  bluefox2_ros_->set_fps(config.fps);
  bluefox2_ros_->camera().Configure(config);
}


void SingleNode::callbackAec(const std_msgs::Int32::ConstPtr &expose_us) {
    int exp_us = int(expose_us->data);
    bluefox2_ros_->camera().callbackAec(exp_us);
}

void SingleNode::callbackAgc(const std_msgs::Float32::ConstPtr &gain_db) {
    double gain = double(gain_db->data);
    bluefox2_ros_->camera().callbackAgc(gain);
}

}  // namepace bluefox2
