# VLP16 LIDAR data reader + point cloud generator

## Techincal info
- Data from the LIDAR is sent as packets trough ethernet
- Parsing of the packet data is implemented in c++ with the help of WinPcap API
- Point cloud is represented in python with the help of the Open3d library
- C++ program is exposed to python with the help of PyBind11 library

## Installation guide for Visual Studio (VS) on Windows

###	Installing Pcap
- Download and install WinPcap from https://www.winpcap.org/install/default.htm
- Download and extract WinPcap SDK from https://www.winpcap.org/devel.htm
- Set Pcap libraries up for c++
  - In VS, you can follow this video from 2:25-6:00 if you need help: https://youtu.be/YpnrR7D_lRI?t=145

### Installing and using Pybind11
- Install Pybind11 ("pip install pybind11", or manage python packages in VS)
- Configure c++ to output a shared library, and reference it for python
  - In VS, you can follow this tutorial https://docs.microsoft.com/hu-hu/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2019

### Installing Open3d
- "pip install open3d", or manage python packages in VS
