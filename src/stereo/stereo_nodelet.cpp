#include "bluefox2/stereo_camera.h"

#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

namespace bluefox2 {

class StereoNodelet : public nodelet::Nodelet {
 public:
  StereoNodelet() : nodelet::Nodelet() {}
  ~StereoNodelet() { stereo_camera_->End(); }

  virtual void onInit() {
    try {
      stereo_camera_.reset(new StereoCamera(getPrivateNodeHandle()));
      stereo_camera_->Run();
    }
    catch (const std::exception &e) {
      ROS_ERROR_STREAM("Bluefox2: " << e.what());
    }
  }

 private:
  boost::shared_ptr<StereoCamera> stereo_camera_;
};

PLUGINLIB_EXPORT_CLASS(bluefox2::StereoNodelet, nodelet::Nodelet)

}  // namespace bluefox2
