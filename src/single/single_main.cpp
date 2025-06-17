#include "bluefox2/single_node.h"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  
  try {

    auto nh = std::make_shared<rclcpp::Node>("bluefox2_single");

      // Area of Interest - these might need to be set elsewhere or have defaults
    nh->declare_parameter<int>("width", 752);  // Default camera resolution
    nh->declare_parameter<int>("height", 480);
    // Pixel Format
    nh->declare_parameter<int>("idpf", 0);
    // Binning
    nh->declare_parameter<int>("cbm", 0);
    // Gain settings
    nh->declare_parameter<bool>("agc", true);
    nh->declare_parameter<double>("gain_db", 0.0);
    // Exposure settings
    nh->declare_parameter<bool>("aec", true);
    nh->declare_parameter<int>("expose_us", 10000);
    // Auto Controller settings
    nh->declare_parameter<int>("acs", 2);
    nh->declare_parameter<int>("des_grey_value", 128);
    // Auto exposure upper limit
    nh->declare_parameter<int>("expose_upper_limit_us", 100000);
    // White Balance settings
    nh->declare_parameter<int>("wbp", 3);
    // Note: r_gain, g_gain, b_gain are not in the launch file, using defaults
    nh->declare_parameter<double>("r_gain", 1.0);
    nh->declare_parameter<double>("g_gain", 1.0);
    nh->declare_parameter<double>("b_gain", 1.0);
    // High Dynamic Range
    nh->declare_parameter<bool>("hdr", false);
    // Dark Current Filter
    nh->declare_parameter<int>("dcfm", 0);
    // Pixel Clock - not in launch file, using default
    nh->declare_parameter<int>("cpc", 40000);
    // Trigger Mode
    nh->declare_parameter<int>("ctm", 1);
    // Trigger Source - not in launch file, using default
    nh->declare_parameter<int>("cts", -1);
    // Max expose jump
    nh->declare_parameter<int>("max_expose_jump", 20000);
    // Request
    nh->declare_parameter<int>("request", 3);
    // Frame rate
    nh->declare_parameter<int>("fps", 60);

    nh->declare_parameter<std::string>("calib_url", "");
    nh->declare_parameter<std::string>("camera_name", "");
    nh->declare_parameter<std::string>("frame_id", "");
    nh->declare_parameter<std::string>("identifier", "");
    nh->declare_parameter<int>("mm", 0);

    bluefox2::SingleNode single_node(nh);
    single_node.Run();
    rclcpp::spin(nh);
    single_node.End();
  } catch (const std::exception &e) {
    RCLCPP_ERROR(rclcpp::get_logger("bluefox2_single"), "%s", e.what());
  }
  
  rclcpp::shutdown();
  return 0;
}