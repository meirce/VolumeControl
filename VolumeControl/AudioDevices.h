#pragma once
#include <combaseapi.h>
#include <iostream>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <map>
#include <list>

class AudioDevices {
public:
	AudioDevices();
	~AudioDevices();
	std::list<LPWSTR>* GetDeviceNames();
	float GetDeviceVolume(LPWSTR deviceName);
	void SetDeviceVolume(LPWSTR deviceName, float volume);
	void ToggleMute(LPWSTR deviceName);
	void StepUpDeviceVolume(LPWSTR deviceName);
	void StepDownDeviceVolume(LPWSTR deviceName);
private:
	IMMDeviceEnumerator* deviceEnumerator;
	IMMDeviceCollection* deviceCollection;
	void InitializeDevices();
	std::map<LPWSTR, UINT>* GetAudioDevices();
	std::map<LPWSTR, UINT> *devices;
};
