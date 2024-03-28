#ifndef CLIENT_CLI_LIB_INTERFACE_I_CLIENT_API_HPP
#define CLIENT_CLI_LIB_INTERFACE_I_CLIENT_API_HPP

#include "client_cli_lib/dto/go-to-point-msg.hpp"
#include "client_cli_lib/dto/state.hpp"

namespace client_cli_lib::interface
{
  class IClientApi
  {
  public:
    virtual ~IClientApi() = default;
    virtual bool GoToPoint(dto::GoToPointMsg const &) = 0;
    virtual dto::RoboticArmState GetRoboticArmState() const = 0;
    virtual dto::TwoWheelsRobotState GetTwoWheelRobotState() const = 0;
  };
} // namespace client_cli_lib::interface

#endif // CLIENT_CLI_LIB_INTERFACE_I_CLIENT_API_HPP
