// Copyright 2023 Stereolabs
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * This tutorial demonstrates simple receipt of ZED depth messages over the ROS system.
 */

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "bbox_ex_msgs/msg/BoundingBox.msg"
#include "bbox_ex_msgs/msg/BoundingBoxes.msg"
int[4] points = {0,0,0,0};
using namespace std::placeholders;

class MinimalDepthSubscriber : public rclcpp::Node
{
public:
  MinimalDepthSubscriber()
  : Node("zed_depth_tutorial")
  {
    /* Note: it is very important to use a QOS profile for the subscriber that is compatible
         * with the QOS profile of the publisher.
         * The ZED component node uses a default QoS profile with reliability set as "RELIABLE"
         * and durability set as "VOLATILE".
         * To be able to receive the subscribed topic the subscriber must use compatible
         * parameters.
         */

    // https://github.com/ros2/ros2/wiki/About-Quality-of-Service-Settings

    rclcpp::QoS depth_qos(10);
    depth_qos.keep_last(10);
    depth_qos.best_effort();
    depth_qos.durability_volatile();

    // Create depth map subscriber
    mDepthSub = create_subscription<sensor_msgs::msg::Image>(
      "/zed2i/zed_node/depth/depth_registered", depth_qos, std::bind(&MinimalDepthSubscriber::depthCallback, this, _1));
  }

protected:
  void depthCallback(const sensor_msgs::msg::Image::SharedPtr msg)
  {
    // Get a pointer to the depth values casting the data
    // pointer to floating point
    float * depths = reinterpret_cast<float *>(&msg->data[0]);

    // Image coordinates of the center pixel
    int u = msg->width / 2;
    int v = msg->height / 2;

    // Linear index of the center pixel
    int centerIdx = u + msg->width * v;

    // Output the measure
    RCLCPP_INFO(get_logger(), "Center distance : %g m", depths[centerIdx]);
  }

private:
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr mDepthSub;
};

class MinimalSubscriber : public rclcpp::Node
{
  public:
    MinimalSubscriber()
    : Node("minimal_subscriber")
    {
      subscription_ = this->create_subscription<bbox_ex_msgs::msg::BoundingBoxes>(
      "/yolov5/bounding_boxes", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));
    }// put here the message type

  private:
    void topic_callback(const bbox_ex_msgs::msg::BoundingBoxes::SharedPtr msg) const
    {
      RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

// The main function
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto depth_node = std::make_shared<MinimalDepthSubscriber>();

  rclcpp::spin(depth_node);
  rclcpp::shutdown();
  return 0;
}
