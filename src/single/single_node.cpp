#include "bluefox2/single_node.h"
#include "bluefox2/bluefox2_ros.h"
#include "std_msgs/Int32.h"
#include <chrono> //jaa
#include <ostream>

namespace bluefox2
{
int counter = 0;
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point prev = begin;

SingleNode::SingleNode(const ros::NodeHandle &pnh)
    : CameraNodeBase(pnh), bluefox2_ros_(boost::make_shared<Bluefox2Ros>(pnh)),
      nh(pnh)
{
  sub_expose_us =
      nh.subscribe("expose_us", 1, &bluefox2::SingleNode::callbackAec, this);
  sub_gain_db =
      nh.subscribe("gain_db", 1, &bluefox2::SingleNode::callbackAgc, this);

  expose_pub = nh.advertise<exposure>("expose_publisher", 10);
}

void SingleNode::Acquire()
{
  while (is_acquire() && ros::ok()) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();

    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();

    const auto expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    const auto time = ros::Time::now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
    int fps_rate =
        1000000 /
        std::chrono::duration_cast<std::chrono::microseconds>(now - prev)
            .count();

    PublishExposure(expose_us, time, bluefox2_ros_->frame_id(), fps_rate);
    Sleep();
    prev = now;
  }
}

void SingleNode::AcquireOnce()
{
  if (is_acquire() && ros::ok()) {
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    const auto expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    const auto time = ros::Time::now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
  }
}

void SingleNode::Setup(Bluefox2DynConfig &config)
{
  bluefox2_ros_->set_fps(config.fps);
  bluefox2_ros_->camera().Configure(config);
}

void SingleNode::callbackAec(const std_msgs::Int32::ConstPtr &expose_us)
{
  int exp_us = int(expose_us->data);
  bluefox2_ros_->camera().callbackAec(exp_us);
}

void SingleNode::callbackAgc(const std_msgs::Float32::ConstPtr &gain_db)
{
  double gain = double(gain_db->data);
  bluefox2_ros_->camera().callbackAgc(gain);
}

} // namespace bluefox2
