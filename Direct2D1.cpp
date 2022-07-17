#include "Direct2D1.h"

std::string GetHRESULTErrorMessage(HRESULT hResult) {

	//=============== ERROR MESSAGE BUFFER ===============//
	constexpr size_t MAX_CHAR_STRING = 256U;
	CHAR HRESULTErrorMessage[MAX_CHAR_STRING] = { 0 };
	//====================================================//

	//=============== GET HRESULT ERROR MESSAGE ===============//
	DWORD Length = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
		nullptr, HRESULT_CODE(hResult), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		HRESULTErrorMessage, MAX_CHAR_STRING - 1U, nullptr);
	//=========================================================//

	if (Length == 0) {
		return "Error Code: " + std::to_string(HRESULT_CODE(hResult)) + " - Unknown Error";
	} else {
		return "Error Code: " + std::to_string(HRESULT_CODE(hResult)) + " - " + HRESULTErrorMessage;
	}

}

Direct2D1::Direct2D1(D2D1_FACTORY_TYPE FactoryType)  {

	//========== FACTORY PROPERTIES ==========//
	D2D1_FACTORY_OPTIONS factory_options{};
	#if defined(DEBUG) || defined(_DEBUG)
	factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	#else // RELEASE
	factory_options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
	#endif // defined(DEBUG) || defined(_DEBUG)
	
	HRESULT hResult = S_OK;
	
	//========== CREATE FACTORY ==========//
	hResult = D2D1CreateFactory(FactoryType, factory_options, m_d2d1Factory.GetAddressOf());
	if (FAILED(hResult)) {
		throw(std::runtime_error(GetHRESULTErrorMessage(hResult)));
	}

	//========== RENDER TARGET PROPERTIES ==========//
	D2D1_RENDER_TARGET_PROPERTIES render_target_properties = {
		.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
		.pixelFormat = { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED },
		.dpiX = 0.0F, .dpiY = 0.0F,
		.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		.minLevel = D2D1_FEATURE_LEVEL_DEFAULT
	};

	//========== CREATE RENDER TARGET ==========//
	hResult = m_d2d1Factory->CreateDCRenderTarget(&render_target_properties, m_dcRenderTarget.GetAddressOf());
	if (FAILED(hResult)) {
		throw(std::runtime_error(GetHRESULTErrorMessage(hResult)));
	}

}

void Direct2D1::BeginDraw(HDC hdc, const RECT &DrawRect, const D2D1_COLOR_F &ClearColor) {

	//========== CREATE RENDER TARGET RESOURCES ==========//
	m_dcRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0F, 0.0F, 0.0F), m_dcSolidColorBrush.GetAddressOf());

	//========== BEGIN DRAW ==========//
	m_dcRenderTarget->BindDC(hdc, &DrawRect);
	m_dcRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
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
		D2D1_RENDER_TARGET_PROPERTIES render_target_properties = {
			.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
			.pixelFormat = { DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED },
			.dpiX = 0.0F, .dpiY = 0.0F,
			.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
			.minLevel = D2D1_FEATURE_LEVEL_DEFAULT
		};
		
		//========== RECREATE RENDER TARGET ==========//
		m_d2d1Factory->CreateDCRenderTarget(&render_target_properties, m_dcRenderTarget.ReleaseAndGetAddressOf());

	}

}

void Direct2D1::CreatePathGeometry(const std::vector<VERTEX_2F> &VertexBuffer, ID2D1PathGeometry **PathGeometry) {

	Microsoft::WRL::ComPtr<ID2D1GeometrySink> d2d1GeometrySink;

	m_d2d1Factory->CreatePathGeometry(PathGeometry);
	(*PathGeometry)->Open(d2d1GeometrySink.GetAddressOf());

	for (size_t i = 0; i < VertexBuffer.size(); i++) {

		if (i == 0) {
			d2d1GeometrySink->BeginFigure({ VertexBuffer[i].x, VertexBuffer[i].y }, D2D1_FIGURE_BEGIN_FILLED);
		} else {
			d2d1GeometrySink->AddLine({ VertexBuffer[i].x, VertexBuffer[i].y });
		}

	}

	d2d1GeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
	d2d1GeometrySink->Close();

}

void Direct2D1::DrawGeometry(const std::vector<VERTEX_2F> &VertexBuffer, const D2D1_COLOR_F &Color,
	FLOAT strokeWidth/*= 1.0F*/, const D2D1_MATRIX_3X2_F *Transform/*= nullptr*/) {

	if (!VertexBuffer.empty()) {

		Microsoft::WRL::ComPtr<ID2D1PathGeometry> d2d1PathGeometry;

		//========== CREATE GEOMETRY ==========//

		this->CreatePathGeometry(VertexBuffer, d2d1PathGeometry.GetAddressOf());

		//========== DRAW GEOMETRY ==========//

		m_dcSolidColorBrush->SetColor(Color);

		D2D1_MATRIX_3X2_F PrevTransform = { 0 };

		if (Transform != nullptr) {
			
			m_dcRenderTarget->GetTransform(&PrevTransform);
			m_dcRenderTarget->SetTransform(Transform);
			
			m_dcRenderTarget->DrawGeometry(d2d1PathGeometry.Get(), m_dcSolidColorBrush.Get(), strokeWidth);
			
			m_dcRenderTarget->SetTransform(&PrevTransform);
		
		} else {

			m_dcRenderTarget->DrawGeometry(d2d1PathGeometry.Get(), m_dcSolidColorBrush.Get(), strokeWidth);

		}

	}
	
}

void Direct2D1::FillGeometry(const std::vector<VERTEX_2F> &VertexBuffer, const D2D1_COLOR_F &Color, const D2D1_MATRIX_3X2_F *Transform/*= nullptr*/) {

	if (!VertexBuffer.empty()) {

		Microsoft::WRL::ComPtr<ID2D1PathGeometry> d2d1PathGeometry;

		//========== CREATE GEOMETRY ==========//

		this->CreatePathGeometry(VertexBuffer, d2d1PathGeometry.GetAddressOf());

		//========== DRAW GEOMETRY ==========//

		m_dcSolidColorBrush->SetColor(Color);

		D2D1_MATRIX_3X2_F PrevTransform = { 0 };

		if (Transform != nullptr) {

			m_dcRenderTarget->GetTransform(&PrevTransform);
			m_dcRenderTarget->SetTransform(Transform);

			m_dcRenderTarget->FillGeometry(d2d1PathGeometry.Get(), m_dcSolidColorBrush.Get());

			m_dcRenderTarget->SetTransform(&PrevTransform);

		} else {

			m_dcRenderTarget->FillGeometry(d2d1PathGeometry.Get(), m_dcSolidColorBrush.Get());

		}

	}

}

void Direct2D1::DrawEllipse(const D2D1_POINT_2F &centerPoint, FLOAT RadiusX, FLOAT RadiusY, const D2D1_COLOR_F &Color,
	FLOAT strokeWidth/*= 1.0F*/, const D2D1_MATRIX_3X2_F *Transform/*= nullptr*/) {

	m_dcSolidColorBrush->SetColor(Color);

	D2D1_ELLIPSE ellipse = {
		.point = centerPoint,
		.radiusX = RadiusX,
		.radiusY = RadiusY
	};


	if (Transform != nullptr) {
		
		D2D1_MATRIX_3X2_F PrevTransform = { 0 };
		m_dcRenderTarget->GetTransform(&PrevTransform);
		m_dcRenderTarget->SetTransform(Transform);
		
		m_dcRenderTarget->DrawEllipse(ellipse, m_dcSolidColorBrush.Get(), strokeWidth);
		
		m_dcRenderTarget->SetTransform(PrevTransform);

	} else {

		m_dcRenderTarget->DrawEllipse(ellipse, m_dcSolidColorBrush.Get(), strokeWidth);
	
	}

}

void Direct2D1::FillEllipse(const D2D1_POINT_2F &centerPoint, FLOAT RadiusX, FLOAT RadiusY, const D2D1_COLOR_F &Color,
	const D2D1_MATRIX_3X2_F *Transform/*= nullptr*/) {

	m_dcSolidColorBrush->SetColor(Color);

	D2D1_ELLIPSE ellipse = {
		.point = centerPoint,
		.radiusX = RadiusX,
		.radiusY = RadiusY
	};


	if (Transform != nullptr) {

		D2D1_MATRIX_3X2_F PrevTransform = { 0 };
		m_dcRenderTarget->GetTransform(&PrevTransform);
		m_dcRenderTarget->SetTransform(Transform);

		m_dcRenderTarget->FillEllipse(ellipse, m_dcSolidColorBrush.Get());

		m_dcRenderTarget->SetTransform(PrevTransform);

	} else {

		m_dcRenderTarget->FillEllipse(ellipse, m_dcSolidColorBrush.Get());

	}

}

void Direct2D1::DrawBitmap(D2D1_RECT_F Rect, const D2D1Bitmap &Bitmap, INT32 Frame/*= 0*/, const D2D1_MATRIX_3X2_F *Transform/*= nullptr*/) {



}
