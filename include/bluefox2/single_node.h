#ifndef BLUEFOX2_SINGLE_NODE_H_
#define BLUEFOX2_SINGLE_NODE_H_

#include "bluefox2/Bluefox2DynConfig.h"
#include "bluefox2/exposure.h"
#include <camera_base/camera_node_base.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

namespace bluefox2 {

class Bluefox2Ros;

class SingleNode : public camera_base::CameraNodeBase<Bluefox2DynConfig> {
 public:
  explicit SingleNode(const ros::NodeHandle &pnh);

  virtual void Acquire() override;
  virtual void Setup(Bluefox2DynConfig &config) override;

  void AcquireOnce();
  void callbackAec(const std_msgs::Int32::ConstPtr &expose_us);
  void callbackAgc(const std_msgs::Float32::ConstPtr &gain_db);
  
  void PublishExposure(int expose_value, const ros::Time& time, std::string camera_name, int fps_rate){
  exposure exposure;
  exposure.exposure_value = expose_value;
  exposure.header.stamp = time;
  exposure.header.frame_id = camera_name;
  exposure.header.seq = 0;
  exposure.fps = fps_rate;

  expose_pub.publish(exposure);
  }

 private:
  boost::shared_ptr<Bluefox2Ros> bluefox2_ros_;
  bool boost_{false};
  ros::Subscriber sub_expose_us;
  ros::Subscriber sub_gain_db;
  ros::NodeHandle nh;
  
  ros::Publisher expose_pub;
  bool initialized = false;
};

}  // namespace bluefox2

#endif  // BLUEFOX2_SINGLE_NODE_H_
