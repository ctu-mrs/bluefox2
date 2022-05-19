#include "bluefox2/single_node.h"
#include "std_msgs/Int32.h"

ros::Publisher expose_pub;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "bluefox2_single");
  ros::NodeHandle pnh("~");
  ros::Publisher expose_pub =
      pnh.advertise<std_msgs::Int32>("chatter", 1000, true);

  try {
    bluefox2::SingleNode single_node(pnh);
    single_node.Run();
    ros::spin();
    single_node.End();
  } catch (const std::exception &e) {
    ROS_ERROR("%s: %s", pnh.getNamespace().c_str(), e.what());
  }
}
