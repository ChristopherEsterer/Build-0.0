#pragma once

#include "stdafx.h"
#include <strsafe.h>
#include "Resource.h"
#include <algorithm>    // std::min_element, std::max_element
#define MAX_LOADSTRING 100

// Global Variables
static const float c_GraphLineThickness = 3.0f;
static const float c_SpineLineThickness = 6.0f;
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
	//double T = (((PixleRange / DataRange) * data));
	double T = ((PixleRange / DataRange) * DataNorm);
	return (float)T;
}
float MaptoYEMG(std::deque<double> dataBuf, float MAX, float MIN, double data)
{
	double DataMax = *std::max_element(dataBuf.begin(), dataBuf.end());
	double DataMin = *std::min_element(dataBuf.begin(), dataBuf.end());
	double DataRange = (DataMax - DataMin);
	//double ScaleFactor;
	double PixleRange = (MAX - MIN);
	//if (DataRange == 0) { return 0; }
	double DataNorm = data - DataMin;
	//double T = (((PixleRange / DataRange) * data));
	double T = ((PixleRange / 500) * data);
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
// Run Graph
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
					PointTemp.y = ((height - 80) / 2) - MaptoY(dataBuffer[0], (height - 80), 0, (dataBuffer[0][i]));
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i + 1]);
					PointTemp2.y = ((height - 80) / 2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i + 1]);
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
			Point0.y = Point0.y / 2;
			Point1.x = width;
			Point1.y = Point1.y / 2;
			m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine4, c_GraphLineThickness);
		}
		else {
			Sleep(1);
		}

		hr = m_pRenderTarget->EndDraw();
	}


}
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
// Run Spine
DWORD GUIApp::RunSpine(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId)
{
		MSG       msg = { 0 };
		WNDCLASS  wc;

		// Dialog custom window class
		ZeroMemory(&wc, sizeof(wc));
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbWndExtra = DLGWINDOWEXTRA;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.lpfnWndProc = DefDlgProcW;
		switch (C) {
		case 1:
			wc.lpszClassName = L"GUIGraphSpine";
			break;
		case 2:
			wc.lpszClassName = L"GUIGraphSpine";
			break;
		case 3:
			wc.lpszClassName = L"GUIGraphSpine";
			break;
		default:
			wc.lpszClassName = L"GUIGraphSpine";
			break;
		}
		C++;
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
			MAKEINTRESOURCE(IDD_GUI_SPINE), // Link to Resource
			hDlg,
			(DLGPROC)GUIApp::MessageRouter,
			reinterpret_cast<LPARAM>(this));


		DWORD TEMPE = GetLastError();
		// Show window
		ShowWindow(hWndApp, nCmdShow);
		Logfile = std::ofstream("log.csv");
		// Main message loop
		while (WM_QUIT != msg.message)
		{
			UpdateSpine();

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
}
void GUIApp::UpdateSpine(void)
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
	// To analyze the spine we need to look at multiple variables at once.
	// Multiple DataBuffers: HipsJointAngle,HipJointAngleV,KneeJointAngle, KneeJointAngleV, HipsYVelocity, WristYAcceleration.
	// Array of getData Codes for each Var
	// Make sure each Var is getting Calculated.
	// Analyse
	// Save to file
	//
	OptiBody* TempBody;
	TempBody = (OptiBody*)m_UserBody;
	if (TempBody->getNewDataFlag())
	{
		if (dataBufferSpine[0][1].size() == 0)
		{
			m_nStartTime = TempBody->getData(0, 0, 104);
		}
		for (int i = 0; i < dataBufferSpine.size(); i++)
		{
			if (dataBufferSpine[i][0].size() < cDataBufferSize)
			{
				double tempData;
				double temptime;
				int tj0;
				int tj1;
				int tDt;
				do {
					tj0 = std::get<0>(SpineGetMap(i));tj1 = std::get<1>(SpineGetMap(i));tDt = std::get<2>(SpineGetMap(i));
					tempData = TempBody->getData(tj0, tj1, tDt);
					temptime = TempBody->getData(tj0, tj1, (tDt - (tDt % 10) + 4));
				} while (tempData == 0 || temptime == -INFINITY);

				dataBufferSpine[i][0].push_back(tempData);
				dataBufferSpine[i][1].push_back(temptime - m_nStartTime);
			}
			else
			{
				double tempData;
				double temptime;
				int tj0;
				int tj1;
				int tDt;
				do {
					tj0 = std::get<0>(SpineGetMap(i)); tj1 = std::get<1>(SpineGetMap(i)); tDt = std::get<2>(SpineGetMap(i));
					tempData = TempBody->getData(tj0, tj1, tDt);
					temptime = TempBody->getData(tj0, tj1, (tDt - (tDt % 10) + 4));
				} while (tempData == 0 || temptime == -INFINITY);

				dataBufferSpine[i][0].push_back(tempData);
				dataBufferSpine[i][1].push_back(temptime - m_nStartTime);
				dataBufferSpine[i][1].pop_front();
				dataBufferSpine[i][0].pop_front();
			}
		}
		SaveSpineBuffer();
	//	f_dataSaved = false;
		TempBody->setNewDataFlag(FALSE);
	}
	else {
			Sleep(15); // Wait for a frame
	}

	DisplaySpine();
	return;
}
void GUIApp::DisplaySpine(void)
{
	if (m_hWnd)
	{
		HRESULT hr = EnsureDirect2DResourcesSpine();

		if (SUCCEEDED(hr) && m_pRenderTarget)
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear();
			ID2D1SolidColorBrush* Brush = m_pBrushGraphLine5;
			RECT rct;
			GetClientRect(GetDlgItem(m_hWnd, IDC_GRAPH_SPINE), &rct);
			int width = rct.right - 90;
			int height = rct.bottom - 80;
			if (!dataBufferSpine[0][0].empty())
			{
			//	FormPoints = HipsYVHipAngV();
			//	FormPoints = KneeAngVHipAngV();
			//	FormPoints = KneeAngHipAng();
				CheckLiftPos();
				FormPoints = FormPoints+KneeAngV();

				if (FormPoints >=4) {
					D2D1_POINT_2F Point0;
					Point0.x = (width / 2);
					Point0.y = (height / 2);
					D2D1_ELLIPSE ellipse = D2D1::Ellipse(Point0, 45.0F, 45.0F);
					m_pRenderTarget->FillEllipse(ellipse, Brush);

					
					//D2D1_POINT_2F Point1;
					//Point0.x = (width / 2) + (cSpinelineLenght);
					//Point0.y = (height / 2) + (cSpinelineLenght / 2);
					////Point1.x = width / 2 + ((cSpinelineLenght)*cos((3.14159265359/180)*dataBuffer[0].back()));
					////Point1.y = height / 2 + ((cSpinelineLenght)*sin((3.14159265359 / 180)*dataBuffer[0].back()));
					//Point1.x = (width / 2) + (cSpinelineLenght);
					//Point1.y = (height / 2) - (cSpinelineLenght / 2);
					//m_pRenderTarget->DrawLine(Point0, Point1, Brush, c_SpineLineThickness);
				}
				//FormPoints = 0;
			}
		hr = m_pRenderTarget->EndDraw();
		}
		else {
			Sleep(1);
		}

		
	}


}
HRESULT GUIApp::EnsureDirect2DResourcesSpine()
{
	HRESULT hr = S_OK;

	if (m_pD2DFactory && !m_pRenderTarget)
	{
		RECT rc;
		GetWindowRect(GetDlgItem(m_hWnd, IDC_GRAPH_SPINE), &rc);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		// Create a Hwnd render target, in order to render to the window set in initialize
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			rtProps,
			D2D1::HwndRenderTargetProperties(GetDlgItem(m_hWnd, IDC_GRAPH_SPINE), size),
			&m_pRenderTarget
			);

		if (FAILED(hr))
		{
			SetStatusMessage(L"Couldn't create Direct2D render target!", 10000, true);
			return hr;
		}

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushGraphLine);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.5f), &m_pBrushGraphLine2);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 0.5f), &m_pBrushGraphLine3);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange, 0.5f), &m_pBrushGraphLine4);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushGraphLine5);

	}

	return hr;
}
std::tuple<int,int,int> GUIApp::SpineGetMap(int i)
{
	switch (i) {
	case SpineData_HipAngle:
		return std::make_tuple(JointType_SpineBase, JointType_SpineBase,200);
	case SpineData_HipAngleVelo:
		return std::make_tuple(JointType_SpineBase, JointType_SpineBase,210);
	case SpineData_KneeAngle:
		return std::make_tuple(JointType_KneeRight, JointType_KneeRight,200);
	case SpineData_KneeAngleVelo:
		return std::make_tuple(JointType_KneeRight, JointType_KneeRight,210);
	case SpineData_HipsYVelo:
		return std::make_tuple(JointType_SpineBase, JointType_SpineBase,111);
	case SpineData_WristYAccel:
		return std::make_tuple(JointType_WristRight, JointType_WristRight,121);
	case SpineData_WristYPos:
		return std::make_tuple(JointType_WristRight, JointType_WristRight, 101);
	}

	
}
void GUIApp::SaveSpineBuffer(void)
{
	
	Logfile.open("log.csv", std::ios_base::app);
	if (Logfile.is_open())
	{
		double t0, t1, t2, t3, t4, t5,t6,tt;
		tt = dataBufferSpine[SpineData_HipAngle][1].back();
		t0 = dataBufferSpine[SpineData_HipAngle][0].back();
		t1 = dataBufferSpine[SpineData_HipAngleVelo][0].back();
		t2 = dataBufferSpine[SpineData_KneeAngle][0].back();
		t3 = dataBufferSpine[SpineData_KneeAngleVelo][0].back();
		t4 = dataBufferSpine[SpineData_HipsYVelo][0].back();
		t5 = dataBufferSpine[SpineData_WristYAccel][0].back();
		t6 = dataBufferSpine[SpineData_WristYPos][0].back();
		Logfile << tt << "," << t0 << "," << t1 << "," << t2 << "," << t3 << "," << t4 << "," << t5 << "," << t6 << ",\n";
		Logfile.close();
		f_dataSaved = true;
	}
}
void GUIApp::CheckLiftPos(void)
{
	if (dataBufferSpine[SpineData_WristYPos][0].back() < WristMin) 
	{
		WristMin = dataBufferSpine[SpineData_WristYPos][0].back();
	}	
	if (dataBufferSpine[SpineData_WristYPos][0].back() > WristMax) {
		WristMax = dataBufferSpine[SpineData_WristYPos][0].back();
	}


	if (abs(dataBufferSpine[SpineData_WristYPos][0].back() - WristMax) < (0.01)) {
		liftState = 1;
	}
	
	if (abs(dataBufferSpine[SpineData_WristYPos][0].back() - WristMin) < (0.01)) {
	
		if (liftState = 1)
		{
			liftState = 0; LiftCount++; FormPoints = 0;
		}
		else {
			liftState = 0;
		};
	}
}
int GUIApp::HipsYVHipAngV(void)
{
	if (dataBufferSpine[SpineData_HipsYVelo][0].back() > dataBufferSpine[SpineData_HipAngleVelo][0].back()) {
		return 1;
	}
	else {
		return 0;
	}
	
}
int GUIApp::KneeAngVHipAngV(void)
{
	if ((dataBufferSpine[SpineData_KneeAngleVelo][0].back() - dataBufferSpine[SpineData_HipAngleVelo][0].back()) > 0) {
		return 1;
	}
	else { return 0; }
}
int GUIApp::KneeAngHipAng(void)
{
	if (dataBufferSpine[SpineData_KneeAngle][0].back() < dataBufferSpine[SpineData_HipAngle][0].back()) {
		return 1;
	}
	else { return 0; }
}
int GUIApp::KneeAngV(void)
{
	if (abs(dataBufferSpine[SpineData_KneeAngleVelo][0].back()) > 4E-5 ) {
		return 1;
	}
	else { return 0; }
	return 0;
}
// Run EMG
DWORD GUIApp::RunEMG(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId)
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
		wc.lpszClassName = L"GUIGraphEMG";
		break;
	case 2:
		wc.lpszClassName = L"GUIGraphEMG";
		break;
	case 3:
		wc.lpszClassName = L"GUIGraphEMG";
		break;
	default:
		wc.lpszClassName = L"GUIGraphEMG";
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
		MAKEINTRESOURCE(IDD_GUI_EMG), // Link to Resource
		hDlg,
		(DLGPROC)GUIApp::MessageRouter,
		reinterpret_cast<LPARAM>(this));


	DWORD TEMPE = GetLastError();
	// Show window
	ShowWindow(hWndApp, nCmdShow);

	// Main message loop
	while (WM_QUIT != msg.message)
	{
		UpdateEMG();
		
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
void GUIApp::UpdateEMG(void)
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

	if (!m_WireLess)
	{
		return;
	}

	Wireless* TempBody;
	TempBody = (Wireless*)m_WireLess;
	if (TempBody->getNewDataFlagEMG())
	{
		if (dataBufferEMG[0].size() < cDataBufferSize)
		{

			double tempData;
			double temptime;
			do {
				tempData = TempBody->getData(EMG_itr);
				temptime = TempBody->getData(EMGTime_itr);
			} while (tempData == 0 || temptime == -INFINITY);
			
			if (dataBufferEMG[1].size() == 0)
			{
				m_nStartTime = temptime;
			}


			dataBufferEMG[0].push_back(tempData);
			dataBufferEMG[1].push_back(temptime - m_nStartTime);
		}
		else
		{
			double tempData;
			double temptime;
			do {
				tempData = TempBody->getData(EMG_itr);
				temptime = TempBody->getData(EMGTime_itr);
			} while (tempData == 0 || temptime == -INFINITY);

			dataBufferEMG[0].push_back(tempData);
			dataBufferEMG[1].push_back(temptime - m_nStartTime);
			dataBufferEMG[1].pop_front();
			dataBufferEMG[0].pop_front();
		}
		TempBody->setNewDataFlagEMG(FALSE);
	}
	else {
		Sleep(13); // Wait for a frame
	}
	DisplayEMG();
	return;
}
void GUIApp::DisplayEMG(void)
{
	if (m_hWnd)
	{
		HRESULT hr = EnsureDirect2DResourcesEMG();

		if (SUCCEEDED(hr) && m_pRenderTarget)
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear();

			RECT rct;
			GetClientRect(GetDlgItem(m_hWnd, IDC_GRAPH_EMG), &rct);
			int width = rct.right;
			int height = rct.bottom;
			if (dataBufferEMG[0].size() != 0) {
				for (int i = 0; i < dataBufferEMG[0].size() - 1; i++)
				{
					D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBufferEMG[1], width, 0, dataBufferEMG[1][i]);
					PointTemp.y = ((height - 81)/2) - MaptoYEMG(dataBufferEMG[0], (height - 81), 0, dataBufferEMG[0][i]-227);
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBufferEMG[1], width, 0, dataBufferEMG[1][i + 1]);
					PointTemp2.y = ((height - 81)/2) - MaptoYEMG(dataBufferEMG[0], (height - 81), 0, dataBufferEMG[0][i + 1]-227);
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
			Point0.y = Point0.y / 2;
			Point1.x = width;
			Point1.y = Point1.y / 2;
			m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine4, c_GraphLineThickness);
		}
		else {
			
		}

		hr = m_pRenderTarget->EndDraw();
	}


}
HRESULT GUIApp::EnsureDirect2DResourcesEMG()
{
	HRESULT hr = S_OK;

	if (m_pD2DFactory && !m_pRenderTarget)
	{
		RECT rc;
		GetWindowRect(GetDlgItem(m_hWnd, IDC_GRAPH_EMG), &rc);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		// Create a Hwnd render target, in order to render to the window set in initialize
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			rtProps,
			D2D1::HwndRenderTargetProperties(GetDlgItem(m_hWnd, IDC_GRAPH_EMG), size),
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
// Run Force
DWORD GUIApp::RunForce(HINSTANCE hInstance, int nCmdShow, HWND hDlg, int wmId)
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
		wc.lpszClassName = L"GUIGraphForce";
		break;
	case 2:
		wc.lpszClassName = L"GUIGraphForce";
		break;
	case 3:
		wc.lpszClassName = L"GUIGraphForce";
		break;
	default:
		wc.lpszClassName = L"GUIGraphForce";
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
		MAKEINTRESOURCE(IDD_GUI_FORCE), // Link to Resource
		hDlg,
		(DLGPROC)GUIApp::MessageRouter,
		reinterpret_cast<LPARAM>(this));


	DWORD TEMPE = GetLastError();
	// Show window
	ShowWindow(hWndApp, nCmdShow);

	// Main message loop
	while (WM_QUIT != msg.message)
	{
		UpdateForce();
		
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
void GUIApp::UpdateForce(void)
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

	if (!m_WireLess)
	{
		return;
	}

	Wireless* TempBody;
	TempBody = (Wireless*)m_WireLess;
	if (TempBody->getNewDataFlagForce())
	{
		if (dataBufferForce[0].size() < cDataBufferSize)
		{
			double tempDataL;
			double tempDataR;
			double temptime;
			do {
				tempDataR = TempBody->getData(ForceR_itr);
				tempDataL = TempBody->getData(ForceL_itr);
				temptime = TempBody->getData(ForceTime_itr);
			} while (tempDataL == 0 || temptime == -INFINITY);
			
			if (dataBufferForce[1].size() == 0)
			{
				m_nStartTime = temptime;
			}
			
			dataBufferForce[0].push_back(tempDataL);
			dataBufferForce[1].push_back(tempDataR);
			dataBufferForce[2].push_back(temptime - m_nStartTime);
		}
		else
		{
			double tempDataR;
			double tempDataL;
			double temptime;
			do {
				tempDataR = TempBody->getData(ForceR_itr);
				tempDataL = TempBody->getData(ForceL_itr);
				temptime = TempBody->getData(ForceTime_itr);
			} while (tempDataL == 0 || temptime == -INFINITY);

			dataBufferForce[0].push_back(tempDataL);
			dataBufferForce[1].push_back(tempDataR);
			dataBufferForce[2].push_back(temptime - m_nStartTime);
			
			dataBufferForce[2].pop_front();
			dataBufferForce[1].pop_front();
			dataBufferForce[0].pop_front();
		}
		TempBody->setNewDataFlagForce(FALSE);
	}
	else {
		Sleep(13); // Wait for a frame
	}
	//DisplayForce();
	return;
}
void GUIApp::DisplayForce(void)
{
	if (m_hWnd)
	{
		HRESULT hr = EnsureDirect2DResourcesForce();

		if (SUCCEEDED(hr) && m_pRenderTarget)
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear();

			RECT rct;
			GetClientRect(GetDlgItem(m_hWnd, IDC_GRAPH_FORCE), &rct);
			int width = rct.right-80;
			int height = rct.bottom-75;
			if (dataBufferForce[0].size() != 0) {
				//for (int i = 0; i < dataBuffer[0].size() - 1; i++)
			//	{

					D2D1_POINT_2F Point0;
					double ForceDiff = (dataBufferForce[0].back() - dataBufferForce[1].back());
					Point0.x = (width / 2) +ForceDiff;
					Point0.y = (height / 2);
					D2D1_ELLIPSE ellipse = D2D1::Ellipse(Point0, 45.0F, 45.0F);
					
					ID2D1SolidColorBrush* EllipseBrush;
					if (abs(ForceDiff) > 20) {
						EllipseBrush = m_pBrushGraphLine;
					}
					else if (abs(ForceDiff) < 20) {
						EllipseBrush = m_pBrushGraphLine5;
					}
					 if (abs(ForceDiff) < 10) {
						EllipseBrush = m_pBrushGraphLine2;
					}
					
						m_pRenderTarget->FillEllipse(ellipse, EllipseBrush);
					
					D2D1_POINT_2F Point1;
					Point0.x = width/2;
					Point0.y = 0;
					Point1.x = width/2;
					Point1.y = height;
					m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine3, c_GraphLineThickness);

					/*D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i]);
					PointTemp.y = ((height - 80) / 2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i]);
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i + 1]);
					PointTemp2.y = ((height - 80) / 2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i + 1]);
					m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine, c_GraphLineThickness);*/
				//}
			}
			D2D1_POINT_2F Point0;
			D2D1_POINT_2F Point1;
			Point0.x = width / 2;
			Point0.y = 0;
			Point1.x = width / 2;
			Point1.y = height;
			m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine3, c_GraphLineThickness);
		
		/*	D2D1_POINT_2F Point0;
			D2D1_POINT_2F Point1;
			Point0.x = 0;
			Point0.y = height - 80;
			Point1.x = width;
			Point1.y = height - 80;
			m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine5, c_GraphLineThickness);*/
		//	Point0.x = 0;
		//	Point0.y = Point0.y / 2;
		//	Point1.x = width;
		//	Point1.y = Point1.y / 2;
		//	m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine4, c_GraphLineThickness);
		}
		else {
			Sleep(1);
		}

		hr = m_pRenderTarget->EndDraw();
	}


}
HRESULT GUIApp::EnsureDirect2DResourcesForce()
{
	HRESULT hr = S_OK;

	if (m_pD2DFactory && !m_pRenderTarget)
	{
		RECT rc;
		GetWindowRect(GetDlgItem(m_hWnd, IDC_GRAPH_FORCE), &rc);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		// Create a Hwnd render target, in order to render to the window set in initialize
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			rtProps,
			D2D1::HwndRenderTargetProperties(GetDlgItem(m_hWnd, IDC_GRAPH_FORCE), size),
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
// Initilize
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
void GUIApp::setWirelessClass(void * DUST)
{
	m_WireLess = DUST;
}
// Stuff
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
		if (m_WireLess) {
		//	Wireless_hWnd =  
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


