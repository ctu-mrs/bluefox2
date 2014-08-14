#ifndef BLUEFOX2_ROS_CAMERA_H_
#define BLUEFOX2_ROS_CAMERA_H_

#include <boost/shared_ptr.hpp>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <camera_info_manager/camera_info_manager.h>

#include "opencv2/core/core.hpp"

#include "bluefox2/camera.h"
#include "bluefox2/SetExposeSrv.h"

namespace bluefox2 {

class RosCamera {
 public:
  RosCamera(const ros::NodeHandle &nh, std::string serial_name = "");
  void PublishImage(const cv::Mat &image, const ros::Time &time);

  const CameraConfig ReadConfig() const;
  boost::shared_ptr<Camera> camera;

 private:
  typedef boost::shared_ptr<camera_info_manager::CameraInfoManager>
      CameraInfoManagerPtr;
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::CameraPublisher camera_pub_;
  sensor_msgs::CameraInfoPtr cinfo_;
  CameraInfoManagerPtr cinfo_manager_;
  std::string frame_id_;
  ros::ServiceServer srv_server_;

  bool SetExposeUs(SetExposeSrv::Request &req, SetExposeSrv::Response &rsp);

};  // class SingleCamera

}  // namespace bluefox2

#endif  // BLUEFOX2_SINGLE_SINGLE_CAMERA_H_
