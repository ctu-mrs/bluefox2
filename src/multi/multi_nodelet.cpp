#include "bluefox2/multi_node.h"

#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>

namespace bluefox2 {

class MultiNodelet : public nodelet::Nodelet {
 public:
  MultiNodelet() = default;
  ~MultiNodelet() {
    if (multi_node_) {
      multi_node_->End();
    }
  }

  virtual void onInit() {
    try {
      multi_node_.reset(new MultiNode(getPrivateNodeHandle()));
      multi_node_->Run();
    } catch (const std::exception &e) {
      NODELET_ERROR("%s: %s", getPrivateNodeHandle().getNamespace().c_str(),
                    e.what());
    }
  }

 private:
  std::unique_ptr<MultiNode> multi_node_;
};

PLUGINLIB_EXPORT_CLASS(bluefox2::MultiNodelet, nodelet::Nodelet)

}  // namespace bluefox2
