#pragma once

#include <cstdint>
#include "ipctools.h"
#include "nolo_api.h"
#include "mathhelper.h"

class ALVRInjector
{
public:
	static const uint32_t ALVR_FREEPIE_SIGNATURE_V2 = 0x11223345;

	static const uint32_t ALVR_FREEPIE_FLAG_OVERRIDE_HEAD_ORIENTATION = 1 << 0;
	static const uint32_t ALVR_FREEPIE_FLAG_OVERRIDE_CONTROLLER_ORIENTATION0 = 1 << 1;
	static const uint32_t ALVR_FREEPIE_FLAG_OVERRIDE_HEAD_POSITION = 1 << 2;
	static const uint32_t ALVR_FREEPIE_FLAG_OVERRIDE_CONTROLLER_POSITION0 = 1 << 3;
	static const uint32_t ALVR_FREEPIE_FLAG_OVERRIDE_BUTTONS = 1 << 4;

	static const uint32_t ALVR_FREEPIE_MESSAGE_LENGTH = 512;

#pragma pack(push, 1)
	struct FreePIEFileMapping {
		uint32_t version;
		uint32_t flags;
		double input_head_orientation[3];
		double input_controller_orientation[3];
		double input_head_position[3];
		double input_controller_position[3];
		double input_trackpad[2];
		uint16_t inputControllerButtons;
		uint16_t controllers;
		uint32_t controllerButtons[2];
		double head_orientation[3];
		double controller_orientation[2][3];
		double head_position[3];
		double controller_position[2][3];
		double trigger[2];
		double trigger_left[2];
		double trigger_right[2];
		double joystick_left[2][2];
		double joystick_right[2][2];
		double trackpad[2][2];
		char message[ALVR_FREEPIE_MESSAGE_LENGTH];
	};
#pragma pack(pop)

	ALVRInjector()
		: m_fileMapping("ALVR_FREEPIE_FILEMAPPING_13B65572-591A-4248-A2F6-BAC2D89EE3B8", sizeof(FreePIEFileMapping))
		, m_mutex("ALVR_FREEPIE_MUTEX_AA77F1C3-86E4-4EF9-AAA2-5C40CF380D7A")
	{
		Initialize();
	};
	~ALVRInjector() {};

	void UpdateTrackingInfo(NOLO::NoloData data) {
		m_mutex.Wait();

		m_p->flags = ALVR_FREEPIE_FLAG_OVERRIDE_HEAD_POSITION
			| ALVR_FREEPIE_FLAG_OVERRIDE_CONTROLLER_POSITION0
			| ALVR_FREEPIE_FLAG_OVERRIDE_CONTROLLER_ORIENTATION0
			| ALVR_FREEPIE_FLAG_OVERRIDE_BUTTONS;
		m_p->controllers = 2;

		NOLO::Pose hmd = NOLO::get_Nolo_Pose(0);
		NOLO::Vector3 head_euler = QuaternionToEulerAngles(hmd.rot);
		m_p->head_position[0] = hmd.pos.x;
		m_p->head_position[1] = hmd.pos.y;
		m_p->head_position[2] = -hmd.pos.z;
		m_p->head_orientation[0] = head_euler.x;
		m_p->head_orientation[1] = head_euler.y;
		m_p->head_orientation[2] = head_euler.z;

		NOLO::Pose right = NOLO::get_Nolo_Pose(2);
		NOLO::Vector3 right_euler = QuaternionToEulerAngles(right.rot);
		m_p->controller_position[0][0] = right.pos.x;
		m_p->controller_position[0][1] = right.pos.y;
		m_p->controller_position[0][2] = -right.pos.z;
		m_p->controller_orientation[0][0] = right_euler.x;
		m_p->controller_orientation[0][1] = right_euler.y;
		m_p->controller_orientation[0][2] = right_euler.z;

		NOLO::Pose left = NOLO::get_Nolo_Pose(1);
		NOLO::Vector3 left_euler = QuaternionToEulerAngles(left.rot);
		m_p->controller_position[1][0] = left.pos.x;
		m_p->controller_position[1][1] = left.pos.y;
		m_p->controller_position[1][2] = -left.pos.z;
		m_p->controller_orientation[1][0] = left_euler.x;
		m_p->controller_orientation[1][1] = left_euler.y;
		m_p->controller_orientation[1][2] = left_euler.z;

		NOLO::ControllerStates rightstate = NOLO::get_Nolo_ControllerStates(2);
		NOLO::ControllerStates leftstate = NOLO::get_Nolo_ControllerStates(1);
		m_p->controllerButtons[0] = ConvertButtonMappings(rightstate);
		m_p->controllerButtons[1] = ConvertButtonMappings(leftstate);

		m_p->trackpad[0][0] = rightstate.touchpadAxis.x;
		m_p->trackpad[0][1] = rightstate.touchpadAxis.y;
		m_p->trackpad[1][0] = leftstate.touchpadAxis.x;
		m_p->trackpad[1][1] = leftstate.touchpadAxis.y;

		m_p->trigger[0] = rightstate.buttons & 0x02;
		m_p->trigger_right[0] = rightstate.buttons & 0x02;
		m_p->trigger[1] = leftstate.buttons & 0x02;
		m_p->trigger_left[1] = leftstate.buttons & 0x02;

		m_mutex.Release();
	}

private:
	void Initialize() {
		m_mutex.Wait();

		m_p = (FreePIEFileMapping *)m_fileMapping.Map(FILE_MAP_WRITE);
		memset(m_p, 0, sizeof(FreePIEFileMapping));

		m_mutex.Release();
	}

	uint32_t ConvertButtonMappings(NOLO::ControllerStates state) {
		UINT noloButton = state.buttons;
		NOLO::Vector2 noloTouch = state.touchpadAxis;
		bool touched = state.touches;

		bool press = noloButton & (1 << NOLO::NoloButtonID::TouchPad);
		bool trigger = noloButton & (1 << NOLO::NoloButtonID::Trigger);
		bool menu = noloButton & (1 << NOLO::NoloButtonID::Menu);
		bool system = noloButton & (1 << NOLO::NoloButtonID::System);
		bool grip = noloButton & (1 << NOLO::NoloButtonID::Grip);

		bool dpad_left = press & (noloTouch.x < -0.3);
		bool dpad_right = press & (noloTouch.x > 0.3);
		bool dpad_down = press & (noloTouch.y < -0.3);
		bool dpad_up = press & (noloTouch.y > 0.3);

		uint32_t alvrButton = 0;
		alvrButton |= (uint32_t)system;
		alvrButton |= (uint32_t)menu << 1;
		alvrButton |= (uint32_t)grip << 2;
		alvrButton |= (uint32_t)dpad_left << 3;
		alvrButton |= (uint32_t)dpad_up << 4;
		alvrButton |= (uint32_t)dpad_right << 5;
		alvrButton |= (uint32_t)dpad_down << 6;
		alvrButton |= (uint32_t)(press & touched) << 11;
		alvrButton |= (uint32_t)(!press & touched) << 12;
		alvrButton |= (uint32_t)trigger << 13;

		return alvrButton;
	}

	IPCFileMapping m_fileMapping;
	IPCMutex m_mutex;

	FreePIEFileMapping *m_p;
};

