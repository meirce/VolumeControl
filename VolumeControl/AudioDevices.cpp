#include "AudioDevices.h"

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioEndPointVolume = __uuidof(IAudioEndpointVolume);

AudioDevices::AudioDevices()
{
	InitializeDevices();
	this->devices = GetAudioDevices();
}

AudioDevices::~AudioDevices()
{
	delete this->devices;
	deviceCollection->Release();
	deviceEnumerator->Release();
}


void AudioDevices::InitializeDevices()
{
	HRESULT result;
	result = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC, IID_IMMDeviceEnumerator, (void**)&deviceEnumerator);
	if (FAILED(result)) {
		std::cout << "Failed Creating MMDeviceEnumerator. Error code: " << result << std::endl;
		exit(result);
	}
	result = deviceEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &deviceCollection);
	if (FAILED(result)) {
		std::cout << "Failed Getting Device Collection. Error code: " << result << std::endl;
		exit(result);
	}
}

std::list<LPWSTR>* AudioDevices::GetDeviceNames()
{
	auto deviceNames = new std::list<LPWSTR>;
	int i = 0;
	for (auto device : *devices) {
		deviceNames->push_back(device.first);
	}
	return deviceNames;
}

float AudioDevices::GetDeviceVolume(LPWSTR deviceName)
{
	float volume;

	for (auto dev : *devices) {
		if (lstrcmpW(dev.first, deviceName)) {
			IMMDevice *pAudioDevice;
			IAudioEndpointVolume *pEndpointVolume;
			deviceCollection->Item(devices->at(deviceName), &pAudioDevice);
			pAudioDevice->Activate(IID_IAudioEndPointVolume, CLSCTX_INPROC, NULL, (void**)&pEndpointVolume);
			pEndpointVolume->GetMasterVolumeLevelScalar(&volume);
			return volume;
		}
	}

	return 0.0f;
}

void AudioDevices::SetDeviceVolume(LPWSTR deviceName, float volume)
{
	for (auto dev : *devices) {
		if (lstrcmpW(dev.first, deviceName)) {
			IMMDevice *pAudioDevice;
			IAudioEndpointVolume *pEndpointVolume;
			deviceCollection->Item(devices->at(deviceName), &pAudioDevice);
			pAudioDevice->Activate(IID_IAudioEndPointVolume, CLSCTX_INPROC, NULL, (void**)&pEndpointVolume);
			pEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
			break;
		}
	}
}

void AudioDevices::ToggleMute(LPWSTR deviceName)
{
	for (auto dev : *devices) {
		if (lstrcmpW(dev.first, deviceName)) {
			IMMDevice *pAudioDevice;
			IAudioEndpointVolume *pEndpointVolume;
			deviceCollection->Item(devices->at(deviceName), &pAudioDevice);
			pAudioDevice->Activate(IID_IAudioEndPointVolume, CLSCTX_INPROC, NULL, (void**)&pEndpointVolume);
			BOOL is_mute;
			pEndpointVolume->GetMute(&is_mute);
			pEndpointVolume->SetMute(!is_mute, NULL);
			break;
		}
	}
}

void AudioDevices::StepUpDeviceVolume(LPWSTR deviceName)
{
	for (auto dev : *devices) {
		if (lstrcmpW(dev.first, deviceName)) {
			IMMDevice *pAudioDevice;
			IAudioEndpointVolume *pEndpointVolume;
			deviceCollection->Item(devices->at(deviceName), &pAudioDevice);
			pAudioDevice->Activate(IID_IAudioEndPointVolume, CLSCTX_INPROC, NULL, (void**)&pEndpointVolume);
			BOOL is_mute;
			pEndpointVolume->VolumeStepUp(NULL);
			break;
		}
	}
}

void AudioDevices::StepDownDeviceVolume(LPWSTR deviceName)
{
	for (auto dev : *devices) {
		if (lstrcmpW(dev.first, deviceName)) {
			IMMDevice *pAudioDevice;
			IAudioEndpointVolume *pEndpointVolume;
			deviceCollection->Item(devices->at(deviceName), &pAudioDevice);
			pAudioDevice->Activate(IID_IAudioEndPointVolume, CLSCTX_INPROC, NULL, (void**)&pEndpointVolume);
			BOOL is_mute;
			pEndpointVolume->VolumeStepDown(NULL);
			break;
		}
	}
}


std::map<LPWSTR, UINT>* AudioDevices::GetAudioDevices()
{
	auto *Devices = new std::map<LPWSTR, UINT>();
	HRESULT result;

	UINT count;

	result = deviceCollection->GetCount(&count);
	if (FAILED(result)) {
		std::cout << "Failed Getting Device Count. Error code: " << result << std::endl;
		exit(result);
	}

	if (count == 0) {
		std::cout << "No Devices found!" << std::endl;
		exit(0);
	}

	for (UINT i = 0; i < count; i++)
	{
		IMMDevice* device;
		IPropertyStore* mProperties;
		PROPVARIANT deviceDesc, friendlyName;

		PropVariantInit(&friendlyName);
		//PropVariantInit(&deviceDesc);

		deviceCollection->Item(i, &device);

		device->OpenPropertyStore(STGM_READ, &mProperties);


		result = mProperties->GetValue(PKEY_Device_FriendlyName, &friendlyName);
		//result = mProperties->GetValue(PKEY_Device_DeviceDesc, &deviceDesc);

		wchar_t *name = new wchar_t[1024];
		lstrcpyW(name, friendlyName.pwszVal);
		(*Devices)[name] = i;


		PropVariantClear(&friendlyName);
		//PropVariantClear(&deviceDesc);
		mProperties->Release();
		device->Release();

	}

	return Devices;
}
