/*
This header file is based on nolo_api.h in NOLO Windows SDK.
https://github.com/NOLOVR/NOLO-Windows-SDK/blob/master/NOLOVR/NOLO_USBHID_SDK/include/nolo_api.h

The function definitions are rewritten so that they fit with the latest noloRuntime.dll distributed in part of NOLO Unity SDK.
https://github.com/NOLOVR/NOLO-Unity-SDK/tree/master/NOLOVR/Assets/Plugins/x86_64
*/

#pragma once
#ifndef _NOLO_V_0_1_RC_API_H_
#define _NOLO_V_0_1_RC_API_H_

#define WIN32_LEAN_AND_MEAN    
#include <windows.h>

#ifndef NOLO_API
#define NOLO_API extern "C"
#else
#define NOLO_API
#endif
namespace NOLO {
	struct Vector2 {
		float x;
		float y;
	};
	struct Vector3 {
		float x;
		float y;
		float z;
	};
	struct Quaternion {
		float x;
		float y;
		float z;
		float w;
	};
	struct Pose {
		Vector3 pos;
		Quaternion rot;
		Vector3 vecVelocity;
		Vector3 vecAngularVelocity;
	};

	struct ControllerStates {
		uint32_t buttons;
		uint32_t touches;
		Vector2 touchpadAxis;
	};

	enum EBattery {
		ShutDown = 0,
		Low,
		Middle,
		High,
	};

	enum EDoubleClickKeyType {
		press = 0,
		trigger,
		menu,
		grip,
		cancel
	};

	//Struct of NOLO controller data 
	struct Controller {
		Vector3 ControllerPosition;
		Quaternion ControllerRotation;
		/*
		  0x0000_XXX1:press
		  0x0000_XX1X:trigger
		  0x0000_X1XX:menu
		  0x0000_1XXX:system
		  0x0001_XXXX:grip
		*/
		UINT Buttons;
		/*
		  0~1 Touchpad  touched:1; Not touched:0
		*/
		int ControllerTouched;
		/*
		   x:-1~1
		   y:-1~1
		*/
		Vector2 ControllerTouchAxis;
		/*
		  0~100:normal
		  255:Shut down
		*/
		EBattery ControllerBattery;
		/* Velocity of the pose in meters/second */
		Vector3 vecVelocity;

		/* Acceleration of the pose in meters/second */
		Vector3 vecAcceleration;

		/* Angular velocity of the pose in axis-angle
		 * representation. The direction is the angle of
		 * rotation and the magnitude is the angle around
		 * that axis in radians/second. */
		Vector3 vecAngularVelocity;

		/* Angular acceleration of the pose in axis-angle
		 * representation. The direction is the angle of
		 * rotation and the magnitude is the angle around
		 * that axis in radians/second^2. */
		Vector3 vecAngularAcceleration;
		/*
		  state & 1 ==1 :normal:state & 1 ==0: Be blocked
	   */
		int state;
	};
	//Struct of NOLO headset marker data
	struct HMD {
		Vector3 HMDPosition;
		/*
		  When pressing the pair button on the headset marker,
		  we record the position data of the headset to calibrate the height of the ground.
		  Open vr don't need this data.
		*/
		Vector3 HMDInitPosition;

		Quaternion HMDRotation;
		/* Velocity of the pose in meters/second */
		Vector3 vecVelocity;

		/* Acceleration of the pose in meters/second */
		Vector3 vecAcceleration;

		/* Angular velocity of the pose in axis-angle
		* representation. The direction is the angle of
		* rotation and the magnitude is the angle around
		* that axis in radians/second. */
		Vector3 vecAngularVelocity;

		/* Angular acceleration of the pose in axis-angle
		* representation. The direction is the angle of
		* rotation and the magnitude is the angle around
		* that axis in radians/second^2. */
		Vector3 vecAngularAcceleration;
		/*
		   state & 1 ==1 :normal:state & 1 ==0: Be blocked
		*/
		int state;
	};
	//Struct of NOLO base station data
	struct BaseStation {
		Vector3  vecPosition;
		/*
		  0~100:normal
		*/
		EBattery BaseStationPower;
	};
	//Struct of all NOLO devices data
	// TODO: probably different in the latest firmware
	struct NoloData {
		Controller left_Controller_Data;
		Controller right_Controller_Data;
		HMD hmdData;
		BaseStation baseStationData;
	};
	// Enumerarion of NOLO devices types
	enum NoloDeviceType {
		HmdDevice = 0,
		LeftControllerDevice,
		RightControllerDevice,
		BaseStationDevice
	};

	enum NoloButtonID {
		TouchPad = 0,
		Trigger,
		Menu,
		System,
		Grip,
		TouchPadUp = 7,
		TouchPadDown,
		TouchPadLeft,
		TouchPadRight,
	};

	enum NoloError {
		None = 0,
		ConnectionFailed,
		NoConnection,
		Disconnected,
		Unknown,
	};

	enum NoloTrackingStatus {
		NotConnected = 0,
		Normal,
		OutOfRange
	};

	enum ExpandMsgType {
		//180 degree turn-around
		DoubleClickKeyTurnAround = 1,
		//recenter
		DoubleClickSystemRecenter
	};

	enum EPlayMode {
		//Place the base station horizontally
		HorizontalMode = 0,
		//Place the base station on the ceiling
		CeilingMode
	};

	typedef void(__cdecl *noloDataNotifyCallBack)(NoloData _noloData, void * context);
	typedef void(__cdecl *funcCallBack)(void * context);
	typedef void(__cdecl *expandMsgCallBack)(ExpandMsgType expandMsgType, void * context);

	NOLO_API void _cdecl open_Nolo_ZeroMQ();
	NOLO_API void _cdecl close_Nolo_ZeroMQ();
	NOLO_API NoloData _cdecl get_Nolo_NoloData();
	NOLO_API EBattery _cdecl get_Nolo_Battery(NoloDeviceType deviceType);
	NOLO_API Controller _cdecl get_Nolo_LeftControllerData();
	NOLO_API Controller _cdecl get_Nolo_RightControllerData();
	NOLO_API HMD _cdecl get_Nolo_HMDData();
	NOLO_API Pose _cdecl get_Nolo_Pose(int type);
	NOLO_API ControllerStates _cdecl get_Nolo_ControllerStates(int type);
	NOLO_API bool _cdecl set_Nolo_TriggerHapticPulse(NoloDeviceType deviceType, int intensity);
	NOLO_API bool _cdecl registerDisConnectCallBack(funcCallBack fun, void * context);
	NOLO_API bool _cdecl registerConnectSuccessCallBack(funcCallBack fun, void * context);
	NOLO_API bool _cdecl registerExpandDataNotifyCallBack(expandMsgCallBack fun, void * context);
	NOLO_API bool _cdecl registerNoloDataNotifyCallBack(noloDataNotifyCallBack fun, void * context);
}
#endif // _NOLO_V_0_1_RC_API_H_