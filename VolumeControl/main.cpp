#include "AudioDevices.h"


int main() {
	CoInitialize(NULL);

	auto devices = new AudioDevices();
	auto deviceNames = devices->GetDeviceNames();
	int i = 0;
	for (auto deviceName : *deviceNames) {

		printf("%S\n", deviceName);
		printf("volume %f\n", devices->GetDeviceVolume(deviceName));
		i++;
	}

	CoUninitialize();
	std::getchar();
}

