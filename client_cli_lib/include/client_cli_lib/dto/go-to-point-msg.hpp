#ifndef CLIENT_CLI_LIB_DTO_GO_TO_POINT_MSG_HPP
#define CLIENT_CLI_LIB_DTO_GO_TO_POINT_MSG_HPP

#include "client_cli_lib/dto/robot-type.hpp"

namespace client_cli_lib::dto
{
  struct GoToPointMsg
  {
    RobotType type;
    double x;
    double y;
    double z;
  };
} // namespace client_cli_lib::dto

#endif // CLIENT_CLI_LIB_DTO_GO_TO_POINT_MSG_HPP
