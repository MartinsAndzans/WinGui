#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <string>

class Graphics {
private:
	ID2D1Factory *Factory = nullptr;
public:

	bool Init() {

		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory))) {
			return false;
		}

		return true;

	}

	~Graphics() {
		Factory->Release();
	}

};
