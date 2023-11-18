#include <Windows.h>
#include <SetupAPI.h>
#include <Cfgmgr32.h>
#include <devguid.h>
#include <initguid.h>
#include <algorithm>
#include <iostream>
#include <chrono>
#pragma comment (lib, "Setupapi.lib")

#define TIME 5000.0

bool changeMouseActive(DWORD state) {
	HDEVINFO devices = SetupDiGetClassDevsW(&GUID_DEVCLASS_MOUSE, nullptr, 0, 2);

	if (!devices) return false;

	SP_DEVINFO_DATA data = { sizeof(SP_DEVINFO_DATA) };
	int index = 0;

	while (SetupDiEnumDeviceInfo(devices, index, &data)) {
		SP_PROPCHANGE_PARAMS params = { 0 };

		params.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		params.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		params.Scope = DICS_FLAG_GLOBAL;
		params.StateChange = state;

		bool a = SetupDiSetClassInstallParamsA(devices, &data, &params.ClassInstallHeader, sizeof(params));
		bool b = SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devices, &data);

		if (!a || !b) return false;

		index++;
	}

	return index ? true : false; // return index and cast
}

void main() {
	using namespace std::chrono;
		
	bool detected = false;
	POINT oldMouse = { 0 };
	POINT newMouse = { 0 };
	steady_clock::time_point startClock;

	if (!changeMouseActive(DICS_DISABLE)) return;

	if (!GetCursorPos(&oldMouse)) {
		detected = true;
		goto End;
	}

	startClock = high_resolution_clock::now();
	while (duration<double, std::milli>(high_resolution_clock::now() - startClock).count() < TIME) {
		
		if (!GetCursorPos(&newMouse)) {
			detected = true;
			goto End;
		}

		if (oldMouse.x != newMouse.x || oldMouse.y != newMouse.y) {
			detected = true;
			goto End;
		}
	}

End:
	while (!changeMouseActive(DICS_ENABLE)){}


	printf("Mouse was %smoved by software", detected ? "" : "not ");

	
}