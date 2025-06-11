#pragma once

#include <rclcpp/rclcpp.hpp>
#include <camera_info_manager/camera_info_manager.hpp>
#include <image_transport/image_transport.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>
//#include <sensor_msgs/image_encodings.hpp>
//#include <diagnostic_updater/diagnostic_updater.hpp>
//#include <diagnostic_updater/publisher.hpp>

namespace camera_base {

/**
 * @brief getParam Util function for getting ROS2 parameters
 * @param node Node pointer
 * @param name Parameter name
 * @return Parameter value
 */
template <typename T>
T getParam(rclcpp::Node::SharedPtr node, const std::string& name) {
  T value{};
  if (!node->get_parameter(name, value)) {
    RCLCPP_ERROR(node->get_logger(), "Cannot find parameter: %s", name.c_str());
  }
  return value;
}

/**
 * @brief The CameraRosBase class
 * This class implements a ROS2 camera with publishing and diagnostic capabilities
 */
class CameraRosBase {
 public:
  explicit CameraRosBase(rclcpp::Node::SharedPtr node,
                         const std::string& prefix = std::string())
      : node_(node),
        fps_(10.0),
        camera_name_(getParam<std::string>(node_, "camera_name")),
        calib_url_(getParam<std::string>(node_, "calib_url")) {
    
    // Initialize image transport
    image_transport_ = std::make_shared<image_transport::ImageTransport>(node_);
    
    // Create camera publisher
    std::string topic_name = prefix.empty() ? "image_raw" : (prefix + "/image_raw");
    camera_pub_ = image_transport_->advertiseCamera(topic_name, 1);
    
    // Initialize camera info manager
    camera_info_manager_ = std::make_shared<camera_info_manager::CameraInfoManager>(
      node_.get(), camera_name_, calib_url_);
          
    // Get frame_id parameter
    //node_->declare_parameter("frame_id", node_->get_namespace());
    frame_id_ = node_->get_parameter("frame_id").as_string();
    
    // Get identifier parameter
    //node_->declare_parameter("identifier", "");
    identifier_ = node_->get_parameter("identifier").as_string();
    
    // Initialize diagnostic updater
    //diagnostic_updater_ = std::make_shared<diagnostic_updater::Updater>(node_);
    
    // Set up topic diagnostic
    //diagnostic_updater::FrequencyStatusParam freq_params(&fps_, &fps_, 0.1, 10);
    //diagnostic_updater::TimeStampStatusParam time_params(-0.01, 0.1);
    
    //topic_diagnostic_ = std::make_shared<diagnostic_updater::TopicDiagnostic>(
    //    topic_name, *diagnostic_updater_, freq_params, time_params);
  }

  CameraRosBase() = delete;
  CameraRosBase(const CameraRosBase&) = delete;
  CameraRosBase& operator=(const CameraRosBase&) = delete;
  virtual ~CameraRosBase() = default;

  const std::string& identifier() const { return identifier_; }
  const std::string& frame_id() const { return frame_id_; }
  const std::string& camera_name() const { return camera_name_; }

  double fps() const { return fps_; }
  void set_fps(double fps) { fps_ = fps; }

  /**
   * @brief SetHardwareId Set hardware id for diagnostic updater
   * @param id hardware id
   */
  void SetHardwareId(const std::string& id) {
    //diagnostic_updater_->setHardwareID(id);
  }

  /**
   * @brief PublishCamera Publish a camera topic with Image and CameraInfo
   * @param time Acquisition time stamp
   */
  void PublishCamera(const rclcpp::Time& time) {
    auto image_msg = std::make_shared<sensor_msgs::msg::Image>();
    auto cinfo_msg = std::make_shared<sensor_msgs::msg::CameraInfo>(
        camera_info_manager_->getCameraInfo());
    
    image_msg->header.frame_id = frame_id_;
    image_msg->header.stamp = time;
    
    if (Grab(image_msg, cinfo_msg)) {
      // Update camera info header
      cinfo_msg->header = image_msg->header;
      camera_pub_.publish(*image_msg, *cinfo_msg);
      //topic_diagnostic_->tick(image_msg->header.stamp);
    }
    else{
      RCLCPP_ERROR(node_->get_logger(), "Error: could not grab frame");
    }
    
    //diagnostic_updater_->update();
  }

  /**
   * @brief Publish Publish image with automatically generated camera info
   * @param image_msg Pre-filled image message
   */
  void Publish(const sensor_msgs::msg::Image::SharedPtr& image_msg) {
    auto cinfo_msg = std::make_shared<sensor_msgs::msg::CameraInfo>(
        camera_info_manager_->getCameraInfo());
    
    // Update camera info header
    image_msg->header.frame_id = frame_id_;
    cinfo_msg->header = image_msg->header;
    
    camera_pub_.publish(*image_msg, *cinfo_msg);
    //topic_diagnostic_->tick(image_msg->header.stamp);
    //diagnostic_updater_->update();
  }

  /**
   * @brief GetCameraInfoManager Get the camera info manager for external configuration
   * @return Shared pointer to camera info manager
   */
  std::shared_ptr<camera_info_manager::CameraInfoManager> GetCameraInfoManager() {
    return camera_info_manager_;
  }

  /**
   * @brief GetNode Get the underlying ROS2 node
   * @return Shared pointer to the node
   */
  rclcpp::Node::SharedPtr GetNode() {
    return node_;
  }

  /**
   * @brief Grab Fill image_msg and cinfo_msg from low level camera driver
   * @param image_msg ROS2 message ImagePtr
   * @param cinfo_msg ROS2 message CameraInfoPtr (optional)
   * @return True if successful
   */
  virtual bool Grab(const sensor_msgs::msg::Image::SharedPtr image_msg,
                    const sensor_msgs::msg::CameraInfo::SharedPtr cinfo_msg = nullptr) = 0;

 protected:
  rclcpp::Node::SharedPtr node_;
  std::shared_ptr<image_transport::ImageTransport> image_transport_;
  image_transport::CameraPublisher camera_pub_;
  std::shared_ptr<camera_info_manager::CameraInfoManager> camera_info_manager_;
  
  double fps_;
  //std::shared_ptr<diagnostic_updater::Updater> diagnostic_updater_;
  //std::shared_ptr<diagnostic_updater::TopicDiagnostic> topic_diagnostic_;
  
  std::string frame_id_;
  std::string identifier_;
  std::string camera_name_;
  std::string calib_url_;
};

} // namespace camera_base