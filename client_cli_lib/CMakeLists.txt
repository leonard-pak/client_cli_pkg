cmake_minimum_required(VERSION  3.0.2)
project(client_cli_lib)

add_library(${PROJECT_NAME} STATIC
  src/cli.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)
