#ifndef CLIENT_CLI_LIB_CLI_HPP
#define CLIENT_CLI_LIB_CLI_HPP

#include <client_cli_lib/dto/robot-type.hpp>
#include <client_cli_lib/interface/i-client-api.hpp>

#include <memory>
#include <sstream>

namespace client_cli_lib
{
  class CLI
  {
  private:
    std::unique_ptr<interface::IClientApi> mClient;

    std::string CallGoToPoint(dto::RobotType type, double x, double y,
                              double z);
    std::string CallGetState(dto::RobotType type);

  public:
    CLI(std::unique_ptr<interface::IClientApi> client);
    ~CLI() = default;

    void Run(std::istream &in, std::ostream &out);
  };

} // namespace client_cli_lib

#endif // CLIENT_CLI_LIB_CLI_HPP
