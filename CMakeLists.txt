# Top-level project file for the ESP32-C3 multi-sensor monitor (Level 1).
cmake_minimum_required(VERSION 3.16)

# Firmware version embedded in the app descriptor and printed at boot.
set(PROJECT_VER "0.1.0")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32c3_multisensor_monitor)
