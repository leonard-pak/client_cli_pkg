#include "client_cli_lib/cli.hpp"

#include <ranges>

namespace client_cli_lib
{
  namespace vw = std::ranges::views;

  CLI::CLI(std::unique_ptr<interface::IClientApi> client)
      : mClient(std::move(client))
  {
  }

  std::string CLI::CallGoToPoint()
  {
    return mClient->GoToPoint({.type = mContextCLI.robotType,
                               .x = mContextCLI.x,
                               .y = mContextCLI.y,
                               .z = mContextCLI.z})
               ? "success"
               : "fail";
  }

  std::string CLI::CallGetState()
  {
    std::string res;
    switch (mContextCLI.robotType)
    {
    case dto::RobotType::ROBOTIC_ARM:
    {
      res += "Robotic arm joints position: ";
      dto::RoboticArmState state = mClient->GetRoboticArmState();
      if (state.jointAngles.empty())
      {
        res += "no data";
      }
      else
      {
        for (auto &&i : vw::iota(0u, state.jointAngles.size()))
        {
          res += std::to_string(i) + " - " +
                 std::to_string(state.jointAngles[i]) + ' ';
        }
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

  bool CLI::SaveRobotType(std::istream &in)
  {
    int input;
    if (in >> input; not in.good())
    {
      return false;
    }

    switch (input)
    {
    case 1:
      mContextCLI.robotType = dto::RobotType::ROBOTIC_ARM;
      break;
    case 2:
      mContextCLI.robotType = dto::RobotType::TWO_WHEEL_ROBOT;
      break;
    default:
      return false;
    }

    return true;
  }

  bool CLI::SaveCmdType(std::istream &in)
  {
    int input;

    if (in >> input; not in.good())
    {
      return false;
    }

    switch (input)
    {
    case 1:
      mContextCLI.cmdType = dto::CmdType::GET_STATE;
      break;
    case 2:
      mContextCLI.cmdType = dto::CmdType::GO_TO_POINT;
      break;
    default:
      return false;
    }

    return true;
  }

  bool CLI::SavePoint(std::istream &in)
  {
    int x, y, z;

    if (in >> x >> y >> z; not in.good())
    {
      return false;
    }

    mContextCLI.x = x;
    mContextCLI.y = y;
    mContextCLI.z = z;

    return true;
  }

  void CLI::Run(std::istream &in, std::ostream &out)
  {
    out << "Command: a b x y z\n"
        << "a - type robot (1 - robotic arm, 2 - two wheel robot)\n"
        << "b - type command (1 - get state, 2 - go to point)\n"
        << "x y z - only for go to point cmd, three float nums\n"
        << "For exit any non numeric char or invalid num for cmd\n";
    mState = CLIState::GET_ROBOT_TYPE;
    while (mState != CLIState::EXIT)
    {
      switch (mState)
      {
      case CLIState::GET_ROBOT_TYPE:
        out << "Print next cmd: " << std::endl;
        mState = SaveRobotType(in) ? CLIState::GET_CMD_TYPE : CLIState::EXIT;
        break;
      case CLIState::GET_CMD_TYPE:
        if (SaveCmdType(in))
        {
          switch (mContextCLI.cmdType)
          {
          case dto::CmdType::GET_STATE:
            out << CallGetState() << std::endl;
            mState = CLIState::GET_ROBOT_TYPE;
            break;
          case dto::CmdType::GO_TO_POINT:
            mState = CLIState::CALL_GO_TO_POINT;
            break;
          default:
            mState = CLIState::EXIT;
            break;
          }
        }
        else
        {
          mState = CLIState::EXIT;
        }
        break;
      case CLIState::CALL_GO_TO_POINT:
        if (SavePoint(in))
        {
          out << CallGoToPoint() << std::endl;
          mState = CLIState::GET_ROBOT_TYPE;
        }
        else
        {
          mState = CLIState::EXIT;
        }
        break;
      default:
        mState = CLIState::EXIT;
        break;
      }
    }
    out << "EXIT" << std::endl;
  }
} // namespace client_cli_lib
