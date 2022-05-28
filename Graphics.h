#pragma once

#include <ciso646>
#include <Windows.h>
#include <d2d1.h>
#include <vector>

class Graphics {
private:

	ID2D1Factory *Factory = nullptr;

public:

	Graphics() {
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &Factory);
	}

	void _fastcall DrawGrid(ID2D1DCRenderTarget *render_target, D2D1_POINT_2F position, D2D1_SIZE_F size, FLOAT rows, FLOAT collumns, D2D1_COLOR_F color) {

		

	}

	~Graphics() {
		Factory->Release();
	}

};
