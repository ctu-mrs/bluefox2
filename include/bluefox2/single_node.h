#ifndef BLUEFOX2_SINGLE_NODE_H_
#define BLUEFOX2_SINGLE_NODE_H_

#include "bluefox2/bluefox2_ros.h"
#include "bluefox2/Bluefox2DynConfig.h"
#include "camera_base/camera_node_base.h"

namespace bluefox2 {

class SingleNode : public CameraNodeBase<Bluefox2DynConfig> {

 public:
  SingleNode(const ros::NodeHandle &nh)
      : CameraNodeBase{nh}, bluefox2_ros_{nh} {}

  virtual void Acquire() override;
  virtual void Setup(Bluefox2DynConfig &config) override;

 private:
  Bluefox2Ros bluefox2_ros_;
};

}  // namespace bluefox2

#endif  // BLUEFOX2_SINGLE_NODE_H_
