#include "Direct2D1.h"

std::string GetHRESULTErrorMessage(HRESULT hResult) {

	//=============== ERROR MESSAGE BUFFER ===============//
	constexpr size_t MAX_CHAR_STRING = 256;
	CHAR HRESULTErrorMessage[MAX_CHAR_STRING] = { 0 };
	//====================================================//

	DWORD Length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, HRESULT_CODE(hResult), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		HRESULTErrorMessage, MAX_CHAR_STRING, nullptr);

	if (Length == 0) {
		return "Error Code: " + std::to_string(HRESULT_CODE(hResult)) + " - Unknown Error";
	} else {
		return "Error Code: " + std::to_string(HRESULT_CODE(hResult)) + " - " + HRESULTErrorMessage;
	}

}

Direct2D1::Direct2D1(D2D1_FACTORY_TYPE FactoryType)  {

	//========== FACTORY PROPERTIES ==========//
	D2D1_FACTORY_OPTIONS factory_options;
	#if defined(DEBUG) || defined(_DEBUG)
	factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	#else // RELEASE
	factory_options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
	#endif // defined(DEBUG) || defined(_DEBUG)
	
	HRESULT hResult = S_OK;
	
	//========== CREATE FACTORY ==========//
	hResult = D2D1CreateFactory(FactoryType, factory_options, m_d2d1Factory.ReleaseAndGetAddressOf());
	if (hResult != S_OK) {
		throw(std::runtime_error(GetHRESULTErrorMessage(hResult)));
	}

	//========== RENDER TARGET PROPERTIES ==========//
	D2D1_RENDER_TARGET_PROPERTIES render_target_properties;
	render_target_properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	render_target_properties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
	render_target_properties.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
	render_target_properties.dpiX = 0.0F; render_target_properties.dpiY = 0.0F;
	render_target_properties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	//========== CREATE RENDER TARGET ==========//
	hResult = m_d2d1Factory->CreateDCRenderTarget(&render_target_properties, m_dcRenderTarget.ReleaseAndGetAddressOf());
	if (hResult != S_OK) {
		throw(std::runtime_error(GetHRESULTErrorMessage(hResult)));
	}

}

void Direct2D1::BeginDraw(HDC DeviceContext, const RECT &DrawingSurface, D2D1_COLOR_F ClearColor) {

	//========== CREATE RENDER TARGET RESOURCES ==========//
	m_dcRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0F, 0.0F, 0.0F), m_dcSolidColorBrush.ReleaseAndGetAddressOf());

	//========== BEGIN DRAW ==========//
	m_dcRenderTarget->BindDC(DeviceContext, &DrawingSurface);
	m_dcRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
	m_dcRenderTarget->BeginDraw();
	m_dcRenderTarget->Clear(ClearColor);

}

void Direct2D1::EndDraw(void) {

	//========== RELEASE RENDER TARGET RESOURCES ==========//
	m_dcSolidColorBrush.Reset();

	//========== END DRAW ==========//
	HRESULT hResult = m_dcRenderTarget->EndDraw();
	if (hResult == D2DERR_RECREATE_TARGET) {

		//========== RENDER TARGET PROPERTIES ==========//
		D2D1_RENDER_TARGET_PROPERTIES render_target_properties;
		render_target_properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		render_target_properties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
		render_target_properties.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
		render_target_properties.dpiX = 0.0F; render_target_properties.dpiY = 0.0F;
		render_target_properties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
		
		//========== RECREATE RENDER TARGET ==========//
		m_d2d1Factory->CreateDCRenderTarget(&render_target_properties, m_dcRenderTarget.ReleaseAndGetAddressOf());

	}

}

void Direct2D1::DrawCircle(D2D1_POINT_2F centerPoint, FLOAT Radius, D2D1_COLOR_F Color, FLOAT strokeWidth/*= 1.0F*/, const D2D1_MATRIX_3X2_F *Transform/*= nullptr*/) {

	m_dcSolidColorBrush->SetColor(Color);

	D2D1_ELLIPSE ellipse = {
		.point = centerPoint,
		.radiusX = Radius,
		.radiusY = Radius
	};

	if (Transform != nullptr) {

		D2D1_MATRIX_3X2_F OldTransform = { 0 };
		m_dcRenderTarget->GetTransform(&OldTransform);

		m_dcRenderTarget->SetTransform(Transform);
		m_dcRenderTarget->DrawEllipse(ellipse, m_dcSolidColorBrush.Get(), strokeWidth);

		m_dcRenderTarget->SetTransform(OldTransform);
	
	} else {

		m_dcRenderTarget->DrawEllipse(ellipse, m_dcSolidColorBrush.Get(), strokeWidth);

	}
}
