#pragma once

#include "stdafx.h"
#include <strsafe.h>
#include "Resource.h"
#include <algorithm>    // std::min_element, std::max_element
#define MAX_LOADSTRING 100

// Global Variables
static const float c_GraphLineThickness = 3.0f;
static int C = 0;

template<typename TYPE, typename TYPE2, typename TYPE3>
TYPE2 Mapto(std::deque<TYPE> dataBuf, TYPE2 MAX, TYPE2 MIN, TYPE3 data)
{
	//TYPE DataMax = *std::max_element(dataBuf.begin(),dataBuf.end());
	//TYPE DataMin = *std::min_element(dataBuf.begin(),dataBuf.end());
	TYPE DataMax = *std::max_element(dataBuf.begin(), dataBuf.end());
	TYPE DataMin = *std::min_element(dataBuf.begin(), dataBuf.end());
	// add check to min element. getting -inifin
	TYPE2 DataRange = (DataMax - DataMin);
	if (DataRange == 0) { return 0; }
	TYPE3 DataNorm = data - DataMin;
	TYPE3 T = ( (DataNorm / (DataRange)) * (MAX - MIN) );
	return T;
}
float MaptoY(std::deque<double> dataBuf, float MAX, float MIN, double data)
{
	double DataMax = *std::max_element(dataBuf.begin(), dataBuf.end());
	double DataMin = *std::min_element(dataBuf.begin(), dataBuf.end());
	double DataRange = (DataMax - DataMin);
	//double ScaleFactor;
	double PixleRange = (MAX - MIN);
	//if (DataRange == 0) { return 0; }
	double DataNorm = data - DataMin;
	double T = (((PixleRange / DataRange) * data));
	return (float)T;
}

GUIApp::GUIApp() :
m_hWnd(NULL),
m_nStartTime(0),
m_nLastCounter(0),
m_nFramesSinceUpdate(0),
m_fFreq(0),
m_nNextStatusTime(0LL)
{
	LARGE_INTEGER qpf = { 0 };
	if (QueryPerformanceFrequency(&qpf))
	{
		m_fFreq = double(qpf.QuadPart);
	}
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

GUIApp::~GUIApp()
{
}

DWORD GUIApp::Run(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId)
{

	MSG       msg = { 0 };
	WNDCLASS  wc;
	
	// Dialog custom window class
	ZeroMemory(&wc, sizeof(wc));
	
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	//wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));//***
	//	wc.hIcon = LoadIconW(NULL, NULL);//***
	//	wc.hIcon = LoadIconW(hInstance, nCmdShow);//***
	wc.lpfnWndProc = DefDlgProcW;
	//wc.lpszClassName = GUICLASS1;
	//wc.lpszClassName = GuiWindowClass;
	switch (C) {
	case 1:
		wc.lpszClassName = L"GUIGraphClassWindow1";
		break;
	case 2:
		wc.lpszClassName = L"GUIGraphClassWindow2";
		break;
	case 3:
		wc.lpszClassName = L"GUIGraphClassWindow3";
		break;
	default:
		wc.lpszClassName = L"GUIGraphClassWindow";
		break;
	}
	C++;
	//wc.lpszClassName = L"GUIGraphClassWindow";

	if (!RegisterClassW(&wc))
	{
		return 0;
	}

	// Create main application window
	/* HWND hWndApp = CreateDialogParamW(
	NULL,
	MAKEINTRESOURCE(IDD_APP),
	NULL,
	(DLGPROC)GUIApp::MessageRouter,
	reinterpret_cast<LPARAM>(this)); */
	HWND hWndApp = CreateDialogParamW(
		NULL,
		MAKEINTRESOURCE(IDD_GUI_GRAPH1), // Link to Resource
		hDlg,
		(DLGPROC)GUIApp::MessageRouter,
		reinterpret_cast<LPARAM>(this));


	DWORD TEMPE = GetLastError();
	// Show window
	ShowWindow(hWndApp, nCmdShow);

	// Main message loop
	while (WM_QUIT != msg.message)
	{
		Update();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If a dialog message will be taken care of by the dialog proc
			if (hWndApp && IsDialogMessageW(hWndApp, &msg))
			{
				continue;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	
	// Window is closeing, deregister the class.
	if (!UnregisterClassW(wc.lpszClassName, wc.hInstance))
	{
		return 0;
	}
	C--;
	return static_cast<int>(msg.wParam);
};
void GUIApp::Update(void)
{
/*
Check if UserBody,
Set TempBody
Case buffer size
	Ask TempBody For data (lock)
	Receive Data from Tempbody (unlock/return)
		Put data point into buffer
Display DataBufer()

*/

	if (!m_UserBody)
	{
		return;
	}

	OptiBody* TempBody;
	TempBody =  (OptiBody*) m_UserBody;
	if (TempBody->getNewDataFlag())
	{
		if (dataBuffer[0].size() < cDataBufferSize)
		{
			if (dataBuffer[1].size() == 0)
			{
				m_nStartTime = TempBody->getData(JointType0, JointType1, (Datatype - (Datatype % 10) + 4));
			}
			double tempData;
			double temptime;
			do {
				tempData = TempBody->getData(JointType0, JointType1, Datatype);
				temptime = TempBody->getData(JointType0, JointType1, (Datatype - (Datatype % 10) + 4));
			} while (tempData == 0 || temptime == -INFINITY);

			dataBuffer[0].push_back(tempData);
			dataBuffer[1].push_back(temptime - m_nStartTime);
		}
		else 
		{
			double tempData;
			double temptime;
			do {
				tempData = TempBody->getData(JointType0, JointType1, Datatype);
				temptime = TempBody->getData(JointType0, JointType1, (Datatype - (Datatype % 10) + 4));
			} while (tempData == 0 || temptime == -INFINITY);

			dataBuffer[0].push_back(tempData);
			dataBuffer[1].push_back(temptime - m_nStartTime);
			dataBuffer[1].pop_front();
			dataBuffer[0].pop_front();
		}
		TempBody->setNewDataFlag(FALSE);
	}
	else {	
		Sleep(13); // Wait for a frame
	}
	Display();
	return;
}
void GUIApp::setJointTypes(int J0, int J1)
{
	JointType0 = J0;
	JointType1 = J1;
}
void GUIApp::pushData(double data, double time)
{
	if (dataBuffer[0].size() < cDataBufferSize)
	{
		if (dataBuffer[1].size() == 0)
		{
			m_nStartTime = time;
		}
		dataBuffer[0].push_back(data);
		dataBuffer[1].push_back(time - m_nStartTime);

	}
	else {
		dataBuffer[0].push_back(data);
		dataBuffer[1].push_back(time - m_nStartTime);
		dataBuffer[1].pop_front();
		dataBuffer[0].pop_front();
	}
}
void GUIApp::setOptiBodyClass(void* UBC)
{
	m_UserBody = UBC;
}
;
LRESULT CALLBACK GUIApp::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	GUIApp* pThis = NULL;

	if (WM_INITDIALOG == uMsg)
	{
		pThis = reinterpret_cast<GUIApp*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<GUIApp*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
	//return LRESULT();
}
LRESULT GUIApp::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Bind application window handle
		m_hWnd = hWnd;

		OptiBody* TempBody;
		TempBody = (OptiBody*)m_UserBody;

		if (m_UserBody) {

			Kinect_hWnd = TempBody->getHWnd(); 
		}
		// Init Direct2D
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	}
	break;

	// If the titlebar X is clicked, destroy app
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// Quit the main message pump
		PostQuitMessage(0);
		break;
	}

	return FALSE;
}

void GUIApp::Display(void)
{
	if (m_hWnd)
	{
		HRESULT hr = EnsureDirect2DResources();

		if (SUCCEEDED(hr) && m_pRenderTarget)
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear();

			RECT rct;
			GetClientRect(GetDlgItem(m_hWnd, IDC_GRAPH), &rct);
			int width = rct.right;
			int height = rct.bottom;
			if (dataBuffer[0].size() != 0) {
				for (int i = 0; i < dataBuffer[0].size() - 1; i++)
				{
					D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i]);
					PointTemp.y = ((height - 80)/2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i]);
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i + 1]);
					PointTemp2.y = ((height - 80)/2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i + 1]) ;
					m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine, c_GraphLineThickness);
				}
			}
			D2D1_POINT_2F Point0;
			D2D1_POINT_2F Point1;
			Point0.x = 0;
			Point0.y = height - 80;
			Point1.x = width;
			Point1.y = height - 80;
			m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine5, c_GraphLineThickness);
			Point0.x = 0;
			Point0.y = Point0.y/2;
			Point1.x = width;
			Point1.y = Point1.y/2;
			m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine4, c_GraphLineThickness);
		}
		else {
			Sleep(1);
		}

		hr = m_pRenderTarget->EndDraw();
	}

	
}
bool GUIApp::SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce)
{
	INT64 now = GetTickCount64();

	if (m_hWnd && (bForce || (m_nNextStatusTime <= now)))
	{
		SetDlgItemText(m_hWnd, IDC_GUI_TEXT, szMessage);
		m_nNextStatusTime = now + nShowTimeMsec;

		return true;
	}

	return false;
}

/// <summary>
/// Ensure necessary Direct2d resources are created
/// </summary>
/// <returns>S_OK if successful, otherwise an error code</returns>
HRESULT GUIApp::EnsureDirect2DResources()
{
	HRESULT hr = S_OK;

	if (m_pD2DFactory && !m_pRenderTarget)
	{
		RECT rc;
		GetWindowRect(GetDlgItem(m_hWnd, IDC_GRAPH), &rc);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		// Create a Hwnd render target, in order to render to the window set in initialize
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			rtProps,
			D2D1::HwndRenderTargetProperties(GetDlgItem(m_hWnd, IDC_GRAPH), size),
			&m_pRenderTarget
			);

		if (FAILED(hr))
		{
			SetStatusMessage(L"Couldn't create Direct2D render target!", 10000, true);
			return hr;
		}

		// light green
	//	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.27f, 0.75f, 0.27f), &m_pBrushJointTracked);

	//	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &m_pBrushJointInferred);
	//	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &m_pBrushBoneTracked);
	//	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &m_pBrushBoneInferred);

	//	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushHandClosed);
	//	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.5f), &m_pBrushHandOpen);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushGraphLine);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.5f), &m_pBrushGraphLine2);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushGraphLine3);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple, 0.5f), &m_pBrushGraphLine4);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 0.5f), &m_pBrushGraphLine5);

	}

	return hr;
}

/// <summary>
/// Dispose Direct2d resources 
/// </summary>
void GUIApp::DiscardDirect2DResources()
{
	SafeRelease(m_pRenderTarget);

	SafeRelease(m_pBrushGraphLine);
	//SafeRelease(m_pBrushJointInferred);
	//SafeRelease(m_pBrushBoneTracked);
	//SafeRelease(m_pBrushBoneInferred);

	//SafeRelease(m_pBrushHandClosed);
	//SafeRelease(m_pBrushHandOpen);
	//SafeRelease(m_pBrushHandLasso);
}


