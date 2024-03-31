# client_cli_pkg
Package for passing commands from the command line to the server

## Launch

- `start-client.launch` - start client

## Nodes

- `client_cli_node` - client backend implementation for linking cli and ros

## Client cli lib

Library contains interface of client backend and data-transfer objects. Also here implement the cli class.
```
Command: a b x y z
    a - type robot (1 - robotic arm, 2 - two wheel robot)
    b - type command (1 - get state, 2 - go to point)
    x y z - only for go to point cmd, three float nums
    For exit any non numeric char or invalid num for cmd
Examples:
    1 1 - get state of robotic arm
    2 1 - get state of two wheel robot
    1 2 5 3 4 - go to point (5 3 4) by robotic arm
    2 2 100 50 0 - go to point (100 50 0) by two wheel robot
```
