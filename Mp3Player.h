#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <string>

class Mp3Player {
public:

	Mp3Player(const Mp3Player&) = delete;
	Mp3Player operator=(const Mp3Player&) = delete;

	Mp3Player() : device_id(0U) {

		MCI_OPEN_PARMSW Open = { 0 };
		Open.lpstrDeviceType = L"mpegvideo";

		mciSendCommandW(NULL, MCI_OPEN, MCI_OPEN_TYPE, (DWORD_PTR)(&Open));

		this->device_id = Open.wDeviceID;

	}

	MCIERROR LoadFile(LPCWSTR FilePath) {

		MCI_OPEN_PARMSW Load = { 0 };
		Load.lpstrElementName = FilePath;

		MCIERROR LoadError = mciSendCommandW(this->device_id, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD_PTR)(&Load));

		return LoadError;

	}

	MCIERROR Play() {

		MCI_PLAY_PARMS Play = { 0 };
		Play.dwFrom = 0;

		MCIERROR PlayError = mciSendCommandW(this->device_id, MCI_PLAY, MCI_FROM, (DWORD_PTR)(&Play));

		return PlayError;

	}

	~Mp3Player() {
		mciSendCommandW(this->device_id, MCI_CLOSE, NULL, NULL);
	}

private:

	MCIDEVICEID device_id;

};