#include "bluefox2/single_node.h"
#include "bluefox2/bluefox2_ros.h"
#include "std_msgs/msg/int32.h"
#include "bluefox2/Bluefox2DynConfig.h"
#include <chrono> //jaa
#include <ostream>

using namespace std::placeholders;

namespace bluefox2
{
int counter = 0;
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point prev = begin;

SingleNode::SingleNode(const rclcpp::Node::SharedPtr pnh)
    : nh(pnh), CameraNodeBase(pnh), bluefox2_ros_(std::make_shared<Bluefox2Ros>(pnh))
{
  //sub_expose_us =
  //    nh.subscribe("expose_us", 1, &bluefox2::SingleNode::callbackAec, this);
  sub_expose_us = nh->create_subscription<std_msgs::msg::Int32>("expose_us", 10, std::bind(&bluefox2::SingleNode::callbackAec, this, _1));
  //sub_gain_db =
  //    nh.subscribe("gain_db", 1, &bluefox2::SingleNode::callbackAgc, this);
  sub_gain_db = nh->create_subscription<std_msgs::msg::Float32>("gain_db", 10, std::bind(&bluefox2::SingleNode::callbackAgc, this, _1));

  //expose_pub = nh.advertise<exposure>("expose_publisher", 10);
}

void SingleNode::Acquire()
{
  while (is_acquire() && rclcpp::ok()) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    const auto expose_duration = rclcpp::Duration(0, expose_us * 1e3 / 2);
    const auto time = nh->now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
    // int fps_rate =
    //     1000000 /
    //     std::chrono::duration_cast<std::chrono::microseconds>(now - prev)
    //         .count();
    // PublishExposure(expose_us, time, bluefox2_ros_->frame_id(), fps_rate);
    bluefox2_ros_->camera().RescaleExposeLimits(expose_us);
    prev = now;
    Sleep();
  }
}

void SingleNode::AcquireOnce()
{
  if (is_acquire() && rclcpp::ok()) {
    bluefox2_ros_->RequestSingle();
    const auto expose_us = bluefox2_ros_->camera().GetExposeUs();
    const auto expose_duration = rclcpp::Duration(0, expose_us * 1e3 / 2);
    const auto time = nh->now() + expose_duration;
    bluefox2_ros_->PublishCamera(time);
  }
}

void SingleNode::Setup(Bluefox2DynConfig &config)
{
  bluefox2_ros_->set_fps(config.fps);
  bluefox2_ros_->camera().Configure(config);
}

void getAecCallback(){
  getAec();
}

Bluefox2DynConfig SingleNode::GetCurrentConfig(rclcpp::Node::SharedPtr nh){
  Bluefox2DynConfig config;

  try {
      // Get existing parameters
      config.width = nh->get_parameter("width").as_int();
      config.height = nh->get_parameter("height").as_int();
      config.idpf = nh->get_parameter("idpf").as_int();
      config.cbm = nh->get_parameter("cbm").as_int();
      config.agc = nh->get_parameter("agc").as_bool();
      config.gain_db = nh->get_parameter("gain_db").as_double();
      config.aec = nh->get_parameter("aec").as_bool();
      config.expose_us = nh->get_parameter("expose_us").as_int();
      config.acs = nh->get_parameter("acs").as_int();
      config.des_grey_value = nh->get_parameter("des_grey_value").as_int();
      config.expose_upper_limit_us = nh->get_parameter("expose_upper_limit_us").as_int();
      config.wbp = nh->get_parameter("wbp").as_int();
      config.r_gain = nh->get_parameter("r_gain").as_double();
      config.g_gain = nh->get_parameter("g_gain").as_double();
      config.b_gain = nh->get_parameter("b_gain").as_double();
      config.hdr = nh->get_parameter("hdr").as_bool();
      config.dcfm = nh->get_parameter("dcfm").as_int();
      config.cpc = nh->get_parameter("cpc").as_int();
      config.ctm = nh->get_parameter("ctm").as_int();
      config.cts = nh->get_parameter("cts").as_int();
      config.max_expose_jump = nh->get_parameter("max_expose_jump").as_int();
      config.request = nh->get_parameter("request").as_int();
      config.fps = nh->get_parameter("fps").as_int();
      
  } catch (const rclcpp::exceptions::ParameterNotDeclaredException& e) {
      RCLCPP_ERROR(nh->get_logger(), "Parameter not found: %s", e.what());
      // Set default values or handle error as needed
  }
  
  return config;
}

void SingleNode::callbackAec(const std_msgs::msg::Int32::ConstSharedPtr &expose_us)
{
  int exp_us = int(expose_us->data);
  bluefox2_ros_->camera().callbackAec(exp_us);
}

void SingleNode::callbackAgc(const std_msgs::msg::Float32::ConstSharedPtr &gain_db)
{
  double gain = double(gain_db->data);
  bluefox2_ros_->camera().callbackAgc(gain);
}

} // namespace bluefox2
