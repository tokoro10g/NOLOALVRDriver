# NOLO CV1 Driver for ALVR

[日本語](README-ja.md)

This application enables injection of 6-DoF tracking information from NOLO CV1 to ALVR server.
The implementation exploits ALVR-FreePIE integration (without using FreePIE).

## Installation

1. Install [NOLO Assistant](http://download.nolovr.com/download/noloassistant.html) and [ALVR](https://github.com/polygraphene/ALVR)
1. Clone and build this project or download the prebuilt binary from [here](https://github.com/tokoro10g/NOLOALVRDriver/releases)

## Usage

1. Turn on the base station and controllers
1. Plug in the headset marker's USB cable **to your computer**  
   *standalone 6-DoF tracking is currently not supported*
1. Launch NOLO Assistant
1. Execute NOLOALVRDriver.exe
1. Enjoy!

## Note

* Standalone tracking might be possible by using [VirtualHere USB](https://www.virtualhere.com/home)
* This work is inspired by [Oculus GoとALVRとNOLO-CV1でSteam VR Gameを遊ぶ](https://qiita.com/ahinore@github/items/a4a8e675eb89b41921a0)
* Files in `lib` folder are taken and generated from [NOLO Unity SDK](https://github.com/NOLOVR/NOLO-Unity-SDK)