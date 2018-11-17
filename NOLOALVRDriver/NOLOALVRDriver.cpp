#include "pch.h"
#include <iostream>

#include "ALVRInjector.h"
#include "nolo_api.h"

#pragma comment(lib,"lib/noloRuntime.lib")

using namespace NOLO;
using namespace std;

ALVRInjector injector;

void disconnectFunc(void* ctx) {
	cout << "Disconnected from NOLO Assistant" << endl;
}

void connectFunc(void* ctx) {
	cout << "Connected to NOLO Assistant" << endl;
}

void noloDataFunc(NoloData data, void* ctx) {
	/*
	Pose hpos = get_Nolo_Pose(1);
	Vector3 euler = QuaternionToEulerAngles(hpos.rot);

	cout << "position:" << hpos.pos.x << "," << hpos.pos.y << "," << hpos.pos.z << endl;
	cout << "orientation:" << hpos.rot.x << "," << hpos.rot.y << "," << hpos.rot.z << "," << hpos.rot.w << endl;
	cout << endl;
	*/
	injector.UpdateTrackingInfo(data);
}

int main() {
	registerDisConnectCallBack(disconnectFunc, nullptr);  
	registerConnectSuccessCallBack(connectFunc, nullptr);
	registerNoloDataNotifyCallBack(noloDataFunc, nullptr);
	open_Nolo_ZeroMQ();

	do {
		cout << "Press Enter to exit" << endl;
	} while (cin.get() != '\n');

	close_Nolo_ZeroMQ();
}
