<!--
 * @Author: Cao Ning, Zeng XiangYu
 * @Date: 2023-04-20 13:41:56
 * @FilePath: README
 * @Assignment: Practicum II
 * @Course: CS5600 Spring 2023
-->
# Practicum II

TCP Socket Server with Multi-threading and File Reading Capability

This project implements a multi-threaded TCP socket server that allows clients to send requests for file operations on two USB devices. The server can handle multiple clients simultaneously and supports file operations such as getting, putting, and removing files, as well as creating directories and retrieving file information.

# Group Members

Ning Cao, Xiangyu Zeng

# Features

- Multi-threaded server capable of handling multiple clients simultaneously
- File operations on two USB devices:
- GET: Read and transfer a file to the client
- PUT: Write a file from the client to both USB devices
- RM: Delete a file or directory
- MD: Create a new directory
- INFO: Retrieve file information (owner, group, size, last modified, permissions)
- PARAGET: Read requests for files can be distributed across USB devices and can read from two devices in parallel.

# File description

- server.h: The header file that contains the function declarations and the struct definitions for the server.
- server.c: The main server file that contains the main function and the server's main loop.
- client_util.h: The header file that contains the function declarations and the struct definitions for the client.
- client_util.c: The client utility file that contains the client's main function dealing with the client's requests.
- client.c: The client file that contains the client's main function.
- test.c: The test file that contains the test cases for GET, INFO, MD, PUT, RM command.
- multi_client_test.c: The test file that contains the test cases for multiple clients (Q7 test).
- config.txt: The configuration file that contains the paths to the two USB devices.
- Makefile: The makefile that contains the commands for compiling the program.

# Test Guide

run `export PATH=$PATH:/home/ning/Workspace/HW12/Practicum2 to ~/.bashrc` for use of fget command first.

- run test.c for GET, INFO, MD, PUT, RM command.
- run multi_client_test.c for multiple clients (Q7 test).
- run two command line in two window, one for big file, another for small file. (Q8 test)
  - `fget PARAGET server/InitialD.mp4`
  - `fget PARAGET server/server.txt`

# Compilation

To compile the program, open a terminal and navigate to the directory containing the source files. Then, run the following command:

`make`

To clean the class files, run the following command:

`make clean`

To run the program for test, execute the following command:

`./test`
`./multi_client_test`
and two command

# Acknowledgments

I would like to thank my TA and professor for their guidance and support throughout the development of this project. Thanks for your effort creating this homework and taking time to grade it.

License

This project is licensed under the MIT License - see the LICENSE.md file for details.
