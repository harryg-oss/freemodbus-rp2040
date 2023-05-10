FreeMODBUS implementation with the RP2040 in mind. The project folder
structure this was designed for is as follows:

my_project
 -> CMakeLists.txt
 -> pico_sdk_import.cmake
 -> main.c
 -> ...
 -> freemodbus_pinmap
    -> pinmap.h
 -> libs
    -> pico_sdk
    -> freemodbus-rp2040
 -> build

An example CMakeLists.txt is provided (project_CMakeLists.txt) based on
the CMakeLists.txt provided by the Raspberry Pico SDK.
