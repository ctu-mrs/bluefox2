#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/set_parameters_result.hpp>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

namespace camera_base {

/**
 * @brief The CameraNodeBase class
 * A base class that implements camera node functionality using composition.
 * This class manages acquisition threading, parameter callbacks, and rate control.
 * It does NOT inherit from rclcpp::Node - instead it takes a node reference.
 */
template <typename ConfigType>
class CameraNodeBase {
 public:
  explicit CameraNodeBase(rclcpp::Node::SharedPtr node)
      : node_(node), 
        is_acquire_(false),
        rate_hz_(30.0) {
    
    // Set up parameter callback for this node
    //parameter_callback_handle_ = node_->add_on_set_parameters_callback(
    //    std::bind(&CameraNodeBase::ParameterCallback, this, std::placeholders::_1));
  }

  CameraNodeBase() = delete;
  CameraNodeBase(const CameraNodeBase&) = delete;
  CameraNodeBase& operator=(const CameraNodeBase&) = delete;
  virtual ~CameraNodeBase() { 
    End(); 
  }

  /**
   * @brief Get the underlying ROS2 node
   * @return Shared pointer to the node
   */
  rclcpp::Node::SharedPtr GetNode() const { return node_; }

  bool is_acquire() const { return is_acquire_.load(); }

  /**
   * @brief Run Initialize and start the node
   * This will get initial parameters, configure, and start acquisition
   */
  void Run() {
    RCLCPP_INFO(node_->get_logger(), "%s: Initializing camera node", 
                node_->get_namespace());
    
    // Get initial configuration from parameters
    ConfigType config = GetCurrentConfig(node_);
    Setup(config);
    SetRate(config.fps);
    Start();
  }

  /**
   * @brief End Stop acquisition and cleanup
   */
  void End() { 
    Stop(); 
  }

  /**
   * @brief Sleep according to the configured rate
   */
  void Sleep() {
    if (rate_) {
      rate_->sleep();
    }
  }

  /**
   * @brief Acquire Do acquisition here - pure virtual
   * This method should implement the main acquisition loop
   */
  virtual void Acquire() = 0;

  /**
   * @brief Setup Setup your camera here - pure virtual
   * @param config Configuration parameters
   */
  virtual void Setup(ConfigType& config) = 0;

  /**
   * @brief GetCurrentConfig Get current configuration from node parameters - pure virtual
   * Derived classes must implement this to read their specific parameters
   * @return Current configuration struct
   */
  virtual ConfigType GetCurrentConfig(rclcpp::Node::SharedPtr nh) = 0;

 protected:
  /**
   * @brief ParameterCallback Handle parameter changes
   * @param parameters List of changed parameters
   * @return Result indicating success/failure
   */
  rcl_interfaces::msg::SetParametersResult ParameterCallback(
      const std::vector<rclcpp::Parameter>& parameters) {
    
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    
    try {
      RCLCPP_INFO(node_->get_logger(), "%s: Reconfiguring due to parameter change", 
                  node_->get_namespace());
      
      // Check if acquisition is running
      bool was_acquiring = is_acquire();
      if (was_acquiring) {
        Stop();
      }
      
      // Get updated configuration
      ConfigType config = GetCurrentConfig(node_);
      
      // Update rate if fps changed
      for (const auto& param : parameters) {
        if (param.get_name() == "fps") {
          SetRate(param.as_double());
        }
      }
      
      // Reconfigure camera
      Setup(config);
      
      // Restart acquisition if it was running
      if (was_acquiring) {
        Start();
      }
      
      RCLCPP_INFO(node_->get_logger(), "%s: Parameter update successful", 
                  node_->get_namespace());
      
    } catch (const std::exception& e) {
      RCLCPP_ERROR(node_->get_logger(), "%s: Parameter update failed: %s", 
                   node_->get_namespace(), e.what());
      result.successful = false;
      result.reason = std::string("Configuration failed: ") + e.what();
    }
    
    return result;
  }

 private:
  /**
   * @brief SetRate Set the acquisition rate
   * @param fps Frames per second
   */
  void SetRate(double fps) { 
    rate_hz_ = fps;
    auto period = std::chrono::duration<double>(1.0 / fps);
    rate_ = std::make_unique<rclcpp::WallRate>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(period));
  }

  /**
   * @brief Start Begin acquisition thread
   */
  void Start() {
    if (is_acquire_.load()) {
      return;  // Already running
    }
    
    is_acquire_.store(true);
    acquire_thread_ = std::make_unique<std::thread>(&CameraNodeBase::Acquire, this);
    
    RCLCPP_INFO(node_->get_logger(), "%s: Camera acquisition started at %.1f Hz", 
                node_->get_namespace(), rate_hz_);
  }

  /**
   * @brief Stop Stop acquisition thread
   */
  void Stop() {
    if (!is_acquire_.load()) {
      return;  // Already stopped
    }
    
    RCLCPP_INFO(node_->get_logger(), "%s: Stopping camera acquisition...", 
                node_->get_namespace());
    is_acquire_.store(false);
    
    if (acquire_thread_ && acquire_thread_->joinable()) {
      acquire_thread_->join();
    }
    
    RCLCPP_INFO(node_->get_logger(), "%s: Camera acquisition stopped", 
                node_->get_namespace());
  }

  rclcpp::Node::SharedPtr node_;
  std::atomic<bool> is_acquire_;
  double rate_hz_;
  std::unique_ptr<rclcpp::WallRate> rate_;
  std::unique_ptr<std::thread> acquire_thread_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr parameter_callback_handle_;
};

} // namespace camera_base