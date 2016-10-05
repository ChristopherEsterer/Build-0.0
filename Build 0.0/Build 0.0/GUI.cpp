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
float MaptoYEMG(std::deque<double>*dataBuf, int* MAX, int* MIN, double* data)
{
	double DataMax = *std::max_element(dataBuf->begin(), dataBuf->end());
	double DataMin = *std::min_element(dataBuf->begin(), dataBuf->end());
	double DataRange = (DataMax - DataMin);
	//double ScaleFactor;
	double PixleRange = (*MAX - *MIN);
	//if (DataRange == 0) { return 0; }
	double DataNorm = *data - DataMin;
	//double T = (((PixleRange / DataRange) * data));
	double T = ((PixleRange / 1000) * *data);
	return (float)T;
}
float MaptoYForce(std::deque<double>*dataBuf, int* MAX, int* MIN, double* data)
{
	//double DataMax = *std::max_element(dataBuf->begin(), dataBuf->end());
	//double DataMin = *std::min_element(dataBuf->begin(), dataBuf->end());
	//double DataRange = (DataMax - DataMin);
	//double ScaleFactor;
	double PixleRange = (*MAX - *MIN);
	//if (DataRange == 0) { return 0; }
	double DataNorm = *data - 10300;
	//double T = (((PixleRange / DataRange) * data));
	double T = ((PixleRange / 700) * DataNorm);
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
			//if (dataBuffer[1].size() == 0)
			//{
				//m_nStartTime = TempBody->getData(JointType0, JointType1, (Datatype - (Datatype % 10) + 4));
			//}
			double tempData;
			double temptime;
			do {
				tempData = TempBody->getData(JointType0, JointType1, Datatype);
				temptime = TempBody->getData(JointType0, JointType1, (Datatype - (Datatype % 10) + 4));
			} while (tempData == 0 || temptime == -INFINITY);
			if (dataBuffer[1].size() == 0)
			{
				m_nStartTime = temptime;
			}
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
	if (!EMGGUICLASS)
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

	GUIApp* TempEmg;
	TempEmg = (GUIApp*)EMGGUICLASS;
	OptiBody* TempBody;
	TempBody = (OptiBody*)m_UserBody;
	if (TempBody->getNewDataFlag())
	{

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
				if (dataBufferSpine[0][1].size() == 0)
				{
					m_nStartTime = temptime;
				}
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
				WristAccelAverage();
				WristVeloAverage();
				WristPosAverage();
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
				CheckLiftPos();
			//	FormPoints = HipsYVHipAngV();
			//	FormPoints = KneeAngVHipAngV();
			//	FormPoints = KneeAngHipAng();
				ID2D1Brush* BGBrush;
				switch (BGState) {
				case 1:
					BGBrush = YellowGreen;
					break;
				case 0:
					BGBrush = Black;
					break;
				case 2:
					if (StrokeWarn = 1) { BGBrush = BGBrush = OrangeRed; }
					else { BGBrush = Black; }
					break;
				}

				ID2D1Brush* StrokeBrush;
				switch (StrokeWarn) {
				case 1:
					StrokeBrush = OrangeRed;
					break;
				case 0:
					StrokeBrush = YellowGreen;
					break;
				case 2:
					StrokeBrush = White;
					break;
				}
				GUIApp* TempEmg;
				TempEmg = (GUIApp*)EMGGUICLASS;
				D2D1_POINT_2F Point0;
				D2D1_POINT_2F Point1;
				//Blue bar on the left will show your lift stroke
				Point0.x = (width / 4);
				Point0.y = (height-30);
				Point1.x = (width / 4);
				Point1.y = (height-30)-(LiftStroke*8.0E1);
				m_pRenderTarget->DrawLine(Point0, Point1, StrokeBrush, 30.0f);
				// Bar on right is current lift
				Point0.x = (width * 3/4);
				Point0.y = (height - 30);
				Point1.x = (width * 3/4);
				Point1.y = (height - 30) - (CurrentLift*8.0E1);


				ID2D1Brush* DotBrush;
				switch (liftState) {
				case 1://bot
					DotBrush = m_pBrushGraphLine;
					break;
				case 2://up
					DotBrush = m_pBrushGraphLine2;
					break;
				case 3://top
					DotBrush = m_pBrushGraphLine3;
					break;
				case 4://down
					DotBrush = m_pBrushGraphLine4;
					break;
				}
				m_pRenderTarget->DrawLine(Point0, Point1, StrokeBrush, 30.0f);

				Point0.x = (width / 2);
				Point0.y = (height / 2);
				D2D1_ELLIPSE ellipse = D2D1::Ellipse(Point0, 45.0F, 45.0F);
				m_pRenderTarget->FillEllipse(ellipse, DotBrush);
				//1,2,3,4,1,
				if (243 < abs(TempEmg->getEmgAvg()) && 250 > abs(TempEmg->getEmgAvg()) && 1.0E-8 > abs(WristVeloAvg)) {
					
					Point0.x = (width / 2);
					Point0.y = (height / 2);
					D2D1_ELLIPSE ellipse = D2D1::Ellipse(Point0, 45.0F, 45.0F);
					m_pRenderTarget->FillEllipse(ellipse, m_pBrushGraphLine2);
				}

				
				FormPoints = FormPoints+KneeAngV();
			/*	if (liftState) { Brush = m_pBrushGraphLine; }
				else { Brush = m_pBrushGraphLine5; }

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
				*/
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

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DodgerBlue, 0.7f), &m_pBrushGraphLine);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.7f), &m_pBrushGraphLine2);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 0.7f), &m_pBrushGraphLine3);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange, 0.7f), &m_pBrushGraphLine4);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.7f), &m_pBrushGraphLine5);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple, 0.7f), &Purple);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::OrangeRed, 0.8f), &OrangeRed);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::YellowGreen, 0.8f), &YellowGreen);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &White);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &Black);
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
		return std::make_tuple(JointType_WristRight, JointType_WristRight,111); // WristYVelo
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
	if (WristPosAvg  < WristMin && abs(WristVeloAvg) < 5.0E-9)
	{
		WristMin = WristPosAvg;
	}	
	if (WristPosAvg > WristMax && abs(WristVeloAvg) < 5.0E-9) {
		WristMax = WristPosAvg;
	}
	LiftStroke = WristMax - WristMin;
	CurrentLift = WristPosAvg - WristMin;

	if (CurrentLift > (LiftStroke*.75) &&  WristVeloAvg > 0 && abs(WristVeloAvg) < 5.0E-9) { // within 80% of top 
		liftState = 3; // top
	}
	else if (CurrentLift < (LiftStroke*0.25) && abs(WristVeloAvg) < 5.0E-9) {
		liftState = 1; // bottom
	}
	else if (CurrentLift < LiftStroke && WristPosAvg > WristMin && WristVeloAvg > 3.0E-8) {
		liftState = 2; // upstroke
	}
	else if (CurrentLift < LiftStroke && WristPosAvg > WristMin && WristVeloAvg < -3.0E-8) {
		liftState = 4; // downstroke
	}

	if (liftStates.empty()){
	liftStates.push_back(liftState);
	}
	else if (liftState != *(liftStates.end() - 1)) { // if the lift state changed
	liftStates.push_back(liftState);
	}

	if (liftStates.size() > 2) {
		if (liftStates.size() > 8) {
			liftStates.pop_front();
		}
		int* prev3State;
		int* prev2State;
		int* prevState = &*(liftStates.end()- 2);
			switch (*prevState) { // we know last and current state, we should check order.
			case 1: // was at bottom
				if (liftState == 2) { // now should be going up
					StrokeWarn = 0;
				}
				break;
			case 2: // was up
				if (liftState == 3) { // now should be at top
					StrokeWarn = 0;
				}
				else if(liftState == 4) {					
					liftStates.clear();
					StrokeWarn = 1;
					BGState = 2;
				}
				break;
			case 3: // was top
				if (liftState == 4) { // now should be going down
					StrokeWarn = 0;
				}
				break;
			case 4: // was down
				if (liftState == 1) { // now should be at bottom
					StrokeWarn = 0;
					// if we are at the bottom then we could possibly be finished our lift stroke.
					if (liftStates.size() >= 4) // 4123,4123
					{
						prev2State = &*(liftStates.end() - 3);
						prev3State = &*(liftStates.end() - 4);
						if (*prev2State == 3 && *prev3State == 2) 
						{
							StrokeWarn = 2;
							LiftCount++;
							//Beep(440, 500);
						 }
					}
				}
				break;
			}
	}
	/*if (WristVeloAvg > 0) {
		liftState = 0;
	}
	else {
		liftState = 1;
	}*/
}
void GUIApp::CheckEMGPos(void)
{

};
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

//	emgHighCount = 0;
//	emgLowCount = 0;
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
		//	do {
				tempData = TempBody->getData(EMG_itr);
				temptime = TempBody->getData(EMGTime_itr);
		//	} while (tempData == 0 || temptime == -INFINITY);
			
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
		//	do {
				tempData = TempBody->getData(EMG_itr);
				temptime = TempBody->getData(EMGTime_itr);
		//	} while (tempData == 0 || temptime == -INFINITY);

			dataBufferEMG[0].push_back(tempData);
			dataBufferEMG[1].push_back(temptime - m_nStartTime);
			dataBufferEMG[1].pop_front();
			dataBufferEMG[0].pop_front();
			CheckEMGAverage();
			if (dataBufferEMGAvg[0].size() < cDataBufferSize) {
				dataBufferEMGAvg[0].push_back(getEmgAvg());
				dataBufferEMGAvg[1].push_back(temptime - m_nStartTime);
			}
			else {
				dataBufferEMGAvg[0].push_back(getEmgAvg());
				dataBufferEMGAvg[1].push_back(temptime - m_nStartTime);
				dataBufferEMGAvg[1].pop_front();
				dataBufferEMGAvg[0].pop_front();
			}
		}
		SaveEMGBuffer();
		TempBody->setNewDataFlagEMG(FALSE);
	}
	else {
		Sleep(10); // Wait for a frame
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
			int height = rct.bottom -81;
			int Zero = 0;
			if (dataBufferEMG[0].size() != 0) {
				for (int i = 0; i < dataBufferEMG[0].size() - 1; i++)
				{
					D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBufferEMG[1], width, 0, dataBufferEMG[1][i]);
					PointTemp.y = ((height)/2) - MaptoYEMG(&dataBufferEMG[0], &height, &Zero, &dataBufferEMG[0][i]);
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBufferEMG[1], width, 0, dataBufferEMG[1][i + 1]);
					PointTemp2.y = ((height)/2) - MaptoYEMG(&dataBufferEMG[0], &height, &Zero, &dataBufferEMG[0][i + 1]);
					m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine, c_GraphLineThickness);
				}
			}

			if (dataBufferEMGAvg[0].size() != 0) {
				for (int i = 0; i < dataBufferEMGAvg[0].size() - 1; i++)
				{
					D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBufferEMGAvg[1], width, 0, dataBufferEMGAvg[1][i]);
					PointTemp.y = ((height) / 2) - MaptoYEMG(&dataBufferEMGAvg[0], &height, &Zero, &dataBufferEMGAvg[0][i]);
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBufferEMGAvg[1], width, 0, dataBufferEMGAvg[1][i + 1]);
					PointTemp2.y = ((height) / 2) - MaptoYEMG(&dataBufferEMGAvg[0], &height, &Zero, &dataBufferEMGAvg[0][i + 1]);
					m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine5, c_GraphLineThickness);
				}
			}
			D2D1_POINT_2F Point0;
			D2D1_POINT_2F Point1;
			//Point0.x = 0;
			//Point0.y = height;
			//Point1.x = width;
			//Point1.y = height;
			//m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine5, c_GraphLineThickness);
			//Point0.x = 0;
			double Emg244 = 244;
			Point0.y = ((height) / 2) - MaptoYEMG(&dataBufferEMGAvg[0], &height, &Zero, &Emg244);
			Point1.y = Point0.y;
			Point1.x = width;
			Point0.x = width;
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
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple, 0.5f), &Purple);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::OrangeRed, 0.5f), &OrangeRed);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::YellowGreen, 0.5f), &YellowGreen);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 0.5f), &White);
	}

	return hr;
}
void GUIApp::SaveEMGBuffer(void)
{

	Logfile.open("logEMG.csv", std::ios_base::app);
	if (Logfile.is_open())
	{
		double t0, tt;
		tt = dataBufferEMG[1].back();
		t0 = dataBufferEMG[0].back();

		Logfile << tt << "," << t0 << ",\n";
		Logfile.close();
		f_dataEMGSaved = true;
	}
}
void GUIApp::CheckEMGAverage(void)
{
	double tAvg = 0;
	int c = 0;
	for (std::deque<double>::iterator it = ((dataBufferEMG[0].end()) - 50); it != dataBufferEMG[0].end(); ++it) {
	//	if (*it != EMGLow) {
			tAvg += *it;
			c++;
	//	}
	}
	tAvg = tAvg / c;
	if (tAvg > EMGMaxAvg) {
		EMGMaxAvg = tAvg;
	}
	if (tAvg > EMGMaxAvg*zone) {
		MutexEmgCount.lock();
		emgHighCount=1;
		MutexEmgCount.unlock();
	}
	else if (tAvg < EMGMaxAvg*zone) {
		MutexEmgCount.lock();
		emgHighCount=0;
		MutexEmgCount.unlock();
	}
	MutexEmgCount.lock();
	emgAvg = tAvg;
	MutexEmgCount.unlock();

	return;
}
double GUIApp::WristAccelAverage(void)
{
	float tAvg = 0;
	int c = 0;
	for (std::deque<double>::iterator it = ((dataBufferSpine[SpineData_WristYAccel][0].end()) - 20); it != dataBufferSpine[SpineData_WristYAccel][0].end(); ++it) {
		tAvg += *it;
		c++;
	}
	tAvg = tAvg / c;
	return tAvg;
}
double GUIApp::WristVeloAverage(void)
{
	float tAvg = 0;
	int c = 0;
	for (std::deque<double>::iterator it = ((dataBufferSpine[4][0].end()) - 10); it != dataBufferSpine[4][0].end(); ++it) {
		tAvg += *it;
		c++;
	}
	tAvg = tAvg / c;
	WristVeloAvg = tAvg;
	return tAvg;
}
double GUIApp::WristPosAverage(void)
{
	float tAvg = 0;
	int c = 0;
	for (std::deque<double>::iterator it = ((dataBufferSpine[SpineData_WristYPos][0].end()) - 5); it != dataBufferSpine[SpineData_WristYPos][0].end(); ++it) {
		tAvg += *it;
		c++;
	}
	tAvg = tAvg / c;
	WristPosAvg = tAvg;
	return tAvg;
}
double GUIApp::ForceDataLAverage(void)
{
	float tAvg = 0;
	int c = 0;
	for (std::deque<double>::iterator it = ((dataBufferForce[0].end()) - 25); it != dataBufferForce[0].end(); ++it) {
		tAvg += *it;
		c++;
	}
	tAvg = tAvg / c;
	WristPosAvg = tAvg;
	return tAvg;
}
double GUIApp::ForceDataRAverage(void)
{
	float tAvg = 0;
	int c = 0;
	for (std::deque<double>::iterator it = ((dataBufferForce[1].end()) - 25); it != dataBufferForce[1].end(); ++it) {
		tAvg += *it;
		c++;
	}
	tAvg = tAvg / c;
	WristPosAvg = tAvg;
	return tAvg;
}
double GUIApp::ForceDiffAverage(void)
{
	float tAvg = 0;
	int c = 0;
	for (int i = dataBufferForce[0].size() - 10; i < dataBufferForce[0].size(); i++) {
		tAvg += (dataBufferForce[0][i] - dataBufferForce[1][i]);
		c++;
	}
	tAvg = tAvg / c;
	WristPosAvg = tAvg;
	return tAvg;
}
// Run Force
void GUIApp::SaveForceBuffer(void)
{

	Logfile.open("logForce.csv", std::ios_base::app);
	if (Logfile.is_open())
	{
		double t0,t1, tt;
		tt = dataBufferForce[2].back();
		t0 = dataBufferForce[0].back();
		t1 = dataBufferForce[1].back();
		Logfile << tt << "," << t0 << "," << t1 << ",\n";
		Logfile.close();
		f_dataForceSaved = true;
	}
}
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
	
	// ** Run GetMass
	
	//ShowWindow(MassWnd, nCmdShow);
	DWORD TEMPE = GetLastError();
	// Show window
	ShowWindow(hWndApp, nCmdShow);
	
	//DialogBox(hInstance, MAKEINTRESOURCE(IDD_GETMASS), hWndApp, (DLGPROC)GetMass);
	
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
void GUIApp::CheckEMGHigh(double data)
{
	if (data > EMGHigh) {
		emgHighCount++;
	}
}
void GUIApp::CheckEMGLow(double data)
{
	if (EMGLow > data) {
		emgLowCount++;
	}
}
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
	//		do {
				tempDataR = TempBody->getData(ForceR_itr);
				tempDataL = TempBody->getData(ForceL_itr);
				temptime = TempBody->getData(ForceTime_itr);
	//		} while (tempDataL == 0 || temptime == -INFINITY);
			
			if (dataBufferForce[1].size() == 0)
			{
				m_nStartTime = temptime;
			}
			
			dataBufferForce[0].push_back(tempDataL);
			dataBufferForce[1].push_back(tempDataR);
			dataBufferForce[2].push_back(temptime - m_nStartTime);

			if(EMGOffSets.size() < 30){
				EMGOffSets.push_back(tempDataR - tempDataL);
			}
			else {
				float tAvg = 0;
				int c = 0;
				for (std::deque<double>::iterator it = ((EMGOffSets.end()) - 29); it != EMGOffSets.end(); ++it) {
					tAvg += *it;
					c++;
				}
				tAvg = tAvg / c;
				//WristPosAvg = tAvg;
				ForceOffset = tAvg;
			}
		}
		else
		{
			double tempDataR;
			double tempDataL;
			double temptime;
		//	do {
				tempDataR = TempBody->getData(ForceR_itr);
				tempDataL = TempBody->getData(ForceL_itr);
				temptime = TempBody->getData(ForceTime_itr);
		//	} while (tempDataL == 0 || temptime == -INFINITY);

			dataBufferForce[0].push_back(tempDataL + ForceOffset);
			dataBufferForce[1].push_back(tempDataR);
			dataBufferForce[2].push_back(temptime - m_nStartTime);
			
			dataBufferForce[2].pop_front();
			dataBufferForce[1].pop_front();
			dataBufferForce[0].pop_front();

			if (dataBufferForceAvg[0].size() < cDataBufferSize) {
				dataBufferForceAvg[0].push_back(ForceDataLAverage());
				dataBufferForceAvg[1].push_back(ForceDataRAverage());
			//	if (EMGOffset == 0) {
			//		EMGOffset = dataBufferForceAvg[1].back() - dataBufferForceAvg[0].back();
			//	}
				
				dataBufferForceAvg[2].push_back(temptime - m_nStartTime);
			}
			else {
				dataBufferForceAvg[0].push_back(ForceDataLAverage());
				dataBufferForceAvg[1].push_back(ForceDataRAverage());
				dataBufferForceAvg[2].push_back(temptime - m_nStartTime);
				dataBufferForceAvg[1].pop_front();
				dataBufferForceAvg[2].pop_front();
				dataBufferForceAvg[0].pop_front();
			}

			/*if (dataBufferForceDiffAvg[0].size() < cDataBufferSize) {
				dataBufferForceDiffAvg[0].push_back(ForceDiffAverage());
				dataBufferForceDiffAvg[1].push_back(temptime - m_nStartTime);
			}
			else {
				dataBufferForceDiffAvg[0].push_back(ForceDiffAverage());
				dataBufferForceDiffAvg[1].push_back(temptime - m_nStartTime);
				dataBufferForceDiffAvg[1].pop_front();
				dataBufferForceDiffAvg[0].pop_front();
			}*/
		}

		SaveForceBuffer();
		TempBody->setNewDataFlagForce(FALSE);
	}
	else {
		Sleep(20); // Wait for a frame
	}
	DisplayForce();
	return;
}
void GUIApp::DisplayForce(void)
{
	//if (m_hWnd)
	//{
	//	HRESULT hr = EnsureDirect2DResourcesForce();

	//	if (SUCCEEDED(hr) && m_pRenderTarget)
	//	{
	//		m_pRenderTarget->BeginDraw();
	//		m_pRenderTarget->Clear();

	//		RECT rct;
	//		GetClientRect(GetDlgItem(m_hWnd, IDC_GRAPH_FORCE), &rct);
	//		int width = rct.right-80;
	//		int height = rct.bottom-75;
	//		if (dataBufferForceAvg[0].size() != 0) {
	//			//for (int i = 0; i < dataBuffer[0].size() - 1; i++)
	//		//	{

	//				D2D1_POINT_2F Point0;
	//				double ForceDiff = (dataBufferForceAvg[0].back() - dataBufferForceAvg[1].back() + (int)ForceOffset);

	//				Point0.x = (width / 2) +ForceDiff;
	//				Point0.y = (height / 2);
	//				
	//				
	//				ID2D1SolidColorBrush* EllipseBrush;
	//				if (abs(ForceDiff) > 50) {
	//					ForceDiffCount++;
	//					EllipseBrush = m_pBrushGraphLine;
	//				}
	//				else if (abs(ForceDiff) < 50) {
	//					EllipseBrush = m_pBrushGraphLine5;
	//				}
	//				 if (abs(ForceDiff) < 10) {
	//					 if(ForceDiffCount > -10){ ForceDiffCount--; }
	//					EllipseBrush = m_pBrushGraphLine2;
	//				}
	//				 float DotSize = 40.0f + ForceDiffCount;

	//				 D2D1_ELLIPSE ellipse = D2D1::Ellipse(Point0, DotSize, DotSize);
	//				m_pRenderTarget->FillEllipse(ellipse, EllipseBrush);
	//				
	//				D2D1_POINT_2F Point1;
	//				Point0.x = width/2;
	//				Point0.y = 0;
	//				Point1.x = width/2;
	//				Point1.y = height;
	//				m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine3, c_GraphLineThickness);

	//				/*D2D1_POINT_2F PointTemp;
	//				PointTemp.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i]);
	//				PointTemp.y = ((height - 80) / 2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i]);
	//				D2D1_POINT_2F PointTemp2;
	//				PointTemp2.x = Mapto(dataBuffer[1], width, 0, dataBuffer[1][i + 1]);
	//				PointTemp2.y = ((height - 80) / 2) - MaptoY(dataBuffer[0], (height - 80), 0, dataBuffer[0][i + 1]);
	//				m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine, c_GraphLineThickness);*/
	//			//}
	//		}
	//		/* if (dataBufferForce[0].size() != 0) {
	//			int Zero = 0;
	//			for (int i = 0; i < dataBufferForce[0].size() - 1; i++)
	//			{
	//				D2D1_POINT_2F PointTemp;
	//				PointTemp.x = Mapto(dataBufferForce[1], width, 0, dataBufferForce[1][i]);
	//				PointTemp.y = ((height) / 4) - MaptoYForce(&dataBufferForce[0], &height, &Zero, &dataBufferForce[0][i]);
	//				D2D1_POINT_2F PointTemp2;
	//				PointTemp2.x = Mapto(dataBufferForce[1], width, 0, dataBufferEMG[1][i + 1]);
	//				PointTemp2.y = ((height) / 4) - MaptoYForce(&dataBufferForce[0], &height, &Zero, &dataBufferForce[0][i + 1]);
	//				m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine, c_GraphLineThickness);
	//			}
	//		} */
	//		D2D1_POINT_2F Point0;
	//		D2D1_POINT_2F Point1;
	//		Point0.x = width / 2;
	//		Point0.y = 0;
	//		Point1.x = width / 2;
	//		Point1.y = height;
	//		m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine3, c_GraphLineThickness);
	//	
	//	/*	D2D1_POINT_2F Point0;
	//		D2D1_POINT_2F Point1;
	//		Point0.x = 0;
	//		Point0.y = height - 80;
	//		Point1.x = width;
	//		Point1.y = height - 80;
	//		m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine5, c_GraphLineThickness);*/
	//	//	Point0.x = 0;
	//	//	Point0.y = Point0.y / 2;
	//	//	Point1.x = width;
	//	//	Point1.y = Point1.y / 2;
	//	//	m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine4, c_GraphLineThickness);
	//	}
	//	else {
	//		Sleep(1);
	//	}

	//	hr = m_pRenderTarget->EndDraw();
	//}

	if (m_hWnd)
	{
		HRESULT hr = EnsureDirect2DResourcesForce();

		if (SUCCEEDED(hr) && m_pRenderTarget)
		{
			m_pRenderTarget->BeginDraw();
			m_pRenderTarget->Clear();

			RECT rct;
			GetClientRect(GetDlgItem(m_hWnd, IDC_GRAPH_FORCE), &rct);
			int width = rct.right - 80;
			int height = rct.bottom - 75;
			int Zero = 0;
			if (dataBufferForceAvg[0].size() != 0) {
				for (int i = 0; i < dataBufferForceAvg[0].size() - 1; i++)
				{
					D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBufferForceAvg[2], width, 0, dataBufferForceAvg[2][i]);
					PointTemp.y = ((height) / 2) - MaptoYForce(&dataBufferForceAvg[0], &height, &Zero, &dataBufferForceAvg[0][i]);
					D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBufferForceAvg[2], width, 0, dataBufferForceAvg[2][i + 1]);
					PointTemp2.y = ((height) / 2) - MaptoYForce(&dataBufferForceAvg[0], &height, &Zero, &dataBufferForceAvg[0][i + 1]);
					m_pRenderTarget->DrawLine(PointTemp2, PointTemp, m_pBrushGraphLine, c_GraphLineThickness);

					//D2D1_POINT_2F PointTemp;
					PointTemp.x = Mapto(dataBufferForceAvg[2], width, 0, dataBufferForceAvg[2][i]);
					PointTemp.y = ((height) / 2) - MaptoYForce(&dataBufferForceAvg[1], &height, &Zero, &dataBufferForceAvg[1][i]);
					//D2D1_POINT_2F PointTemp2;
					PointTemp2.x = Mapto(dataBufferForceAvg[2], width, 0, dataBufferForceAvg[2][i + 1]);
					PointTemp2.y = ((height) / 2) - MaptoYForce(&dataBufferForceAvg[1], &height, &Zero, &dataBufferForceAvg[1][i + 1]);
					m_pRenderTarget->DrawLine(PointTemp2, PointTemp, m_pBrushGraphLine3, c_GraphLineThickness);
				}

			}

			//if (dataBufferEMGAvg[0].size() != 0) {
			//	for (int i = 0; i < dataBufferEMGAvg[0].size() - 1; i++)
			//	{
			//		D2D1_POINT_2F PointTemp;
			//		PointTemp.x = Mapto(dataBufferEMGAvg[1], width, 0, dataBufferEMGAvg[1][i]);
			//		PointTemp.y = ((height) / 2) - MaptoYEMG(&dataBufferEMGAvg[0], &height, &Zero, &dataBufferEMGAvg[0][i]);
			//		D2D1_POINT_2F PointTemp2;
			//		PointTemp2.x = Mapto(dataBufferEMGAvg[1], width, 0, dataBufferEMGAvg[1][i + 1]);
			//		PointTemp2.y = ((height) / 2) - MaptoYEMG(&dataBufferEMGAvg[0], &height, &Zero, &dataBufferEMGAvg[0][i + 1]);
			//		m_pRenderTarget->DrawLine(PointTemp, PointTemp2, m_pBrushGraphLine5, c_GraphLineThickness);
			//	}
			//}
			D2D1_POINT_2F Point0;
			D2D1_POINT_2F Point1;
			//Point0.x = 0;
			//Point0.y = height;
			//Point1.x = width;
			//Point1.y = height;
			//m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine5, c_GraphLineThickness);
			//Point0.x = 0;
			//double Emg244 = 244;
			//Point0.y = ((height) / 2) - MaptoYEMG(&dataBufferEMGAvg[0], &height, &Zero, &Emg244);
			//Point1.y = Point0.y;
			//Point1.x = width;
			//Point0.x = width;
			//m_pRenderTarget->DrawLine(Point0, Point1, m_pBrushGraphLine4, c_GraphLineThickness);
		}
		else {

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
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Purple, 0.5f), &Purple);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::OrangeRed, 0.5f), &OrangeRed);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::YellowGreen, 0.5f), &YellowGreen);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 0.5f), &White);
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
void GUIApp::setEMGGUIClass(void* UBC)
{
	EMGGUICLASS = UBC;
}
void GUIApp::setWirelessClass(void * DUST)
{
	m_WireLess = DUST;
}
int GUIApp::getEmgHighCount(void)
{
	int i;
	MutexEmgCount.lock();
	i = emgHighCount;
	MutexEmgCount.unlock();
	return i;
}
float GUIApp::getEmgAvg(void)
{
	float i;
	MutexEmgCount.lock();
	i = emgAvg;
	MutexEmgCount.unlock();
	return i;
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

INT_PTR CALLBACK GUIApp::GetMass(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK_GM || LOWORD(wParam) == IDCANCEL)
		{
			std::string MassString;
			GetDlgItemText(hDlg, IDC_USERMASS,(LPWSTR) &MassString, 10);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}

