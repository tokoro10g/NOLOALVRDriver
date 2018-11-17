#pragma once

#include <cmath>
#include "nolo_api.h"

/*
Notice:
The transformation is not standard since the coordinate systems in ALVR and NOLO are different.
*/
NOLO::Vector3 QuaternionToEulerAngles(NOLO::Quaternion q) {
	NOLO::Vector3 ret;
	double sinr = 2.0 * (-q.w * q.x - q.y * q.z);
	double cosr = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	ret.z = (float)atan2(sinr, cosr);

	double sinp = 2.0 * (-q.w * q.y + q.z * q.x);
	if (fabs(sinp) >= 1.0)
	{
		ret.y = (float)((sinp>0?1.0:-1.0) * 3.1415926536 / 2.0);
	}
	else
	{
		ret.y = (float)asin(sinp);
	}

	double siny = 2.0 * (q.w * q.z + q.x * q.y);
	double cosy = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	ret.x = (float)atan2(siny, cosy);

	return ret;
}

NOLO::Quaternion EulerAnglesToQuaternion(NOLO::Vector3 yaw_pitch_roll)
{
	NOLO::Quaternion q;
	double cy = cos(yaw_pitch_roll.x * 0.5);
	double sy = sin(yaw_pitch_roll.x * 0.5);
	double cr = cos(yaw_pitch_roll.z * 0.5);
	double sr = sin(yaw_pitch_roll.z * 0.5);
	double cp = cos(yaw_pitch_roll.y * 0.5);
	double sp = sin(yaw_pitch_roll.y * 0.5);

	q.w = (float)(cy * cr * cp + sy * sr * sp);
	q.x = -(float)(cy * sr * cp - sy * cr * sp);
	q.y = -(float)(cy * cr * sp + sy * sr * cp);
	q.z = (float)(sy * cr * cp - cy * sr * sp);
	return q;
}
