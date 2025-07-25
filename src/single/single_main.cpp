#include "bluefox2/single_node.h"
#include "rclcpp/rclcpp.hpp"

namespace bluefox2 {

class BluefoxSingleComponent : public rclcpp::Node {
  private:
    std::shared_ptr<bluefox2::SingleNode> single_node;
    bool initialized_ = false;
    rclcpp::TimerBase::SharedPtr init_timer_;

  public:
    BluefoxSingleComponent(const rclcpp::NodeOptions & options) 
      : Node("bluefox2_single", options) {
      // Only declare parameters in constructor
      declare_parameter<int>("width", 752);
      declare_parameter<int>("height", 480);
      declare_parameter<int>("idpf", 0);
      declare_parameter<int>("cbm", 0);
      declare_parameter<bool>("agc", true);
      declare_parameter<double>("gain_db", 0.0);
      declare_parameter<bool>("aec", true);
      declare_parameter<int>("expose_us", 10000);
      declare_parameter<int>("acs", 2);
      declare_parameter<int>("des_grey_value", 128);
      declare_parameter<int>("expose_upper_limit_us", 100000);
      declare_parameter<int>("wbp", 3);
      declare_parameter<double>("r_gain", 1.0);
      declare_parameter<double>("g_gain", 1.0);
      declare_parameter<double>("b_gain", 1.0);
      declare_parameter<bool>("hdr", false);
      declare_parameter<int>("dcfm", 0);
      declare_parameter<int>("cpc", 40000);
      declare_parameter<int>("ctm", 1);
      declare_parameter<int>("cts", -1);
      declare_parameter<int>("max_expose_jump", 20000);
      declare_parameter<int>("request", 3);
      declare_parameter<int>("fps", 60);
      declare_parameter<std::string>("calib_url", "");
      declare_parameter<std::string>("camera_name", "");
      declare_parameter<std::string>("frame_id", "");
      declare_parameter<std::string>("identifier", "");
      declare_parameter<int>("mm", 0);

      //init_timer_ = this->create_wall_timer(std::chrono::milliseconds(0), [this]() { this->initialize(); }
      init_timer_ = this->create_wall_timer(std::chrono::milliseconds(0), std::bind(&BluefoxSingleComponent::initialize, this));
    }

    // This is your "onInit" equivalent method!
    void initialize() {
      if (initialized_) {
        RCLCPP_WARN(get_logger(), "Already initialized, skipping...");
        return;
      }

      try {
        RCLCPP_INFO(get_logger(), "Initializing BlueFox camera...");
        
        // Cancel the timer since we only need to initialize once
        init_timer_->cancel();
        
        // Now shared_from_this() works since the component is fully constructed
        single_node = std::make_shared<bluefox2::SingleNode>(shared_from_this());
        single_node->Run();
        
        initialized_ = true;
        RCLCPP_INFO(get_logger(), "BlueFox camera initialized successfully");
      } catch (const std::exception &e) {
        RCLCPP_ERROR(get_logger(), "Failed to initialize BlueFox camera: %s", e.what());
        // Don't re-throw here since we're in a timer callback
      }
    }

    ~BluefoxSingleComponent() {
      if (single_node) {
        single_node->End();
      }
    }
};

}

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(bluefox2::BluefoxSingleComponent)