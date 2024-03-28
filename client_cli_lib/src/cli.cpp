#include "client_cli_lib/cli.hpp"

#include <ranges>

namespace client_cli_lib
{
  namespace vw = std::ranges::views;

  CLI::CLI(std::unique_ptr<interface::IClientApi> client)
      : mClient(std::move(client))
  {
  }
  std::string CLI::CallGoToPoint(dto::RobotType type, double x, double y,
                                 double z)
  {
    return mClient->GoToPoint({.type = type, .x = x, .y = y, .z = z})
               ? "success"
               : "fail";
  }

  std::string CLI::CallGetState(dto::RobotType type)
  {
    std::string res;
    switch (type)
    {
    case dto::RobotType::ROBOTIC_ARM:
    {
      res += "Robotic arm joints position: ";
      dto::RoboticArmState state = mClient->GetRoboticArmState();
      for (auto &&i : vw::iota(0u, state.jointAngles.size()))
      {
        res += std::to_string(i) + " - " +
               std::to_string(state.jointAngles[i]) + ' ';
      }
      break;
    }
    case dto::RobotType::TWO_WHEEL_ROBOT:
    {
      res += "Two wheel robot velocity: ";
      dto::TwoWheelsRobotState state = mClient->GetTwoWheelRobotState();
      res += "leftVel - " + std::to_string(state.leftWheelVelocity) +
             " rightVel - " + std::to_string(state.rightWheelVelocity);
      break;
    }
    default:
      break;
    }
    return res;
  }

  void CLI::Run(std::istream &in, std::ostream &out)
  {
    out << "Command: a b x y z\n"
        << "a - type robot (1 - robotic arm, 2 - two wheel robot)\n"
        << "b - type command (1 - get state, 2 - go to point)\n"
        << "x y z - only for go to point cmd, three float nums\n"
        << "For exit any non numeric char or invalid num for cmd\n";

    int a, b;
    double x, y, z;
    while (in >> a >> b)
    {
      dto::RobotType type;
      switch (a)
      {
      case 1:
        type = dto::RobotType::ROBOTIC_ARM;
        break;
      case 2:
        type = dto::RobotType::TWO_WHEEL_ROBOT;
        break;
      default:
        return;
      }
      switch (b)
      {
      case 1:
        out << CallGetState(type) << std::endl;
        break;
      case 2:
        if (not(in >> x >> y >> z))
        {
          return;
        }
        out << CallGoToPoint(type, x, y, z) << std::endl;
        break;
      default:
        return;
      }
    }
  }
} // namespace client_cli_lib
