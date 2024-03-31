#include "client_cli_pkg/client_cli_node.hpp"

#include "common_msgs_srvs/GoToPoint.h"
#include "common_msgs_srvs/TwoWheelRobotState.h"
#include "std_msgs/Float64MultiArray.h"

#include "client_cli_lib/cli.hpp"

#include <csignal>
#include <functional>
#include <iostream>

namespace client_cli_pkg
{
  namespace m_s = common_msgs_srvs;
  ClientCliNode::ClientCliNode(std::string const &armGoToPointService,
                               std::string const &twoWheelGoToPointService,
                               std::string const &armStateTopic,
                               std::string const &twoWheelStateTopic)
      : mRoboticArmClient(
            mNode.serviceClient<m_s::GoToPoint>(armGoToPointService)),
        mTwoWheelRobotClient(
            mNode.serviceClient<m_s::GoToPoint>(twoWheelGoToPointService)),
        mRoboticArmStateSub(mNode.subscribe<std_msgs::Float64MultiArray>(
            armStateTopic, 1000,
            [this](std_msgs::Float64MultiArray::ConstPtr const &msg)
            {
              std::lock_guard lock(mRoboticArmState.mut);
              mRoboticArmState.data.jointAngles = msg->data;
            })),
        mTwoWheelRobotSub(mNode.subscribe<m_s::TwoWheelRobotState>(
            twoWheelStateTopic, 1000,
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

  bool ClientCliNode::GoToPoint(lib::dto::GoToPointMsg const &msg)
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

} // namespace client_cli_pkg

int main(int argc, char **argv)
{
  ros::init(argc, argv, "client", ros::init_options::NoSigintHandler);

  std::unique_ptr<client_cli_lib::interface::IClientApi> client =
      std::make_unique<client_cli_pkg::ClientCliNode>(
          ros::param::param<std::string>("robotic_arm_go_to_point_service",
                                         "robotic_arm/go_to_point"),
          ros::param::param<std::string>("two_wheel_robot_go_to_point_service",
                                         "two_wheel_robot/go_to_point"),
          ros::param::param<std::string>("robotic_arm_state_topic",
                                         "robotic_arm/state"),
          ros::param::param<std::string>("two_wheel_robot_state_topic",
                                         "two_wheel_robot/state"));
  client_cli_lib::CLI cli(std::move(client));

  ros::AsyncSpinner spinner(1);
  spinner.start();

  std::signal(SIGINT,
              [](int signal)
              {
                ros::shutdown();
                exit(signal);
              });
  cli.Run(std::cin, std::cout);
  return 0;
}
