#ifndef CLIENT_CLI_PKG_CLIENT_CLI_NODE_HPP
#define CLIENT_CLI_PKG_CLIENT_CLI_NODE_HPP

#include "client_cli_lib/interface/i-client-api.hpp"

#include "ros/ros.h"

#include <thread>

namespace client_cli_pkg
{
  namespace lib = client_cli_lib;
  class ClientCliNode : public lib::interface::IClientApi
  {
    template <typename T> struct DataMutex
    {
      mutable std::mutex mut;
      T data;
    };

  private:
    ros::NodeHandle mNode;

    ros::ServiceClient mRoboticArmClient;
    ros::ServiceClient mTwoWheelRobotClient;

    ros::Subscriber mRoboticArmStateSub;
    ros::Subscriber mTwoWheelRobotSub;

    DataMutex<lib::dto::RoboticArmState> mRoboticArmState;
    DataMutex<lib::dto::TwoWheelsRobotState> mTwoWheelsRobotState;

  public:
    ClientCliNode();
    ~ClientCliNode() = default;

    void Run();

    bool GoToPoint(lib::dto::GoToPointMsg) override;
    lib::dto::RoboticArmState GetRoboticArmState() const override;
    lib::dto::TwoWheelsRobotState GetTwoWheelRobotState() const override;
  };

} // namespace client_cli_pkg

#endif // CLIENT_CLI_PKG_CLIENT_CLI_NODE_HPP
