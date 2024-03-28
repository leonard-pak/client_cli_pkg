#ifndef CLIENT_CLI_LIB_DTO_STATE_HPP
#define CLIENT_CLI_LIB_DTO_STATE_HPP

#include <vector>

namespace client_cli_lib::dto
{
  struct RoboticArmState
  {
    std::vector<double> jointAngles;
  };

  struct TwoWheelsRobotState
  {
    double leftWheelVelocity;
    double rightWheelVelocity;
  };

} // namespace client_cli_lib::dto

#endif // CLIENT_CLI_LIB_DTO_STATE_HPP
