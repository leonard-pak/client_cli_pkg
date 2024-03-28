#include "client_cli_pkg/client_cli_node.hpp"

#include "common_msgs_srvs/GoToPoint.h"
#include "common_msgs_srvs/TwoWheelRobotState.h"
#include "std_msgs/Float64MultiArray.h"

#include <iostream>

namespace client_cli_pkg
{
  namespace m_s = common_msgs_srvs;
  ClientCliNode::ClientCliNode()
      : mNode("robots"), mRoboticArmClient(mNode.serviceClient<m_s::GoToPoint>(
                             "arm/go_to_point")),
        mTwoWheelRobotClient(
            mNode.serviceClient<m_s::GoToPoint>("two_wheel/go_to_point")),
        mRoboticArmStateSub(mNode.subscribe<std_msgs::Float64MultiArray>(
            "arm/state", 1000,
            [this](std_msgs::Float64MultiArray::ConstPtr const &msg)
            {
              std::lock_guard lock(mRoboticArmState.mut);
              mRoboticArmState.data.jointAngles = msg->data;
            })),
        mTwoWheelRobotSub(mNode.subscribe<m_s::TwoWheelRobotState>(
            "two_wheel/state", 1000,
            [this](m_s::TwoWheelRobotState::ConstPtr const &msg)
            {
              std::lock_guard lock(mTwoWheelsRobotState.mut);
              mTwoWheelsRobotState.data.leftWheelVelocity =
                  msg->leftWheelVelocity;
              mTwoWheelsRobotState.data.rightWheelVelocity =
                  msg->rightWheelVelocity;
            }))
  {
  }

  bool ClientCliNode::GoToPoint(lib::dto::GoToPointMsg msg)
  {
    m_s::GoToPoint srvMsg;
    srvMsg.request.target.x = msg.x;
    srvMsg.request.target.y = msg.y;
    srvMsg.request.target.z = msg.z;

    switch (msg.type)
    {
    case lib::dto::RobotType::ROBOTIC_ARM:
      return mRoboticArmClient.call(srvMsg) && srvMsg.response.success;
      break;

    case lib::dto::RobotType::TWO_WHEEL_ROBOT:
      return mTwoWheelRobotClient.call(srvMsg) && srvMsg.response.success;
      break;

    default:
      return false;
    }
  }

  lib::dto::RoboticArmState ClientCliNode::GetRoboticArmState() const
  {
    std::lock_guard lock(mRoboticArmState.mut);
    return mRoboticArmState.data;
  }

  lib::dto::TwoWheelsRobotState ClientCliNode::GetTwoWheelRobotState() const
  {
    std::lock_guard lock(mTwoWheelsRobotState.mut);
    return mTwoWheelsRobotState.data;
  }

  void ClientCliNode::Run()
  {
    ros::AsyncSpinner spinner(1);
    spinner.start();
    int a;
    while (std::cin >> a)
    {
      std::cout << "Res: "
                << GoToPoint({.type = static_cast<lib::dto::RobotType>(a),
                              .x = 1,
                              .y = 2,
                              .z = 3})
                << std::endl;
    };
  }
} // namespace client_cli_pkg

int main(int argc, char **argv)
{
  ros::init(argc, argv, "client");
  client_cli_pkg::ClientCliNode clientNode;
  clientNode.Run();
  return 0;
}
