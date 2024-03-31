#ifndef CLIENT_CLI_LIB_CLI_HPP
#define CLIENT_CLI_LIB_CLI_HPP

#include <client_cli_lib/dto/cmd-type.hpp>
#include <client_cli_lib/dto/robot-type.hpp>
#include <client_cli_lib/interface/i-client-api.hpp>

#include <memory>
#include <sstream>

namespace client_cli_lib
{

  class CLI
  {
    enum class CLIState
    {
      GET_ROBOT_TYPE,
      GET_CMD_TYPE,
      CALL_GO_TO_POINT,
      CALL_ROBOT_STATE,
      EXIT,
    };

    struct Context
    {
      dto::CmdType cmdType;
      dto::RobotType robotType;
      double x;
      double y;
      double z;
    };

  private:
    std::unique_ptr<interface::IClientApi> mClient;
    CLIState mState;
    Context mContextCLI;

    std::string CallGoToPoint();
    std::string CallGetState();

    bool SaveRobotType(std::istream &in);
    bool SaveCmdType(std::istream &in);
    bool SavePoint(std::istream &in);

  public:
    CLI(std::unique_ptr<interface::IClientApi> client);
    ~CLI() = default;

    void Run(std::istream &in, std::ostream &out);
  };

} // namespace client_cli_lib

#endif // CLIENT_CLI_LIB_CLI_HPP
